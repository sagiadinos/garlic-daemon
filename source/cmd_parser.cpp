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
