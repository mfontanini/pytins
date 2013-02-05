#include <sstream>
#include <iomanip>
#include "utils.h"

namespace PyUtils {
    std::string string_repr(const std::string &str) {
        std::ostringstream oss;
        oss << '\'';
        for(std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
            if(*it < 20 || *it > 126) {
                if(*it == '\n')
                    oss << "\\n";
                else if(*it == '\b')
                    oss << "\\n";
                else
                    oss << "\\x" << std::setw(2) << std::setfill('0') << std::hex << (static_cast<int>(*it) & 0xff) << std::dec;
            }
            else
                oss << *it;
        }
        oss << '\'';
        return oss.str();
    }
}
