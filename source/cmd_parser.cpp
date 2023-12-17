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

#include "cmd_parser.hpp"

CmdParser::CmdParser(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        this->m_tokens.push_back(std::string(argv[i]));
    }
}

void CmdParser::checkForConfigOption()
{
    if (!cmdOptionExist("--config"))
        return;

    const std::string configFileName = getCmdOptionValue("--config");
    LOG_INFO("Config file name = ", configFileName);
    libconfig::Config config;
    config.readFile(configFileName.c_str());
    std::string test{};
    config.lookupValue("test", test);
    LOG_INFO("Config option test = ", test);
}

const std::string &CmdParser::getCmdOptionValue(const std::string &option) const
{
    auto it = std::find(this->m_tokens.begin(), this->m_tokens.end(), option);
    if (it != this->m_tokens.end() && ++it != this->m_tokens.end())
    {
        return *it;
    }
    static const std::string emptyString("");
    return emptyString;
}

bool CmdParser::cmdOptionExist(const std::string &option) const
{
    auto it = std::find(this->m_tokens.begin(), this->m_tokens.end(), option);
    return (it != this->m_tokens.end());
}
