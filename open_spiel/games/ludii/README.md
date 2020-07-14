# Ludii Wrapper

This is an experimental work-in-progress C++ wrapper of the
[Ludii General Game System](https://ludii.games/). The Ludii library is written
in Java so this wrapper uses
[JNI](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/) to interact
with the Ludii jar through C++.

For discussion on the development of this wrapper, please see
[issue #39](https://github.com/deepmind/open_spiel/issues/39).

## How to build

Tested on Ubuntu 16.04 with Java 8 openJDK and Ludii player (0.3.0).

1.  Install openjdk if you haven't already.

2.  Check `games/ludii/CMakeLists`. Assuming Java 8 openJDK is installed the
    JDK_HOME is set to `/usr/lib/jvm/java-8-openjdk-amd64`. This might have to
    be changed if a different version is installed.

3.  See `open_spiel/scripts/global_variables.sh` for enabling Ludii as an
	optional dependency.

4.  Build OpenSpiel as usual. The `open_spiel/scripts/install.sh` script will
	automatically download the latest Ludii.jar file and place it in the
	`open_spiel/games/ludii` directory if the optional Ludii dependency is enabled. 
	Alternatively, you may also manually download Ludii's JAR file and place it in that 
	directory.

If `libjvm.so` is not found, run:

`export LD_LIBRARY_PATH=/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/`
