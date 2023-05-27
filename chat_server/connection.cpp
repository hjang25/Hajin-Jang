/*
 * C++ implementation of connection.cpp
 * Jiwon Moon, Hajin Jang
 */

#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // call open_clientfd to connect to the server
  m_fd = open_clientfd(hostname.c_str(), std::to_string(port).c_str());
  // call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

Connection::~Connection() {
  // close the socket if it is open
  if (is_open()) {
    close();
  }
}

bool Connection::is_open() const {
  // return true if the connection is open
  if (m_fd >= 0) {
    return true;
  }
  return false;
}

void Connection::close() {
  // close the connection if it is open
  if (is_open()) {
    ::close(m_fd);
    m_fd = -1;
  }
}

bool Connection::send(const Message &msg) {
  // convert the message to a string to have the format "tag:data\n"
  const std::string str_msg = msg.tag + ":" + msg.data + "\n";
  // convert the string to a C-style string for use with rio_writen
  const char* char_msg = str_msg.c_str();
  size_t msg_length = str_msg.length();

  // check if the message size is valid
  if(msg_length > msg.MAX_LEN) {
    m_last_result = INVALID_MSG;
    return false;
  }

  // send the message using rio_writen and store the response
  ssize_t response = rio_writen(m_fd, char_msg, msg_length);

  // check if an error occurred while sending the message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  if (response < 0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  } else {
    m_last_result = SUCCESS;
    return true;
  }
}

bool Connection::receive(Message &msg) {
  // create buffer to store result
  char usrbuf[Message::MAX_LEN + 1];

  // handle rio_readlineb success
  if (rio_readlineb(&m_fdbuf, usrbuf, Message::MAX_LEN) >= 0) {
    // read message tag and data from the buffer array
    std::stringstream str_stream(usrbuf);

    getline(str_stream, msg.tag, ':');
    getline(str_stream, msg.data);

    m_last_result = SUCCESS;
    return true;
  }

  // handle rio_readlineb failure
  m_last_result = INVALID_MSG;
  return false;
}