#ifndef WATCHDOG_H
#define WATCHDOG_H

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

using namespace std;

class Watchdog
{
    public:
        Watchdog();
        bool isPlayerRunning();
        bool isReboot();
        void setPathToPlayerExecutable(string path);
        void setActivityStatus(bool newIs_active);
        void runPlayer();
        void killZombieProcesses();
    private:
        string player_name    = "garlic-player";
        string path_to_player_executable = "/opt/garlic-player/garlic.sh";
        int getPlayerProcId();
        bool isInCmdLine(string dir_name);
        bool isZombie(int pid);
        bool is_active = true;
        bool is_reboot = false;
};

#endif // WATCHDOG_H
