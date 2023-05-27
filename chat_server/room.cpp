/*
 * C++ implementation of room.cpp
 * Jiwon Moon, Hajin Jang
 */

#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // initialize the mutex
  pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
  // destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // add User to the room
  Guard g(lock);
  members.insert(user);
}

void Room::remove_member(User *user) {
  // remove User from the room
  Guard g(lock);
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // send a message to every (receiver) User in the room
  Guard g(lock);
  for (auto user : members) {
    if (user->username != sender_username) {
      std::string msg_data = this->get_room_name() + ":" + sender_username + ":" + message_text;
      user->mqueue.enqueue(new Message(TAG_DELIVERY, msg_data));
    }
  }
}