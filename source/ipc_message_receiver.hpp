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

/**
 * @brief The IPCMessageReceiver class
 *
 * This threadsafe class polls for for unix messages in receiveMessages()
 * in a detached thread started with start().
 * A mmutex is used for thread safety.
 *
 * When a message is received it will be stored in std::queue<std::string> queue;
 * There are two ways to get these messages:
 *  1. Polling
 *      Calling non-blocking pollingQueue(std::string& message) will fill the parameter
 *      with the oldest message from queue and return a true. It will return
 *      false when queue is empty.
 *
 *  2. Waiting for a message
 *        calling waitForMessage(std::string& message) with an empty message string
 *        will block when the queue is empty until a message will be received.
 *        This is the more resource friendly and perfered way.
 *
 */
class IPCMessageReceiver
{
    public:
        IPCMessageReceiver(const std::string& queueName);
        ~IPCMessageReceiver();
        void receiveMessages();
        void start();
        void stop();
        bool pollingQueue(std::string& message);
        void waitForMessage(std::string& message);

    private:
        mqd_t mq;
        std::atomic<bool> running;
        std::thread receiverThread;
        std::queue<std::string> queue;
        std::mutex mtx;
        std::condition_variable cv;
        void push(const std::string& message);
};


#endif // IPCMESSAGERECEIVER_HPP
