#-----------------------------------------------------------------------------------
# Source Files
#-----------------------------------------------------------------------------------

set (filianore_sources 
    core/elemental.h 
    core/elemental.cpp 
)

set (filianore_core_sources   
    core/sampler.h 
    core/sampler.cpp 
    core/sampling.h 
    core/shape.h 
)

list (APPEND filianore_sources
    ${filianore_core_sources}
)
source_group ("filianore\\core" FILES
    ${filianore_core_sources}
)


set (filianore_math_sources
    math/aabb.h 
    math/matrix.h 
    math/ray.h 
    math/scalar.h 
    math/static_array.h 
    math/transform.h 
    math/vec3_math.h 
)

list (APPEND filianore_sources
    ${filianore_math_sources}
)
source_group ("filianore\\math" FILES
    ${filianore_math_sources}
)


set (filianore_samplers_sources
    samplers/global.h 
    samplers/global.cpp 
    samplers/pixel.h 
    samplers/pixel.cpp 
    samplers/stratified.h 
    samplers/stratified.cpp 
)

list (APPEND filianore_sources
    ${filianore_samplers_sources}
)
source_group ("filianore\\samplers" FILES
    ${filianore_samplers_sources}
)


set (filianore_shapes_sources
    shapes/triangle.h 
    shapes/triangle.cpp 
)

list (APPEND filianore_sources
    ${filianore_shapes_sources}
)
source_group ("filianore\\shapes" FILES
    ${filianore_shapes_sources}
)


#-----------------------------------------------------------------------------------
# Main Build
#-----------------------------------------------------------------------------------

add_library(filianore SHARED 
    ${filianore_sources})
