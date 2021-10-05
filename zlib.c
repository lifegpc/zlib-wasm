#include "emscripten.h"
#include "zlib.h"
#include <malloc.h>
#include <string.h>

EMSCRIPTEN_KEEPALIVE
const char * version() {
    return zlibVersion();
}

EMSCRIPTEN_KEEPALIVE
int zlib_compress(unsigned char** de, unsigned long* de_len, unsigned char* source, unsigned long len) {
    if (source == NULL) {
        return 1;
    }
    if (len == 0) {
        len = strlen((char*)source);
    }
    unsigned char* dest = malloc(len);
    if (dest == NULL) {
        return 1;
    }
    unsigned long dest_len = len;
    int re = compress(dest, &dest_len, source, len);
    while (re != Z_OK) {
        if (re != Z_BUF_ERROR) {
            free(dest);
            return 1;
        }
        dest_len += len;
        unsigned char* pre = dest;
        dest = realloc(dest, dest_len);
        if (dest == NULL) {
            free(pre);
            return 1;
        }
        re = compress(dest, &dest_len, source, len);
    }
    *de = dest;
    *de_len = dest_len;
    return 0;
}

EMSCRIPTEN_KEEPALIVE
int zlib_uncompress(unsigned char** de, unsigned long* de_len, unsigned char* source, unsigned long len) {
    if (source == NULL) {
        return 1;
    }
    if (len == 0) {
        len = strlen((char*)source);
    }
    unsigned char* dest = malloc(len);
    if (dest == NULL) {
        return 1;
    }
    unsigned long dest_len = len;
    int re = uncompress(dest, &dest_len, source, len);
    while (re != Z_OK) {
        if (re != Z_BUF_ERROR) {
            free(dest);
            return 1;
        }
        dest_len += len;
        unsigned char* pre = dest;
        dest = realloc(dest, dest_len);
        if (dest == NULL) {
            free(pre);
            return 1;
        }
        re = uncompress(dest, &dest_len, source, len);
    }
    *de = dest;
    *de_len = dest_len;
    return 0;
}
