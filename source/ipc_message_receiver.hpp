#ifndef IPCMESSAGERECEIVER_HPP
#define IPCMESSAGERECEIVER_HPP

#include <mqueue.h>
#include <atomic>
#include <thread>
#include <iostream>
#include <cstring>
#include <queue>
#include <mutex>
#include <condition_variable>

class IPCMessageReceiver
{
    public:
        IPCMessageReceiver(const std::string& queueName);
        ~IPCMessageReceiver();
        void receiveMessages();
        void start();
        void stop();
        void push(const std::string& message);
        bool pollingQueue(std::string& message);
        void waitAndPop(std::string& message);

    private:
        mqd_t mq;
        std::atomic<bool> running;
        std::thread receiverThread;
        std::queue<std::string> queue;
        std::mutex mtx;
        std::condition_variable cv;
};


#endif // IPCMESSAGERECEIVER_HPP
