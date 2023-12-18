/*************************************************************************************
    garlic-daemon: Linux daemon for garlic-player
    Copyright (C) 2023 Nikolaos Sagiadinos <ns@smil-control.com>
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

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <libconfig.h++>

class Configuration
{
    public:
        Configuration();
        void readLibConfigValues(libconfig::Config  *config);

        std::string getAppName() const;
        std::string getPlayerName() const;
        std::string getPlayerPath() const;
        std::string getPlayerUser() const;

    private:
        const std::string APP_NAME    = "garlic-daemon";
        const std::string PLAYER_NAME = "garlic-player";
        const std::string PLAYER_PATH = "/usr/local/bin/";
        const std::string PLAYER_USER = "digitalsignage";

        std::string app_name    = APP_NAME;
        std::string player_name = PLAYER_NAME;
        std::string player_path = PLAYER_PATH;
        std::string player_user = PLAYER_USER;
};

#endif // CONFIGURATION_HPP
