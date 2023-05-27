/*
 * C++ implementation of server.cpp
 * Jiwon Moon, Hajin Jang
 */

#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// add any additional data types that might be helpful
// for implementing the Server member functions

struct ConnInfo {
  Connection *conn;
  Server *server;
  ~ConnInfo() { delete conn; }
};

void chat_with_sender(Connection *conn, Server *server, User *user);
void chat_with_receiver(Connection *conn, Server *server, User *user);

namespace
{

  void *worker(void *arg) {
    pthread_detach(pthread_self());
    struct ConnInfo *temp_info = (ConnInfo *)arg;
    std::unique_ptr<ConnInfo> info(temp_info);

    // read login message (should be tagged either with
    // TAG_SLOGIN or TAG_RLOGIN), send response
    Message login_msg = Message();
    Connection* curr_conn = info->conn; // local variable for readability

    // handle receive failure
    if (!curr_conn->receive(login_msg)) {
      if (curr_conn->get_last_result() == Connection::INVALID_MSG) {
        curr_conn->send(Message(TAG_ERR, "given message is invalid"));
        return nullptr;
      } 
      curr_conn->send(Message(TAG_ERR, "failed to receive message"));
      return nullptr;
    }

    // handle invalid commands/attempts before logging in
    if (login_msg.tag != TAG_RLOGIN && login_msg.tag != TAG_SLOGIN) {
      curr_conn->send(Message(TAG_ERR, "must login first"));
      return nullptr;
    }

    // handle login failure
    std::string username = login_msg.data;
    if (!curr_conn->send(Message(TAG_OK, "logged in as " + username))) {
      return nullptr;
    }

    // depending on whether the client logged in as a sender or
    // receiver, communicate with the client (implementing
    // separate helper functions for each of these possibilities
    // is a good idea)

    User *user = new User(username);

    if (login_msg.tag == TAG_RLOGIN) {
      chat_with_receiver(curr_conn, info->server, user);
    } else { // since error handled above, TAG_SLOGIN otherwise
      chat_with_sender(curr_conn, info->server, user);
    }

    return nullptr;
  }

}

void chat_with_receiver(Connection *conn, Server *server, User *user) {
  // terminate the loop and tear down the client thread if any message
  // transmission fails or if quit message respond to join room
  Message msg = Message();
  bool received_message = conn->receive(msg);
  bool is_invalid = (conn->get_last_result() == Connection::INVALID_MSG);
  // handle failure to receive message
  if (!received_message) {
    if (!is_invalid) {
      conn->send(Message(TAG_ERR, "unable to receive message"));
      return;
    } else { // handle invalid message format
      conn->send(Message(TAG_ERR, "received invalid message"));
      return;
    }
  }
  // declare local variables to enhance readability
  std::string msg_tag = msg.tag;
  std::string msg_data = msg.data;
  Room *joined_room = nullptr;
  bool is_join_tag = (msg_tag == TAG_JOIN);
  if (!is_join_tag) {
    conn->send(Message(TAG_ERR, "not in a room"));
    return;
  }

  // checked that the message tag specifies join request
  joined_room = server->find_or_create_room(msg_data);
  joined_room->add_member(user);

  // handle failure to send confirmation that the user has joined a room
  if (!conn->send(Message(TAG_OK, "joined room " + joined_room->get_room_name()))) {
    return;
  }
  // deliver dequeued messages to the receiver
  Message *dequeued_msg = nullptr;
  while (true) {
    dequeued_msg = user->mqueue.dequeue();
    if (dequeued_msg != nullptr) {
      if (conn->send(*dequeued_msg)) {
        delete dequeued_msg;
      } else {
        delete dequeued_msg;
        break;
      }
    }
  }
  joined_room->remove_member(user);
  return;
}

