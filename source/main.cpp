
// started with https://github.com/ftraple/cpp-daemon-template

#include <chrono>
#include <iostream>
#include <libconfig.h++>
#include <thread>
#include <cstdio>
#include <cstddef>

#include "command-line-parser.hpp"
#include "dbus_server.hpp"

#include "daemon.hpp"
#include "log.hpp"

// This function will be called when the daemon receive a SIGHUP signal.
void reload()
{
    LOG_INFO("Reload function called.");
}

int main(int argc, char** argv)
{
    // Command line and config file example
    CommandLineParser commandLine(argc, argv);
    if (commandLine.cmdOptionExist("--config"))
    {
        const std::string configFileName = commandLine.getCmdOptionValue("--config");
        LOG_INFO("Config file name = ", configFileName);
        libconfig::Config config;
        config.readFile(configFileName.c_str());
        std::string test{};
        config.lookupValue("test", test);
        LOG_INFO("Config option test = ", test);
    }

    // The Daemon class is a singleton to avoid be instantiate more than once
    Daemon& daemon = Daemon::instance();

    DBusServer MyDBusServer;
    std::cout << "Connected to D-Bus as \"" << MyDBusServer.getUniqueName() << "\"." << std::endl;

    MyDBusServer.addRuleForListening();

    daemon.setReloadFunction(reload);

    // Daemon main loop
    int count = 0;
    while (daemon.IsRunning())
    {
        LOG_DEBUG("Count: ", count++);
        if (MyDBusServer.isReceiving())
        {
            fprintf(stderr, "Huuraaaasah!");
            MyDBusServer.checkForSignal();
         //   MyDBusServer.checkForMethod();
        }

        // Check here if garlic-player runs

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    LOG_INFO("The daemon process ended gracefully.");
}
