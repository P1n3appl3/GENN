/*
 * base64 code by René Nyffenegger
 */
#ifndef GENN_BASE64_H
#define GENN_BASE64_H

#include <string>

class base64 {
public:
    static std::string encode(unsigned char const *, unsigned int len);

    static std::string decode(std::string const &s);
};


#endif //GENN_BASE64_H
