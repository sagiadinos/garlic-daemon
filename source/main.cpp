
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
    // Command line and config file example
    CmdParser cmd(argc, argv);
    cmd.checkForConfigOption();

    Watchdog MyWatchdog;

    Daemon& MyDaemon = Daemon::instance(); // The Daemon class is a singleton to avoid be instantiate more than once
    MyDaemon.setWatchdog(&MyWatchdog);


    std::thread DaemonThread(&Daemon::run,  &Daemon::instance());
    DaemonThread.join();

    LOG_INFO("The daemon process ended gracefully.");
    return 0;
}
