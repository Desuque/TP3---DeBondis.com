
#ifndef SERVER_THREAD_H_
#define SERVER_THREAD_H_

#include <thread>

class server_Thread {
	private:
		std::thread thread;
    public:
        server_Thread();
        void start();
        void join();
        virtual void run() = 0;
        virtual ~server_Thread();

    private:
        server_Thread(const server_Thread&) = delete;
        server_Thread& operator=(const server_Thread&) = delete;
};

#endif /* SERVER_THREAD_H_ */
