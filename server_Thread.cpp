#include "server_Thread.h"

server_Thread::server_Thread() {
}

void server_Thread::start() {
	thread = std::thread(&server_Thread::run, this);
}

void server_Thread::join() {
	thread.join();
}

server_Thread::~server_Thread() {
}

