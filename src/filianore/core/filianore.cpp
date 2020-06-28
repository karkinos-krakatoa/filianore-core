#include "filianore.h"


namespace filianore
{

    const char* Filianore::get_lib_name()
    {
        #if defined _WIN32
        return "filianore.dll";
        #elif defined __APPLE__
        return "libfilianore.dylib";
        #else
        return "filianore.so";
        #endif 
    }

}