/*
 * C++ implementation of receiver.cpp
 * Jiwon Moon, Hajin Jang
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string user_name = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // connect to server
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) { // error handling for unsuccessful connection
    std::cerr << "unable to connect to server" << std::endl;
    return 1;
  }

  // send rlogin messages and expect a response from
  // the server for each one
  Message rlogin_msg = Message(TAG_RLOGIN, user_name);
  conn.send(rlogin_msg);
  Message rlogin_response = Message();
  conn.receive(rlogin_response);
  if (rlogin_response.tag == TAG_ERR) { // error handling for tag error
    std::cerr << rlogin_response.data;
    return 1;
  }

  // send join messages and expect a response from
  // the server for each one
  Message join_msg = Message(TAG_JOIN, room_name);
  conn.send(join_msg);
  Message join_response = Message();
  conn.receive(join_response);
  if (join_response.tag == TAG_ERR) { // error handling for tag error
    std::cerr << join_response.data;
    return 1;
  }
  
  // loop waiting for messages from server
  // (which should be tagged with TAG_DELIVERY)
  bool is_active = true;
  while (is_active) { // continue looping until the user disconnects
    Message server_message; 
    conn.receive(server_message);
    std::string mes_tag = server_message.tag;
    std::string mes_data = server_message.data;
    if(mes_tag == TAG_DELIVERY){
      // declare local variables for tokenization
      std::string room;
      std::string sender;
      std::string message_text;

      std::vector<std::string> tokens;
      std::string token;
      std::istringstream token_stream(mes_data);

      // tokenize the payload into room, user, and message_text
      while (getline(token_stream, token, ':')) {
        tokens.push_back(token);
      }
      if (tokens.size() != 0 && tokens.size() != 3) {
        std::cerr << "Received message with invalid format\n";
        return 1;
      }
      if (tokens.size() >= 3) { // means we successfully extracted desired tokens
        room = tokens[0];
        sender = tokens[1];
        message_text = trim(tokens[2]); // rio does not remove newline
      }
      std::cout << sender << ": " << message_text << std::endl; // print formatted string
    } else if (mes_tag == TAG_ERR){
      std::cerr << mes_data; 
    }
  }

  conn.close();
  return 0;
}