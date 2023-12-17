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
