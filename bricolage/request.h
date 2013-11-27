#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <string>

std::string request(std::string host, std::string url, std::string* session = 0);

#endif