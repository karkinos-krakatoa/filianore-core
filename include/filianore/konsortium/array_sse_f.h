#pragma once


#include <array_base.h>


namespace konsortium
{

    class Array_SSE_F : public ArrayBase<float, __m128, 4>
    {
    public:
        Array_SSE_F() { data = _mm_set1_ps(0.f); }
        KONSORTIUM_INLINE explicit Array_SSE_F(float x, float y = 0.f, float z = 0.f, float w = 0.f)
        {
            data = _mm_set_ps(w, z, y, x);
        }      
    };  


    KONSORTIUM_INLINE Array_SSE_F operator+(Array_SSE_F& a, Array_SSE_F& b)
    {
        a.data = _mm_add_ps(a.data, b.data);
        return a;
    }  

}