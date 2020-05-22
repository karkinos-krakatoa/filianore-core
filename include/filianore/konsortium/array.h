#pragma once

#include <stdint.h>
#include <cmath>
#include <assert.h>
#include <immintrin.h>
#include <filianore/core/elemental.h>


namespace filianore
{

    class Array
    {
    public:
        Array() { data = _mm256_set1_ps(0.f); }
        FILIANORE_INLINE explicit Array(float x, float y, float z)
        {
            data = _mm256_set_ps(8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f);
        }

        union
        {
            float m[8];
            __m256 data;
        };        
    };  


    FILIANORE_INLINE Array operator+(Array& a, Array& b)
    {
        a.data = _mm256_add_ps(a.data, b.data);
        return a;
    }  

}