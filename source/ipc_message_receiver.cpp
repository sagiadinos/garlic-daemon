#include "ipc_message_receiver.hpp"

IPCMessageReceiver::IPCMessageReceiver(const std::string &queueName)
{
    mq = mq_open(queueName.c_str(), O_RDONLY | O_CREAT, 0644, nullptr);
    if (mq == (mqd_t)-1)
    {
        throw std::runtime_error("Unable to open message queue");
    }
}

IPCMessageReceiver::~IPCMessageReceiver()
{
    stop();
    mq_close(mq);
}

void IPCMessageReceiver::receiveMessages()
{
    while (running.load())
    {
        char buffer[8192];
        ssize_t bytes_read;

        bytes_read = mq_receive(mq, buffer, sizeof(buffer), nullptr);
        if (bytes_read >= 0)
        {
            std::string message(buffer, bytes_read);
            this->push(message);
        }
        else if (errno != EAGAIN)
        {
            std::cerr << "Failed to receive message: " << strerror(errno) << std::endl;
        }
    }
}

void IPCMessageReceiver::start()
{
    running.store(true);
    receiverThread = std::thread(&IPCMessageReceiver::receiveMessages, this);
    receiverThread.detach();
}

void IPCMessageReceiver::stop()
{
    running.store(false);
}

void IPCMessageReceiver::push(const std::string &message)
{
    std::unique_lock<std::mutex> lock(mtx);
    queue.push(message);
    cv.notify_one();
}

bool IPCMessageReceiver::pollingQueue(std::string& message)
{
    std::unique_lock<std::mutex> lock(mtx);
    if (queue.empty())
        return false;

    message = queue.front();
    queue.pop();
    return true;
}

void IPCMessageReceiver::waitAndPop(std::string& message)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]{ return !queue.empty(); });
    message = queue.front();
    queue.pop();
}
