#ifndef CMD_PARSER_HPP
#define CMD_PARSER_HPP

#include <algorithm>
#include <functional>
#include <optional>
#include <string>
#include <vector>
#include <libconfig.h++>
#include "log.hpp"

class CmdParser
{
    public:
        CmdParser(int argc, char** argv);
        void checkForConfigOption();
    private:
        const std::string& getCmdOptionValue(const std::string& option) const;
        bool cmdOptionExist(const std::string& option) const;

        std::vector<std::string> m_tokens;
};

#endif  // COMMAND_LINE_PARSER_HPP
