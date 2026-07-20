#include "rsa.h"
#include <algorithm>

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

BigInt rsa::get_random_bigint(const BigInt min, const BigInt max) {
    BigInt range = max - min + BigInt("1");
    std::string range_s = range.to_string();
    std::string res_s = "";
    for (int i = 0; i < range_s.length(); ++i) {
        res_s += std::to_string(std::rand() % 10);
    }
    BigInt res(res_s);
    return min + (res % range);
}

bool rsa::miller_rabin(BigInt n, int k) {
    BigInt b0("0"), b1("1"), b2("2"), b3("3");
    if (n < b2) return false;
    if (n == b2 || n == b3) return true;
    if (n % b2 == b0) return false;

    BigInt d = n - b1;
    BigInt s("0");
    while (d % b2 == b0) {
        d /= b2;
        s += b1;
    }

    for (int i = 0; i < k; i++) {
        BigInt a = get_random_bigint(b2, n - b2);
        BigInt x = BigInt::pow(a, d, n);
        if (x == b1 || x == n - b1) {
            continue;
        }
        bool prime = false;
        for (BigInt r("1"); r < s; r += b1) {
            x = BigInt::pow(x, b2, n);
            if (x == n - b1) {
                prime = true;
                break;
            }
        }
        if(!prime) return false;
    }
    return true;
}

BigInt rsa::generate_prime() {
    srand(time(0));
    std::string key_s = ""; 
    for(int i = 0; i < 4; i++) {
        long long random_num = 1e9 + (std::rand() % (long long)(9999999999 - 1e9 + 1));
        key_s += std::to_string(random_num); 
    }
    BigInt key(key_s);
    if (key % BigInt("2") == BigInt("0")) {
        key += BigInt("1");
    }
    while (!miller_rabin(key, 8)) { 
        key += BigInt("2");
    }
    return key;
}


BigInt rsa::generate_keys(BigInt &e, BigInt &d, BigInt &n) {
    BigInt p = generate_prime(); 
    BigInt q = generate_prime(); 
    n = p * q; 
    BigInt phi = (p - BigInt("1")) * (q - BigInt("1")); 
    for(e = BigInt("2"); e < phi; e += BigInt("1")) {
        if(BigInt::gcd(e, phi) == BigInt("1")) break;
    }
    d = mod_inverse(e, phi); 
    return p; 
}

BigInt rsa::get_hash_num(std::string msg, const BigInt k) {
    BigInt h("0"); 
    for(char c : msg) {
        h = h * k + BigInt(std::to_string(c));
    }
    return h; 
}

std::string rsa::get_hash_msg(BigInt h, const BigInt k) {
    std::string res = "";
    while(h > BigInt("0")) {
        res = res + char((h % k).to_int()); 
        h /= k; 
    }
    std::reverse(res.begin(), res.end()); 
    return res;
}

BigInt rsa::encrypt(std::string msg, BigInt e, BigInt n) {
    if(msg.length() > 20) {
        throw std::invalid_argument("Message length must be <= 20");
    }
    return BigInt::pow(get_hash_num(msg, BigInt("257")), e, n); 
}

std::string rsa::decrypt(BigInt c, BigInt d, BigInt n) {
    BigInt h = BigInt::pow(c, d, n); 
    return get_hash_msg(h, BigInt("257")); 
}