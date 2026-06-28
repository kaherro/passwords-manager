#include "../include/BigInt.h"
#include <cassert>

void test_contructor() {
    std::cout << "Running constructor tests...\n"; 

    assert(BigInt("00123") == BigInt("123")); 
    assert(BigInt("010") == BigInt("10")); 
    assert(BigInt("00") == BigInt("0"));
    assert(BigInt("000") == BigInt("0")); 
    assert(BigInt("-123") == -BigInt("123")); 
    assert(BigInt("-0123") == -BigInt("000123")); 
    assert(BigInt("-0") == -BigInt("0")); 
    assert(BigInt("-0") == BigInt("0")); 
    assert(BigInt("0") == -BigInt("0")); 


    try {
        BigInt x("--123"); 
        assert(false); 
    }
    catch (std::invalid_argument &e) {} 

    try {
        BigInt x("+123"); 
        assert(false); 
    }
    catch (std::invalid_argument &e) {} 

    try {
        BigInt x("abc"); 
        assert(false); 
    }
    catch (std::invalid_argument &e) {}

    try {
        BigInt x("*)-="); 
        assert(false); 
    }
    catch (std::invalid_argument &e) {}

    std::cout << "Constructor tests passed\n";
}

int main() {
    std::cout << "==========================\n";
    std::cout << " STARTING BIGINT TESTS...\n"; 
    std::cout << "==========================\n";

    test_contructor();

    std::cout << "==========================\n";
    std::cout << "   BIGINT TESTS PASSED! \n"; 
    std::cout << "==========================\n";
}