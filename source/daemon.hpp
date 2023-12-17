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

#ifndef DAEMON_HPP
#define DAEMON_HPP

#include <csignal>
#include <atomic>

#include "log.hpp"
#include "watchdog.hpp"
#include "ipc_message_receiver.hpp"

class Daemon
{
    public:
        static Daemon& instance();

        void startWatchdog(Watchdog *MyWatchdog);
        void startMessageListener(IPCMessageReceiver *MyIPCMessageReceiver);
    private:
        std::atomic<bool> is_running{true};
        std::atomic<bool> is_reload{false};

        Daemon();
        Daemon(Daemon const&) = delete;
        void operator=(Daemon const&) = delete;
        void reload();

        static void signalHandler(int signal);
};

#endif  // DAEMON_HPP
