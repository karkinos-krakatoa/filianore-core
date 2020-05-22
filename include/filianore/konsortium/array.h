#pragma once


#include <array_sse_f.h>
#include <array_sse_i.h>


namespace konsortium
{

    template <typename T, size_t N> class Array
    {
    public:
        Array() { }


        T data[N];  

    private:   
        ArrayBase* array_types;
    };  

}