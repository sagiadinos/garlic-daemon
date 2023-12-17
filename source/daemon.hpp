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
