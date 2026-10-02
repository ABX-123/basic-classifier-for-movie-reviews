cmake_minimum_required(VERSION 3.20)

project(CppPythonSentiment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# Dependencies
# ==============================================================================

# Python 3
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# GoogleTest
find_package(GTest REQUIRED)

# ==============================================================================
# Main Application Target
# ==============================================================================

add_executable(sentiment 
    src/main.cpp 
    src/SentimentAnalyzer.cpp
)

target_include_directories(sentiment PRIVATE 
    ${Python3_INCLUDE_DIRS}
)

target_link_libraries(sentiment PRIVATE 
    Python3::Python
)

# ==============================================================================
# Testing Target
# ==============================================================================

enable_testing()

add_executable(sentiment_tests 
    tests/SentimentTests.cpp 
    src/SentimentAnalyzer.cpp
)

target_include_directories(sentiment_tests PRIVATE 
    ${Python3_INCLUDE_DIRS}
)

target_link_libraries(sentiment_tests PRIVATE 
    Python3::Python 
    GTest::gtest 
    GTest::gtest_main
)

add_test(
    NAME SentimentTests 
    COMMAND sentiment_tests
)
