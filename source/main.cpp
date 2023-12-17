
// started with https://github.com/ftraple/cpp-daemon-template

//#include <chrono>
//#include <iostream>

//#include <cstdio>
//#include <cstddef>
//#include <unistd.h>
//#include <sys/reboot.h>

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
