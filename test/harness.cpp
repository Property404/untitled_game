#include <iostream>
#include <vector>
#include "harness.hpp"

using namespace std;


static TestCase* cases[20];
static size_t cases_len = 0;

void TestCase::assert_bool(const char* msg, bool condition){
    if (condition) {
        this->assertions_passed++;
    } else {
        std::cerr << "Assert failed: " << msg  << std::endl;
        std::exit(1);
    }
}

void TestCase::print_stats() const {
    std::cout<< this->assertions_passed << " assertions passed" << std::endl;
}

/// Define the test case.
TestCase::TestCase(const char* file_name, const char* name, TestCase::Action action) {
        this->file_name = file_name;
        this->name = name;

        this->action = action;

        cases[cases_len] = this;
        cases_len++;
}

void TestCase::run_all() {
    for(size_t i=0;i<cases_len;i++) {
        cases[i]->run();
        cases[i]->print_stats();
    }
}

/// Run the test case.
void TestCase::run() {
    std::cout<< " Running test <" << this->file_name << "::" << this->name << ">\n";
    (this->action)(this);
}
