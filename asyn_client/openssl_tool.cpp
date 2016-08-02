//
// Created by Shao.Admin on 16/7/29.
//

#include <openssl/md5.h>
#include "openssl/evp.h"

#include "openssl_tool.h"
#include "assert.h"
//
//#include <openssl/bio.h>
//#include <openssl/evp.h>
//#include <stdint.h>
#include <openssl/buffer.h>


std::string md5(const std::string &data) {

    MD5_CTX ctx;
    unsigned char md[16] = {0};
    char tmp[3]={'\0'}, buf[33]={'\0'};
    MD5((const unsigned char *)data.c_str(), data.size(), md);

    for (int i = 0; i < 16; i++) {
        sprintf(tmp, "%2.2x", md[i]);
        strcat(buf,tmp);
    }

    return std::string(buf);
}

size_t calc_decode_length(const char* b64input) { //Calculates the length of a decoded string
    size_t len = strlen(b64input),
            padding = 0;

    if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len-1] == '=') //last char is =
            padding = 1;

    return (len*3)/4 - padding;
}

std::string base64_encode(const unsigned char* buffer, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
    BIO_write(bio, buffer, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    //    b64_test = bufferPtr->data;

    std::string copy(bufferPtr->data, bufferPtr->length);

    BIO_free_all(bio);

    return copy;
}

int base64_decode(char* b64message, unsigned char** buffer, size_t* length) { //Decodes a base64 encoded string
    BIO *bio, *b64;

    int decodeLen = calc_decode_length(b64message);
    *buffer = (unsigned char*)malloc(decodeLen + 1);
    (*buffer)[decodeLen] = '\0';

    bio = BIO_new_mem_buf(b64message, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
    *length = BIO_read(bio, *buffer, strlen(b64message));
    assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
    BIO_free_all(bio);

    return decodeLen;
}

