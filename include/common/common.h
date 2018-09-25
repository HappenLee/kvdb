//
// Created by happen on 9/15/18.
//

#ifndef LDB_COMMON_H
#define LDB_COMMON_H

#include <stdint-gcc.h>

#ifndef SAFE_FREE
#define SAFE_FREE(ptr) \
    do { \
        if (NULL != ptr) { \
            free(ptr); \
            ptr = NULL; \
        } \
    } while (0)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) \
    do { \
        if (NULL != ptr) { \
            delete ptr; \
            ptr = NULL; \
        } \
    } while (0)
#endif

#ifndef SAFE_DELETE_ARR
#define SAFE_DELETE_ARR(ptr) \
    do { \
        if (NULL != ptr) { \
            delete[] ptr; \
            ptr = NULL; \
        } \
    } while (0)
#endif

int64_t static murmurhash64 (const char* key, int len, unsigned int seed) {
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    unsigned int h1 = seed ^ len;
    unsigned int h2 = 0;
    const unsigned int * data = (const unsigned int *)key;

    while(len >= 8) {
        unsigned int k1 = *data++;
        k1 *= m; k1 ^= k1 >> r; k1 *= m;
        h1 *= m; h1 ^= k1;
        len -= 4;

        unsigned int k2 = *data++;
        k2 *= m; k2 ^= k2 >> r; k2 *= m;
        h2 *= m; h2 ^= k2;
        len -= 4;
    }

    if(len >= 4) {
        unsigned int k1 = *data++;
        k1 *= m; k1 ^= k1 >> r; k1 *= m;
        h1 *= m; h1 ^= k1;
        len -= 4;
    }

    switch (len) {
        case 3: h2 ^= ((unsigned char*)data)[2] << 16;
        case 2: h2 ^= ((unsigned char*)data)[1] << 8;
        case 1: h2 ^= ((unsigned char*)data)[0];
            h2 *= m;
    };

    h1 ^= h2 >> 18; h1 *= m;
    h2 ^= h1 >> 22; h2 *= m;
    h1 ^= h2 >> 17; h1 *= m;
    h2 ^= h1 >> 19; h2 *= m;

    uint64_t h = h1;
    h = (h << 32) | h2;
    return h;
}

#endif //LDB_COMMON_H
