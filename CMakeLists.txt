cmake_minimum_required(VERSION 3.10)
project(OTP_Backend)

# C++17 표준을 사용하도록 설정
set(CMAKE_CXX_STANDARD 17)

# include 디렉토리를 추가하여 openai.hpp 파일을 찾을 수 있도록 설정
include_directories("GPT_API" "GPT_API/nlohmann" "C:/mingw64/include" "Analysis")

# CURL 경로를 수동으로 설정
set(CURL_INCLUDE_DIR "C:/mingw64/include")
set(CURL_LIBRARY "C:/mingw64/lib/libcurl.a")

# 라이브러리 및 include 경로 설정
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

# 소스 파일 인코딩 설정
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-finput-charset=UTF-8>")
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-fexec-charset=UTF-8>")

# 소스 파일 추가
add_executable(OTP_Backend main.cpp Analysis/analysis.cpp Analysis/save_js.cpp)

# 필요한 라이브러리를 링크
target_link_libraries(OTP_Backend PRIVATE ${CURL_LIBRARY})
