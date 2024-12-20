
#include "../includes/utils.hpp"

std::string erasesSidesChar(std::string str, char c) {
    std::string res = str;
    if (!res.empty() && res[0] == c)
        res.erase(0, 1);
    if (!res.empty() && res[res.size() - 1] == c)
        res.erase(res.size() - 1, 1);
    return (res);
}

std::string size_t_to_string(size_t value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string removeConsecutivesSlash(std::string str) {
    size_t i = 0;
    while (i + 1 < str.size()) {
        if (str[i] == '/' && str[i + 1] == '/') {
            str.erase(i, 1);
        } else {
            i++;
        }
    }
    return str;
}
