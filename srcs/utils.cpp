
#include "../includes/utils.hpp"

std::string erasesSidesChar(std::string str, char c) {
    std::string res = str;
    if (!res.empty() && res[0] == c)
        res.erase(0, 1);
    if (!res.empty() && res[res.size() - 1] == c)
        res.erase(res.size() - 1, 1);
    return (res);
}
