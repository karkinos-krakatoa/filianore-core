#include "filianore/core/memory.h"

namespace filianore
{
    void *AllocAligned(size_t size)
    {
#if defined(FILIANORE_HAVE__ALIGNED_MALLOC)
        return _aligned_malloc(size, FILIANORE_L1_CACHE_LINE_SIZE);
#elif defined(FILIANORE_HAVE_POSIX_MEMALIGN)
        void *ptr;
        if (posix_memalign(&ptr, FILIANORE_L1_CACHE_LINE_SIZE, size) != 0)
            ptr = nullptr;
        return ptr;
#else
        void *ptr;
        if (posix_memalign(&ptr, FILIANORE_L1_CACHE_LINE_SIZE, size) != 0)
            ptr = nullptr;
        return ptr;
#endif
    }

    void FreeAligned(void *ptr)
    {
        if (!ptr)
            return;
#if defined(FILIANORE_HAVE__ALIGNED_MALLOC)
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }
}
