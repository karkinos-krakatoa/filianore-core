#pragma once


#include <vector>
#include "array_sse_f.h"
#include "array_sse_i.h"


namespace konsortium
{

    template <typename T, size_t N> class Array
    {
    public:
        Array(T x, T y, T z) 
        { 
            array_group.emplace_back(new Array_SSE_I(x, y, z));
        }

        Array(const ArrayBase& a) : array_result(a) { }

        T data[N];  

 
        ArrayBase array_result; 
        std::vector<ArrayBase*> array_group;
    };  


    template <typename T, size_t N>
    KONSORTIUM_INLINE Array<T, N> operator+(Array<T, N>& a, Array<T, N>& b)
    {
        ArrayBase* result = a.array_group[0] + b.array_group[0];
        return Array<T, N>(&result);
    }  

}