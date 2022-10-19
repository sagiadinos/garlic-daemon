#include "watchdog.h"

Watchdog::Watchdog()
{

}

void Watchdog::setActivityStatus(bool active)
{
    is_active = active;
}

void Watchdog::setPathToPlayerExecutable(string path)
{
    path_to_player_executable = path;
}

void Watchdog::runPlayer()
{
    std::system(path_to_player_executable.c_str());
}

bool Watchdog::isPlayerRunning()
{
    // if watchdog is not active return always true
   if (!is_active)
       return true;

   return (getPlayerProcId() > -1);
}

bool Watchdog::isReboot()
{
    return is_reboot;
}

int Watchdog::getPlayerProcId()
{
    int pid = -1;

    DIR *dp = opendir("/proc");
    if (dp == NULL)
    {
        closedir(dp);
        return pid;
    }

    struct dirent *dirp;
    while (pid < 0 && (dirp = readdir(dp)))
    {
        int id = atoi(dirp->d_name); // Skip non-numeric entries
        if (id > 0 && isInCmdLine(dirp->d_name))
        {
           pid = id;
        }
    }
    closedir(dp);

    // ToDo: Check if it is neccessary to reboot if the player is an zombie and cannot restarted

    return pid;
}

bool Watchdog::isInCmdLine(string dir_name)
{
    // Read contents of virtual /proc/{pid}/cmdline file
    string cmdPath = string("/proc/") + dir_name + "/cmdline";
    ifstream cmdFile(cmdPath.c_str());
    string cmdLine;

    getline(cmdFile, cmdLine);
    if (!cmdLine.empty())
    {
        // Keep first cmdline item which contains the program path
        size_t pos = cmdLine.find('\0');
        if (pos != string::npos)
            cmdLine = cmdLine.substr(0, pos);

        // Keep program name only, removing the path
        pos = cmdLine.rfind('/');
        if (pos != string::npos)
            cmdLine = cmdLine.substr(pos + 1);

        return (player_name == cmdLine);
    }

    return false;
}

bool Watchdog::isZombie(int pid)
{
    bool is_zombie = false;

    char pbuf[32];
    snprintf(pbuf, sizeof(pbuf), "/proc/%d/stat", pid);

    FILE* fpstat = fopen(pbuf, "r");
    if (!fpstat) { perror(pbuf); exit(EXIT_FAILURE); };
    {
      int rpid =0; char rcmd[32]; char rstatc = 0;
      fscanf(fpstat, "%d %30s %c", &rpid, rcmd, &rstatc);
      is_zombie = rstatc == 'Z';
    }

    fclose(fpstat);

    return is_zombie;
}
