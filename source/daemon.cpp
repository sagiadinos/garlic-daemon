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

#include "daemon.hpp"

Daemon::Daemon()
{
    signal(SIGINT, Daemon::signalHandler);
    signal(SIGTERM, Daemon::signalHandler);
    signal(SIGHUP, Daemon::signalHandler);
}

Daemon &Daemon::instance()
{
    static Daemon instance;
    return instance;
}

void Daemon::startWatchdog(Watchdog *MyWatchdog)
{
    while (is_running.load())
    {
        if (is_reload.load())
        {
            instance().is_reload.store(false);
            LOG_INFO("Reload called.");
        }

        MyWatchdog->check();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void Daemon::startMessageListener(IPCMessageReceiver *MyIPCMessageReceiver)
{
    while (is_running.load())
    {
        std:string message;
        MyIPCMessageReceiver->waitForMessage(message);
        std::cout << "Received message from queue: " << message << std::endl;
    }
}


void Daemon::signalHandler(int signal)
{
    LOG_INFO("Interrup signal number [", signal, "] recived.");
    switch (signal)
    {
        case SIGINT:
        case SIGTERM:
        case SIGKILL:
            instance().is_running.store(false);
            break;
        case SIGHUP:
            instance().is_reload.store(true);
            break;
        }
}
