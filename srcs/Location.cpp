#include "../includes/Location.hpp"

Location::Location() {}

Location::~Location() {}

Location& Location::operator=(const Location &rhs)
{
    (void)rhs;
    return(*this);
}

Location::Location(const Location &src)
{
    (void)src;
}

