find_package(JNI)

add_library (ludii OBJECT
  jni_utils.h
  jni_utils.cc
)
target_include_directories (ludii PUBLIC ${JNI_INCLUDE_DIRS})
target_link_directories (ludii PUBLIC ${JNI_LIBRARIES})
target_link_libraries (ludii jvm)
