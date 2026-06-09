#include "BigInt.h"

namespace rsa {
    BigInt mod_inverse(BigInt e, BigInt phi);
    void generate_keys(BigInt &e, BigInt &d, BigInt &n);

    BigInt encrypt(BigInt m, BigInt e, BigInt n);
    BigInt decrypt(BigInt c, BigInt d, BigInt n); 
}