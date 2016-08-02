//
// Created by Shao.Admin on 16/7/29.
//

#ifndef OPENSSL_TOOL_H_
#define OPENSSL_TOOL_H_

#include <string>

std::string md5(const std::string &data);

std::string base64_encode(const unsigned char* buffer, size_t length);
int base64_decode(char* b64message, unsigned char** buffer, size_t* length);


#endif //OPENSSL_TOOL_H_
