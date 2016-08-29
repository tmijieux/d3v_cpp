#ifndef D3V_UTIL_H
#define D3V_UTIL_H

#include <stdlib.h>
#include <glib.h>

#ifndef __GNUC__
#define noexcept
#endif

namespace d3v {

class exception : public std::exception {
private:
    std::string _s;
public:
    exception(std::string const &s): _s(s) {}
    char const *what() const noexcept { return _s.c_str(); }
};
int readline(GIOChannel *chan, std::string &ret);

}; // end namespace d3v;

#endif // D3V_UTIL_H
