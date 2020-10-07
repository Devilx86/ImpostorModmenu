cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_SYSTEM_PROCESSOR arm)

set(sources_files)
list(APPEND sources_files src/main/cpp/main.cpp)

set(include_dir)

if ((${ANDROID_ABI} STREQUAL "arm64-v8a"))
    list(APPEND sources_files src/main/cpp/And64InlineHook/And64InlineHook.cpp)
    list(APPEND include_dir src/main/cpp/And64InlineHook)

elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    list(APPEND sources_files src/main/cpp/Substrate/SubstrateHook.cpp)
    list(APPEND sources_files src/main/cpp/Substrate/SubstrateDebug.cpp)
    list(APPEND sources_files src/main/cpp/Substrate/SubstratePosixMemory.cpp)

    list(APPEND include_dir src/main/cpp/Substrate)
endif()


add_library( devilhook SHARED ${sources_files})
include_directories(${include_files})

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       devilhook
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )