#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <thread>


using namespace std;

class Watchdog
{
    public:
        Watchdog();
        bool isPlayerRunning();
        bool isReboot();
        void setActivity(bool is_active);
        void check();
        void startPlayer();
        void killZombieProcesses();
    private:
        string player_name    = "garlic-player";
        string path_to_player_executable = "/opt/garlic-player/garlic.sh";
        int getPlayerProcId();
        bool isInCmdLine(string dir_name);
        bool isZombie(int pid);
        bool is_active = true;
};

#endif // WATCHDOG_HPP
