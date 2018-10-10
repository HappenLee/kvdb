//
// Created by happen on 9/15/18.
//

#ifndef LDB_COMMON_H
#define LDB_COMMON_H

#include <stdint-gcc.h>

enum Status { Success, Fail };

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



#endif //LDB_COMMON_H
