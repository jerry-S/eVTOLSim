enable_testing()

find_package(GTest REQUIRED)

######### Unit tests #########
include_directories(${GTEST_INCLUDE_DIRS})
message(STATUS "  GTEST_INCLUDE_DIRS  ${GTEST_INCLUDE_DIRS}")

add_executable(unit_tests EVTest.cpp simQueueTest.cpp ../random/src/simpleRandomGen.cpp ../ev/src/ev.cpp)

target_link_libraries(unit_tests     
    ${GTEST_BOTH_LIBRARIES} 
    pthread
)
# We can use this to run "make check" instead of directly executing the unit test executable
add_test(UnitTests unit_tests)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS unit_tests)
