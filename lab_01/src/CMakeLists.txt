cmake_minimum_required(VERSION 3.16)

project(app.exe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Опции для санитайзеров
option(ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(ENABLE_UBSAN "Enable Undefined Behavior Sanitizer" OFF)
option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)
option(ENABLE_COVERAGE "Enable code coverage for tests" OFF)

add_compile_options(-O0)

find_package(GTest REQUIRED)


# Настройка санитайзеров
if(ENABLE_ASAN OR ENABLE_UBSAN OR ENABLE_TSAN)
    set(SANITIZER_FLAGS "")
    
    if(ENABLE_ASAN)
        list(APPEND SANITIZER_FLAGS "-fsanitize=address")
        # Для MacOS также нужна эта опция
        list(APPEND SANITIZER_FLAGS "-fno-omit-frame-pointer")
    endif()
    
    if(ENABLE_UBSAN)
        list(APPEND SANITIZER_FLAGS "-fsanitize=undefined")
        list(APPEND SANITIZER_FLAGS "-fno-sanitize-recover=all")
    endif()
    
    if(ENABLE_TSAN)
        list(APPEND SANITIZER_FLAGS "-fsanitize=thread")
    endif()
    
    # Добавляем флаги компиляции и линковки
    add_compile_options(${SANITIZER_FLAGS})
    add_link_options(${SANITIZER_FLAGS})
    
    if(APPLE)
        add_link_options(-fuse-ld=ld)
    endif()
    
    message(STATUS "Sanitizers enabled: ASAN=${ENABLE_ASAN}, UBSAN=${ENABLE_UBSAN}, TSAN=${ENABLE_TSAN}")
endif()


file(GLOB_RECURSE DEFAULT  "matrix/*.cpp" "measurment/*.cpp")
file(GLOB_RECURSE APP_SOURCES  "main.cpp" ${DEFAULT})

add_executable(app.exe "main.cpp" ${DEFAULT})
target_include_directories(app.exe PRIVATE matrix measurment defaults)

# Сборка тестового
file(GLOB_RECURSE TEST_SOURCES  "tests/*.cpp" ${DEFAULT})

list(FILTER TEST_SOURCES EXCLUDE REGEX "main\\.cpp$") # Удаляем main.cpp из тестовых источников
add_executable(tests.exe ${TEST_SOURCES})
target_include_directories(tests.exe PRIVATE matrix measurment defaults tests)

target_link_libraries(tests.exe GTest::GTest GTest::Main)

if(ENABLE_ASAN OR ENABLE_UBSAN OR ENABLE_TSAN)
    target_compile_options(tests.exe PRIVATE ${SANITIZER_FLAGS})
    target_link_options(tests.exe PRIVATE ${SANITIZER_FLAGS})
endif()

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tests.exe PRIVATE --coverage)
    target_link_options(tests.exe PRIVATE --coverage)
    message(STATUS "Code coverage enabled for tests")
endif()
