#pragma once

#include <stdio.h>
#include "base/define.h"

namespace ylib
{
    namespace mem
    {
        const static uint32 __big_size = 1024 * 1024 * 10;
        void* malloc(size_t size);
        void  free(void* src);
        void* realloc(void* src, size_t length);
    }
    
}
