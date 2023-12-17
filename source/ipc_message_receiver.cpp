/*************************************************************************************
    garlic-daemon: Linux daemon for garlic-player
    Copyright (C) 2023 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-daemon source code

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/

#include "ipc_message_receiver.hpp"

/**
 * @brief IPCMessageReceiver::IPCMessageReceiver
 *
 * Opens a message queue with given name and throws an exception
 * when it is not possible
 *
 * @param queueName
 */
IPCMessageReceiver::IPCMessageReceiver(const std::string &queueName)
{
    mq = mq_open(queueName.c_str(), O_RDONLY | O_CREAT, 0644, nullptr);
    if (mq == (mqd_t)-1)
    {
        throw std::runtime_error("Unable to open message queue");
    }
}

/**
 * @brief IPCMessageReceiver::~IPCMessageReceiver
 *
 * Stops the detached thread and close the message queu.
 */
IPCMessageReceiver::~IPCMessageReceiver()
{
    stop();
    mq_close(mq);
}

/**
 * @brief IPCMessageReceiver::receiveMessages
 *
 * Main waiting loop for receiving messages. Message will be pushed into
 * a std::queue<std::string> queue.
 */
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

/**
 * @brief IPCMessageReceiver::start
 *
 * Starts the receiveMessages thread an detached it to unblock.
 *
 */
void IPCMessageReceiver::start()
{
    running.store(true);
    receiverThread = std::thread(&IPCMessageReceiver::receiveMessages, this);
    receiverThread.detach();
}

/**
 * @brief IPCMessageReceiver::stop
 *
 * Stops the receiveMessages thread
 */
void IPCMessageReceiver::stop()
{
    running.store(false);
}

/**
 * @brief IPCMessageReceiver::pollingQueue
 *
 * Returns immediately after checking if there is a mesasge in queue.
 *
 * @param message
 * @return bool
 */
bool IPCMessageReceiver::pollingQueue(std::string& message)
{
    std::unique_lock<std::mutex> lock(mtx);
    if (queue.empty())
        return false;

    message = queue.front();
    queue.pop();
    return true;
}

/**
 * @brief IPCMessageReceiver::waitForMessage
 *
 * Waits until a message enters the queue and then returns back.
 *
 * @param message
 */
void IPCMessageReceiver::waitForMessage(std::string& message)
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]{ return !queue.empty(); });
    message = queue.front();
    queue.pop();
}

/**
 * @brief IPCMessageReceiver::push
 *
 * A threadsave push into std::queue<std::string> queue;
 *
 * @param message
 */
void IPCMessageReceiver::push(const std::string &message)
{
    std::unique_lock<std::mutex> lock(mtx);
    queue.push(message);
    cv.notify_one();
}