void chat_with_sender(Connection *conn, Server *server, User *user) {
  Room *curr_room = nullptr;
  while (true) {
    Message msg;
    bool received_message = conn->receive(msg);
    std::string msg_tag;
    std::string msg_data;
    // handle failure to receive message
    if (!received_message) {
      Connection::Result receive_result = conn->get_last_result();
      if (receive_result == Connection::EOF_OR_ERROR || receive_result == Connection::INVALID_MSG) {
        conn->send(Message(TAG_ERR, "received invalid message"));
        return;
      } 
      conn->send(Message(TAG_ERR, "unable to receive message"));
    } else { // successfully receieved a message
      msg_tag = msg.tag;
      msg_data = msg.data;
      uint msg_data_length = msg_data.length();
      // message receieve error too long
      if (msg_data_length >= Message::MAX_LEN) {
        conn->send(Message(TAG_ERR, "message is too long"));
      }
      // handle error tag
      if (msg_tag == TAG_ERR) {
        std::cerr << msg_data << std::endl;
        return;
      }
      // handle quit request
      if (msg_tag == TAG_QUIT) {
        conn->send(Message(TAG_OK, "bye"));
        return;
      } else if (curr_room == nullptr) {
        if (msg_tag != TAG_JOIN) {
          conn->send(Message(TAG_ERR, "not in a room"));
        } else {
          curr_room = server->find_or_create_room(msg_data);
          curr_room->add_member(user); // "login"
          // handle failure to join room
          if (!conn->send(Message(TAG_OK, "joined room " + curr_room->get_room_name()))) {
            return;
          }
        }
      } else if (msg_tag == TAG_LEAVE) { // handle leave request
        if (curr_room == nullptr) {
          conn->send(Message(TAG_ERR, "not in a room"));
        } else {
          curr_room->remove_member(user);
          curr_room = nullptr; // reset room
          // handle failure to send confirmation message
          if (!conn->send(Message(TAG_OK, "left room"))) {
            return;
          }
        }
      } else if (msg_tag == TAG_JOIN) { // handle join request
        // user must leave if already in a room
        curr_room->remove_member(user);
        curr_room = server->find_or_create_room(msg_data);
        curr_room->add_member(user);
        // handle failure to join room
        if (!conn->send(Message(TAG_OK, "joined room " + curr_room->get_room_name()))) {
          return;
        }
      } else if (msg_tag == TAG_SENDALL) { // handle sendall request
        curr_room->broadcast_message(user->username, msg_data);
        // handle failure to send message to everyone
        if (!conn->send(Message(TAG_OK, "message sent"))) {
          return;
        }
      } else { // handle case in which the tag received is undefined
        if (!conn->send(Message(TAG_ERR, "received invalid tag"))) {
          return;
        }
      }
    }
  }
  delete user;
  delete conn;
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // initialize mutex
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  // destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // use open_listenfd to create the server socket, return true
  // if successful, false if not
  char const* port = (std::to_string(this->m_port)).c_str();
  this->m_ssock = open_listenfd(port);
  if (m_ssock >= 0) {
    return true;
  }
  std::cerr << "Error: unable to open server socket" << std::endl;
  return false;
}

void Server::handle_client_requests() {
  // infinite loop calling accept or Accept, starting a new
  // pthread for each connected client
  while (true) {
    int clientfd = accept(m_ssock, NULL, NULL);
    if (clientfd < 0) {
      std::cerr << "Error: unable to accept server" << std::endl;
      return;
    } else {
      struct ConnInfo *connInfo = new ConnInfo(); 
      connInfo->conn = new Connection(clientfd); 
      connInfo->server = this;
      pthread_t thread_id;
      int pthread_result = pthread_create(&thread_id, NULL, worker, connInfo);
      if (pthread_result != 0) {
        delete connInfo; 
        std::cerr << "Error: unable to create a new thread." << std::endl;
        return;
      }
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // return a pointer to the unique Room object representing
  // the named chat room, creating a new one if necessary
  if (m_rooms.count(room_name) <= 0) {
    Room * new_room = new Room(room_name);
    Guard g(m_lock);
    m_rooms[room_name] = new_room;
  }
  return m_rooms[room_name];
}