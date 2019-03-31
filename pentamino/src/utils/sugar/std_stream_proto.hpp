#pragma once

#include <iosfwd>

#define REDEMPTION_OSTREAM(out_name, param) \
    template<class Ch, class Tr>            \
    std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& out_name, param)

#define REDEMPTION_ISTREAM(in_name, param) \
    template<class Ch, class Tr>           \
    std::basic_istream<Ch, Tr>& operator>>(std::basic_istream<Ch, Tr>& in_name, param)


#define REDEMPTION_FRIEND_OSTREAM(out_name, param) \
    template<class Ch, class Tr>                   \
    friend std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& out_name, param)

#define REDEMPTION_FRIEND_ISTREAM(in_name, param) \
    template<class Ch, class Tr>                  \
    friend std::basic_istream<Ch, Tr>& operator>>(std::basic_istream<Ch, Tr>& in_name, param)
