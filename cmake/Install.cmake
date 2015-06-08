#-----------------------------------------------
#  Installation instructions
#-----------------------------------------------

FILE(GLOB PROJECT_INCLUDE_FILES_TLKV1 include/cppbg/tlk_v1/*.h)
FILE(GLOB PROJECT_INCLUDE_FILES_TRA include/cppbg/tra/*.h)

install(TARGETS cppbg DESTINATION lib)
install(FILES ${PROJECT_INCLUDE_FILES_TLKV1} DESTINATION include/cppbg/tlk_v1)
install(FILES ${PROJECT_INCLUDE_FILES_TRA} DESTINATION include/cppbg/tra)
