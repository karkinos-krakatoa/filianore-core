#pragma once


#include "array_base.h"


namespace konsortium
{

    class Array_SSE_I : public ArrayBase
    {
    public:
        Array_SSE_I() { data = _mm_set1_epi32(0); }
        KONSORTIUM_INLINE explicit Array_SSE_I(int x, int y = 0, int z = 0, int w = 0)
        {
            data = _mm_setr_epi32(w, z, y, x);
        }  

        union
        {
            int m[4];
            __m128i data;
        };      
    };  


    KONSORTIUM_INLINE Array_SSE_I operator+(Array_SSE_I& a, Array_SSE_I& b)
    {
        a.data = _mm_add_epi32(a.data, b.data);
        return a;
    }  

}