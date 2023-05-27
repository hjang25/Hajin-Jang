/*
 * C++ implementation of sender.cpp
 * Jiwon Moon, Hajin Jang
 */

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

std::string getRoomName(std::string user_command);

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    exit(1);
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // connect to server
  Connection connection; // declare connection object
  connection.connect(server_hostname, server_port);

  if (!connection.is_open()) { // check if connection was successful
    std::cerr << "Error: connection to server failed\n";
    exit(1);
  }

  // send slogin message
  connection.send(Message(TAG_SLOGIN, username));
  Message server_response = Message(); // create empty Message object
  connection.receive(server_response);

  if (server_response.tag == TAG_ERR) { // handle error response
    std::cerr << server_response.data; // print error payload
    connection.close();
    exit(1);
  }

  // loop reading commands from user, sending messages to
  // server as appropriate
  while(true) {
    Message msg = Message();
    std::string user_command;
    std::getline(std::cin, user_command);

    // process user commands based on tag
    if(user_command == "/leave") {
      msg.tag = TAG_LEAVE;
      msg.data = "bye";
      connection.send(msg);
    } else if(user_command.substr(0, 5) == "/join") {
      msg.tag = TAG_JOIN;
      msg.data = getRoomName(user_command);
      connection.send(msg);
    } else if(user_command == "/quit") {
      msg.tag = TAG_QUIT;
      msg.data = "bye";
      Message response = Message();
      connection.send(msg);
      connection.receive(response);
      // handle error response
      if (response.tag != TAG_OK) {
        std::cerr << response.data; // print exactly the error payload
      }
      break;
    } else {
      msg.tag = TAG_SENDALL;
      msg.data = user_command;
      //connection.send(msg);
    }
    connection.send(msg);
    Message msg_response = Message();
    connection.receive(msg_response);
    if (msg_response.tag != TAG_OK) {
      std::cerr << msg_response.data; // print exactly the error payload
      continue;
    }
  }
  connection.close(); // close connection before exiting function
  return 0;
}

// helper function that returns the room name for the join command
std::string getRoomName(std::string user_command) {
  // chars 0-5 is "/join ", so everything from 6-end specifies room name
  return user_command.substr(6);
}
