#include "../include/rsa.h"

BigInt rsa::mod_inverse(BigInt e, BigInt phi) {
    BigInt d("0"), new_d("1");
    BigInt r = phi, new_r = e;
    while (new_r != BigInt("0")) {
        BigInt quotient = r / new_r;
        BigInt temp_d = d;
        d = new_d;
        new_d = temp_d - quotient * new_d;
        BigInt temp_r = r;
        r = new_r;
        new_r = temp_r - quotient * new_r;
    }
    
    if (r > BigInt("1")) {
        throw std::runtime_error("e is not inversible");
    }
    if (d < BigInt("0")) {
        d = d + phi;
    }
    return d;
}

void rsa::generate_keys(BigInt &e, BigInt &d, BigInt &n) {
    BigInt p("10000019"); 
    BigInt q("10000079"); 
    n = p * q; 
    BigInt phi = (p - BigInt("1")) * (q - BigInt("1")); 
    for(e = BigInt("2"); e < phi; e += BigInt("1")) {
        if(BigInt::gcd(e, phi) == BigInt("1")) break;
    }
    d = mod_inverse(e, phi); 
}

BigInt rsa::encrypt(BigInt m, BigInt e, BigInt n) {
    return BigInt::pow(m, e, n); 
}

BigInt rsa::decrypt(BigInt c, BigInt d, BigInt n) {
    return BigInt::pow(c, d, n); 
}