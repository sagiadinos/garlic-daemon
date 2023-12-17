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

#include "cmd_parser.hpp"

#include "daemon.hpp"
#include "log.hpp"
#include "watchdog.hpp"

int main(int argc, char** argv)
{
    try
    {
        // Command line and config file example
        CmdParser cmd(argc, argv);
        cmd.checkForConfigOption();

        Watchdog MyWatchdog;
        std::string queueName = "/garlic-daemon";
        IPCMessageReceiver MyIPCMessageReceiver(queueName);
        MyIPCMessageReceiver.start();

        Daemon& MyDaemon = Daemon::instance(); // The Daemon class is a singleton to avoid be instantiate more than once

        std::thread WatchdogThread(&Daemon::startWatchdog,  &Daemon::instance(), &MyWatchdog);
        std::thread MessageQueueThread(&Daemon::startMessageListener,  &Daemon::instance(), &MyIPCMessageReceiver);
        WatchdogThread.join();
        MessageQueueThread.join();

        LOG_INFO("The daemon process ended gracefully.");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }


    return 0;
}
