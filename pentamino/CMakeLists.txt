cmake_minimum_required (VERSION 3.0)

project(pentamino)
set(TARGET_NAME pentamino)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

set(CMAKE_MACOSX_RPATH 1)

file(GLOB_RECURSE SRC_CORE_FILES "src/core/*.cpp" "src/core/*.hpp" "src/cxx/*.cpp" "src/cxx/*.hpp")
file(GLOB_RECURSE SRC_FILES "src/main/*.cpp" "src/main/*.hpp")
file(GLOB_RECURSE SRC_UTILS_FILES "src/utils/*.cpp" "src/utils/*.hpp")

set(SRCS
	${SRC_CORE_FILES}
	${SRC_UTILS_FILES}
	${SRC_FILES}
)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	if(APPLE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	endif()
    if (CMAKE_COMPILER_IS_GNUCXX)
		add_definitions (-march=native -mtune=native -msse4.2)
##huge optimization but take ages to compile
##		add_definitions (-flto)
		set (CMAKE_EXE_LINKER_FLAGS "-Wl,-O1,--sort-common,--as-needed,-z,relro")
		set (CMAKE_SHARED_LINKER_FLAGS "-Wl,-O1,--sort-common,--as-needed,-z,relro")
		set (CMAKE_MODULE_LINKER_FLAGS "-Wl,-O1,--sort-common,--as-needed,-z,relro")
    endif()
endif(UNIX)
if(WIN32)
	add_definitions (/MP)
endif()

include_directories (
    "src"
    "src/utils"
    "src/utils/png"
    "src/utils/zlib-1.2.11"
)

add_executable(
	${TARGET_NAME}
	${SRCS}
)

target_link_libraries(
	${TARGET_NAME}
)
