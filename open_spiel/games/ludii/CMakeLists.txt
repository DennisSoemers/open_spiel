set (JDK_HOME /usr/lib/jvm/java-8-openjdk-amd64)

add_library (ludii OBJECT
  jni_utils.h
  jni_utils.cc
)
target_include_directories (ludii PUBLIC ${JDK_HOME}/include/linux ${JDK_HOME}/include)
target_link_directories (ludii PUBLIC ${JDK_HOME}/jre/lib/amd64/server)
target_link_libraries (ludii jvm)
