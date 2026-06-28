#include "BigInt.h"
#include "rsa.h"
#include <cassert>
#include <iostream>

void test_get_hash_num() {
    std::cout << "Running get_hash_num tests...\n";
    assert(rsa::get_hash_num("", BigInt("257")) == BigInt("0"));
    assert(rsa::get_hash_num("A", BigInt("257")) == BigInt("65"));
    assert(rsa::get_hash_num("hello", BigInt("257")) == rsa::get_hash_num("hello", BigInt("257")));
    assert(rsa::get_hash_num("hello", BigInt("257")) != rsa::get_hash_num("world", BigInt("257")));
    assert(rsa::get_hash_num("ab", BigInt("257")) != rsa::get_hash_num("ba", BigInt("257")));
    std::cout << "get_hash_num tests passed\n";
}

void test_get_hash_msg() {
    std::cout << "Running get_hash_msg tests...\n";

    std::string original = "hello";
    BigInt h = rsa::get_hash_num(original, BigInt("257"));
    assert(rsa::get_hash_msg(h, BigInt("257")) == original);

    original = "test";
    h = rsa::get_hash_num(original, BigInt("257"));
    assert(rsa::get_hash_msg(h, BigInt("257")) == original);

    original = "abc123";
    h = rsa::get_hash_num(original, BigInt("257"));
    assert(rsa::get_hash_msg(h, BigInt("257")) == original);

    std::cout << "get_hash_msg tests passed\n";
}

void test_encrypt_decrypt() {
    std::cout << "Running encrypt/decrypt tests...\n";

    BigInt e, d, n;
    rsa::generate_keys(e, d, n);

    std::string msg = "hello";
    BigInt enc = rsa::encrypt(msg, e, n);
    std::string dec = rsa::decrypt(enc, d, n);
    assert(dec == msg);

    msg = "a";
    enc = rsa::encrypt(msg, e, n);
    dec = rsa::decrypt(enc, d, n);
    assert(dec == msg);

    msg = "12345678901234567890";
    enc = rsa::encrypt(msg, e, n);
    dec = rsa::decrypt(enc, d, n);
    assert(dec == msg);

    BigInt enc1 = rsa::encrypt("hello", e, n);
    BigInt enc2 = rsa::encrypt("world", e, n);
    assert(enc1 != enc2);

    assert(rsa::encrypt("hello", e, n) == rsa::encrypt("hello", e, n));

    try {
        rsa::encrypt("123456789012345678901", e, n);
        assert(false);
    }
    catch (std::invalid_argument &e) {}

    std::cout << "Encrypt/decrypt tests passed\n";
}

void test_mod_inverse() {
    std::cout << "Running mod_inverse tests...\n";

    // e * d ≡ 1 (mod phi)
    BigInt e("7"), phi("40");
    BigInt d = rsa::mod_inverse(e, phi);
    assert((e * d) % phi == BigInt("1"));

    e = BigInt("13");
    phi = BigInt("120");
    d = rsa::mod_inverse(e, phi);
    assert((e * d) % phi == BigInt("1"));

    try {
        rsa::mod_inverse(BigInt("2"), BigInt("4")); 
        assert(false);
    }
    catch (std::runtime_error &e) {}

    std::cout << "mod_inverse tests passed\n";
}

void test_miller_rabin() {
    std::cout << "Running miller_rabin tests...\n";
    assert(rsa::miller_rabin(BigInt("2"),  8) == true);
    assert(rsa::miller_rabin(BigInt("3"),  8) == true);
    assert(rsa::miller_rabin(BigInt("7"),  8) == true);
    assert(rsa::miller_rabin(BigInt("97"), 8) == true);
    assert(rsa::miller_rabin(BigInt("7919"), 8) == true);
    assert(rsa::miller_rabin(BigInt("1"),  8) == false);
    assert(rsa::miller_rabin(BigInt("4"),  8) == false);
    assert(rsa::miller_rabin(BigInt("100"), 8) == false);
    assert(rsa::miller_rabin(BigInt("561"), 8) == false); 
    std::cout << "miller_rabin tests passed\n";
}

void test_generate_keys() {
    std::cout << "Running generate_keys tests...\n";

    BigInt e, d, n;
    BigInt p = rsa::generate_keys(e, d, n);

    std::string msg = "test";
    assert(rsa::decrypt(rsa::encrypt(msg, e, n), d, n) == msg);

    assert(n > BigInt("0"));

    assert(e > BigInt("1"));

    BigInt e2, d2, n2;
    rsa::generate_keys(e2, d2, n2);
    assert(n != n2);

    std::cout << "generate_keys tests passed\n";
}

int main() {
    std::cout << "==========================\n";
    std::cout << "  STARTING RSA TESTS...  \n";
    std::cout << "==========================\n";

    test_get_hash_num();
    test_get_hash_msg();
    test_mod_inverse();
    test_miller_rabin();
    test_generate_keys();
    test_encrypt_decrypt();

    std::cout << "==========================\n";
    std::cout << "    RSA TESTS PASSED!    \n";
    std::cout << "==========================\n";
}