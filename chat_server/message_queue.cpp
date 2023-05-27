/*
 * C++ implementation of message_queue.cpp
 * Jiwon Moon, Hajin Jang
 */

#include <cassert>
#include <ctime>
#include <semaphore.h>
#include "message_queue.h"
#include "guard.h"
#include "message.h"

MessageQueue::MessageQueue() {
  // initialize the mutex and the semaphore
	sem_init(&m_avail, 0, 0);
  pthread_mutex_init(&m_lock, nullptr);
}

MessageQueue::~MessageQueue() {
  // destroy the mutex and the semaphore
  sem_destroy(&m_avail);
  pthread_mutex_destroy(&m_lock);
}

void MessageQueue::enqueue(Message *msg) {
  // put the specified message on the queue
  Guard g(m_lock);
  m_messages.push_back(new Message(msg->tag, msg->data));

  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
  sem_post(&m_avail);
}

Message *MessageQueue::dequeue() {
  struct timespec ts;

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  Message *msg = nullptr;

  // call sem_timedwait to wait up to 1 second for a message
  // to be available, return nullptr if no message is available
  if (sem_timedwait(&m_avail, &ts) == -1) {
    return nullptr;
  }

  Guard g(m_lock);

  // remove the next message from the queue, return it
  if (!m_messages.empty()) {
    msg = m_messages.front();
    m_messages.pop_front();
  }

  return msg;
}