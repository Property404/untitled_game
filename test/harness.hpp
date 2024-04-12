#pragma once
#include <iostream>

#define TEST_CASE(name, ...) static TestCase _puppet_##name\
        (__extract_file_name(__FILE__), #name, [](TestCase* __self) __VA_ARGS__);

#define REQUIRE_EQUALS(value1, value2) do {\
        __self->assert_equals(""#value1, ""#value2, value1, value2);}while(0)
#define REQUIRE(condition) {__self->assert_bool("Assert failed: '" #condition, condition);}
#define REQUIRE_NOT(condition) REQUIRE(!(condition))

class TestCase final {
    using Action = void(*)(TestCase*);

    // The test procedure itself.
    Action action;

    const char* file_name;
    const char* name;

    int assertions_passed;

    template<typename T,typename U>
    void assert_equals_inner(const char* type1, const char* type2, T value1, U value2){
        if (value1 != value2) {
            std::cerr
                << "Assert failed: "
                << value1 << "(" << type1 << ")" << " != "
                << value2 << "(" << type2 << ")" << std::endl;
            std::exit(1);
        } else {
            this->assertions_passed++;
        }
    }

 public:
    TestCase(const char* file_name, const char* name, Action action);

    /// Run the test case.
    void run();

    /// Run all test cases
    static void run_all();

    template<typename T,typename U>
    void assert_equals(const char* type1, const char* type2, T value1, U value2){
        this->assert_equals_inner<
                std::remove_reference_t<T>,
                std::remove_reference_t<U>
            >
            (type1, type2, value1, value2);
    }


    void assert_bool(const char* msg, bool condition);

    void print_stats() const;
};


// https://stackoverflow.com/questions/31050113/
// how-to-extract-the-source-filename-without-path-and-suffix-at-compile-time
constexpr const char* __extract_file_name(const char* path) {
    const char* file = path;
    while (*path) {
        if (*path++ == '/') {
            file = path;
        }
    }
    return file;
}
