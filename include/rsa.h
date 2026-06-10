#include "BigInt.h"

namespace rsa {
    BigInt mod_inverse(BigInt e, BigInt phi);
    BigInt generate_keys(BigInt &e, BigInt &d, BigInt &n);

    BigInt encrypt(std::string m, BigInt e, BigInt n);
    std::string decrypt(BigInt c, BigInt d, BigInt n); 

    BigInt get_hash_num(std::string num, const BigInt k);
    std::string get_hash_msg(BigInt h, const BigInt k); 

    BigInt get_random_bigint(const BigInt min, const BigInt max);
    bool miller_rabin(BigInt n, int k);
    BigInt generate_prime(); 
}