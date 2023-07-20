find_package(GTest CONFIG REQUIRED)

add_executable(wre_test wre_test.cpp)
target_link_libraries(wre_test PUBLIC wre GTest::gtest GTest::gtest_main)

# define wre properties
set_target_properties(wre_test PROPERTIES
    EXPORT_COMPILE_COMMANDS ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

enable_testing()
add_test(NAME wre_test COMMAND wre_test)