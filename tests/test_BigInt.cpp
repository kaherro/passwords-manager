#include "BigInt.h"
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

void test_comparing() {
    std::cout << "Running comparing tests...\n"; 
    assert(BigInt("123") == BigInt("123")); 
    assert(BigInt("-123") == BigInt("-123")); 
    assert(BigInt("-0") == BigInt("0")); 
    assert(BigInt("-0") != BigInt("1")); 
    assert(BigInt("2") != BigInt("1")); 

    assert(BigInt("2") > BigInt("1"));
    assert(BigInt("2") >= BigInt("1"));
    assert(BigInt("10") > BigInt("1")); 
    assert(BigInt("10") >= BigInt("1")); 
    assert(BigInt("10") >= BigInt("10"));
    assert(BigInt("-2") < BigInt("1"));
    assert(BigInt("-2") <= BigInt("1"));
    assert(BigInt("-10") < BigInt("1")); 
    assert(BigInt("-10") <= BigInt("1")); 
    assert(BigInt("-10") <= BigInt("10"));
    
    assert(BigInt("1") < BigInt("2"));
    assert(BigInt("1") <= BigInt("2"));
    assert(BigInt("1") < BigInt("10")); 
    assert(BigInt("1") <= BigInt("10")); 
    assert(BigInt("10") <= BigInt("10"));
    assert(BigInt("1") > BigInt("-2"));
    assert(BigInt("1") >= BigInt("-2"));
    assert(BigInt("1") > BigInt("-10")); 
    assert(BigInt("1") >= BigInt("-10")); 
    assert(BigInt("10") >= BigInt("-10"));
    std::cout << "Comparing tests passed\n";
}

void test_addition() {
    std::cout << "Running addition tests...\n"; 
    assert(BigInt("123") + BigInt("123") == BigInt("246")); 
    assert(BigInt("-123") + BigInt("123") == BigInt("0")); 
    assert(BigInt("-123") + BigInt("-123") == BigInt("-246")); 
    assert(BigInt("-124") + BigInt("123") == BigInt("-1")); 
    assert(BigInt("123") + BigInt("-123") == BigInt("0")); 
    std::cout << "Addition tests passed\n";
}

void test_decreasing() {
    std::cout << "Running decreasing tests...\n"; 
    assert(BigInt("123") - BigInt("123") == BigInt("0")); 
    assert(BigInt("-123") - BigInt("123") == BigInt("-246")); 
    assert(BigInt("-123") - BigInt("-123") == BigInt("0")); 
    assert(BigInt("-124") - BigInt("123") == BigInt("-247")); 
    assert(BigInt("123") - BigInt("-123") == BigInt("246")); 
    std::cout << "Decreasing tests passed\n";
}

void test_multiplication() {
    std::cout << "Running multiplication tests...\n"; 
    assert(BigInt("123") * BigInt("123") == BigInt("15129"));
    assert(BigInt("123") * BigInt("-123") == BigInt("-15129"));
    assert(BigInt("-123") * BigInt("123") == BigInt("-15129")); 
    assert(BigInt("-123") * BigInt("-123") == BigInt("15129")); 
    assert(BigInt("-1") * BigInt("-123") == BigInt("123")); 
    assert(BigInt("-1234567") * BigInt("0") == BigInt("0"));
    std::cout << "Multiplication tests passed\n";
}

void test_division() {
    std::cout << "Running division tests...\n"; 
    assert(BigInt("123") / BigInt("123") == BigInt("1")); 
    assert(BigInt("-123") / BigInt("123") == BigInt("-1")); 
    assert(BigInt("-123") / BigInt("-123") == BigInt("1")); 
    assert(BigInt("-124") / BigInt("123") == BigInt("-1")); 
    assert(BigInt("123") / BigInt("-123") == BigInt("-1"));
    assert(BigInt("122") / BigInt("-123") == BigInt("0"));
    try{
        assert(BigInt("123") / BigInt("0") == BigInt("-1"));
    }
    catch (std::invalid_argument &e) {}
    std::cout << "Division tests passed\n"; 
}

void test_remainder() {
    std::cout << "Running remainder tests...\n"; 
    assert(BigInt("245") % BigInt("123") == BigInt("122")); 
    assert(BigInt("123") % BigInt("123") == BigInt("0"));   
    assert(BigInt("-123") % BigInt("123") == BigInt("0")); 
    assert(BigInt("-124") % BigInt("123") == BigInt("-1")); 
    assert(BigInt("124") % BigInt("-123") == BigInt("1")); 
    try{
        assert(BigInt("123") % BigInt("0") == BigInt("-1"));
    }
    catch (std::invalid_argument &e) {}
    std::cout << "Remainder tests passed\n";
}

void test_gcd() {
    std::cout << "Running gcd tests...\n"; 
    assert(BigInt::gcd(BigInt("123"), BigInt("246")) == BigInt("123")); 
    assert(BigInt::gcd(BigInt("123"), BigInt("-246")) == BigInt("123")); 
    assert(BigInt::gcd(BigInt("122"), BigInt("246")) == BigInt("2")); 
    assert(BigInt::gcd(BigInt("122"), BigInt("247")) == BigInt("1")); 
    assert(BigInt::gcd(BigInt("0"), BigInt("12345")) == BigInt("12345")); 
    std::cout << "Gcd tests passed\n";
}

void test_pow() {
    std::cout << "Running pow tests...\n"; 
    const BigInt MOD = BigInt("1000000007"); 
    assert(BigInt::pow(BigInt("2"), BigInt("0"), MOD) == BigInt("1")); 
    assert(BigInt::pow(BigInt("2"), BigInt("8"), MOD) == BigInt("256")); 
    assert(BigInt::pow(BigInt("-2"), BigInt("8"), MOD) == BigInt("256")); 
    assert(BigInt::pow(BigInt("-2"), BigInt("9"), MOD) == BigInt("-512")); 
    assert(BigInt::pow(BigInt("0"), BigInt("9"), MOD) == BigInt("0"));  
    assert(BigInt::pow(BigInt("12356346"), BigInt("1688125963"), MOD) == BigInt("279803131")); 
    assert(BigInt::pow(BigInt("-12356346"), BigInt("1688125963"), MOD) == BigInt("-279803131")); 
    std::cout << "Pow tests passed\n";
}

void test_to_int() {
    std::cout << "Running to_int() tests...\n"; 
    assert(BigInt("123").to_int() == 123);
    assert(BigInt("-123").to_int() == -123);
    assert(BigInt("0").to_int() == 0);
    std::cout << "To_int() tests passed\n";
}

void test_to_string() { // without -
    std::cout << "Running to_string() tests...\n"; 
    assert(BigInt("123").to_string() == "123");
    assert(BigInt("-123").to_string() == "123");
    assert(BigInt("0").to_string() == "0");
    std::cout << "To_string() tests passed\n";
}

int main() {
    std::cout << "==========================\n";
    std::cout << " STARTING BIGINT TESTS...\n"; 
    std::cout << "==========================\n";

    test_contructor();
    test_comparing();
    test_addition();
    test_decreasing(); 
    test_multiplication();  
    test_division();
    test_remainder();
    test_gcd(); 
    test_pow(); 
    test_to_int();
    test_to_string(); 

    std::cout << "==========================\n";
    std::cout << "   BIGINT TESTS PASSED! \n"; 
    std::cout << "==========================\n";
}