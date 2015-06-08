#-----------------------------------------------
#  Source code compilation instructions
#-----------------------------------------------

include_directories(include)

FILE(GLOB PROJECT_SOURCE_FILES source/tlk_v1/*.cpp source/tra/*.cpp)

add_library(cppbg STATIC ${PROJECT_SOURCE_FILES})
set_target_properties(cppbg PROPERTIES DEBUG_POSTFIX "d")
