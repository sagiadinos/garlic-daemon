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

void Daemon::run()
{
    while (is_running.load())
    {
        if (is_reload.load())
        {
            instance().is_reload.store(false);
            LOG_INFO("Reload called.");
        }

        MyWatchdog->run();
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void Daemon::setWatchdog(Watchdog *wd)
{
    MyWatchdog = wd;
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
