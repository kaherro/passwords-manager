#include "../include/BigInt.h"
#include <algorithm>
#include <stdexcept>

BigInt::BigInt() {
    sign = false; 
    s = ""; 
}

BigInt::BigInt(std::string s) {
    if(s.size() > 1 && s[0] == '-') {
        sign = true; 
        s.erase(0, 1); 
    }
    for(auto i : s) {
        if(!(i >= '0' && i <= '9')) {
            throw std::invalid_argument("BigInt() argument must be positive and only of numbers");
        }
    }
    int i = 0; 
    while(i < s.length() && s[i] == '0') {
        i++; 
    }
    s.erase(0, i);
    if(s == "") s = "0"; 
    this->s = s; 
}

BigInt BigInt::operator-() const{
    BigInt res = *this; 
    if(s != "0") res.sign = !res.sign; 
    return res; 
}

BigInt BigInt::operator+(const BigInt &x) const
{
    BigInt res; 
    if(sign && x.sign) res.sign = true; 
    if(sign && !x.sign) {
        BigInt a(s), b(x.s); 
        return b - a; 
    }
    if(!sign && x.sign) {
        BigInt a(s), b(x.s); 
        return a - b; 
    }
    int i = s.length() - 1, j = x.s.length() - 1, rem = 0; 
    while(i >= 0 || j >= 0 || rem) {
        int sum = rem; 
        if(i >= 0) {
            sum += s[i] - '0'; 
            i--;
        }
        if(j >= 0) {
            sum += x.s[j] - '0';
            j--;
        }
        res.s.push_back(sum % 10 + '0'); 
        rem = sum / 10;
    }
    reverse(res.s.begin(), res.s.end()); 
    return res;
}

BigInt BigInt::operator*(const BigInt &x) const {
    if(s == "0" || x.s == "0") {
        return BigInt("0");
    }
    BigInt res;
    res.sign = (sign != x.sign); 
    std::string nulls = ""; 
    for(auto num = x.s.end() - 1; num >= x.s.begin(); num--) {
        BigInt temp; 
        int i = s.length() - 1, rem = 0; 
        while(i >= 0 || rem) {
            int prod = rem; 
            if(i >= 0) {
                prod += (s[i] - '0') * (*num - '0'); 
                i--; 
            }
            temp.s.push_back(prod % 10 + '0'); 
            rem = prod / 10; 
        }
        reverse(temp.s.begin(), temp.s.end()); 
        temp.s += nulls;
        nulls += '0';
        res = res + temp;
    }
    return res; 
}

bool BigInt::operator==(const BigInt &x) const {
    return x.s == s && x.sign == sign;
}

bool BigInt::operator!=(const BigInt &x) const {
    return !(*this == x); 
}

bool BigInt::operator<(const BigInt &x) const {
    if(sign && !x.sign) return true; 
    if(!sign && x.sign) return false; 
    if(s.length() != x.s.length()) {
        if(!sign && !x.sign) {
            return s.length() < x.s.length();
        }
        if(sign && x.sign) {
            return s.length() > x.s.length();
        }
    }
    if(!sign && !x.sign) {
        return s < x.s; 
    }
    return s > x.s; 
}

bool BigInt::operator>(const BigInt &x) const {
    return (x < *this); 
}

bool BigInt::operator<=(const BigInt &x) const {
    return !(*this > x); 
}

bool BigInt::operator>=(const BigInt &x) const {
    return !(*this < x); 
}

BigInt BigInt::operator-(const BigInt &x) const {
    if(*this == x) {
        return BigInt("0"); 
    }

    // a - (-b)
    if(!sign && x.sign) {
        return *this + BigInt(x.s);
    }

    // -a - b
    if(sign && !x.sign) {
        BigInt res = BigInt(s) + x;
        res.sign = true;
        return res;
    }
    
    // -a - (-b)
    if(sign && x.sign) {
        BigInt a(s), b(x.s);
        if(a > b) {
            BigInt res = a - b;
            res.sign = true;
            return res;
        } else {
            return b - a;
        }
    }
    
    // a - b, a < b
    if(*this < x) {
        BigInt res = x - *this;
        res.sign = true;
        return res;
    }

    std::string res; 
    int i = s.length() - 1, j = x.s.length() - 1, bor = 0; 
    while(i >= 0) {
        int diff = s[i] - '0' - bor; 
        if(j >= 0) {
            diff -= x.s[j] - '0';
            j--;
        }
        if(diff < 0) {
            diff += 10; 
            bor = 1;
        }
        else bor = 0; 
        res.push_back(diff + '0'); 
        i--;
    }
    reverse(res.begin(), res.end());
    return BigInt(res);
}

BigInt BigInt::operator/(const BigInt &x) const {
    if(x.s == "0") {
        throw std::invalid_argument("Division by zero");
    }
    if(s < x.s) {
        return BigInt("0"); 
    }
    if(sign || x.sign) {
        BigInt res, a(s), b(x.s); 
        if(sign && x.sign) return a / b; 
        res = a / b; 
        res.sign = true; 
        return res; 
    }
    std::string res;
    BigInt cur; 
    for(int i = 0; i < s.length(); i++) {
        if(cur.s == "0") {
            cur.s = s[i];
        }
        else cur.s.push_back(s[i]);
        int cnt = 0;
        while(x <= cur) {
            cur = cur - x; 
            cnt++;
        }
        res.push_back(cnt + '0');
    }
    return BigInt(res);
}

BigInt BigInt::operator%(const BigInt &x) const {
    if(*this < x) return *this; 
    BigInt div = *this / x; 
    return *this - div * x; 
}

BigInt& BigInt::operator+=(const BigInt &x) {
    *this = *this + x; 
    return *this; 
}

BigInt& BigInt::operator*=(const BigInt &x) {
    *this = *this * x; 
    return *this; 
}

BigInt& BigInt::operator-=(const BigInt &x) {
    *this = *this - x; 
    return *this; 
}

BigInt& BigInt::operator/=(const BigInt &x) {
    *this = *this / x; 
    return *this; 
}

BigInt& BigInt::operator%=(const BigInt &x) {
    *this = *this % x; 
    return *this; 
}

BigInt BigInt::pow(BigInt a, BigInt n, const BigInt &mod) {
    BigInt b0("0"), b1("1"), b2("2"); 
    if(n == b0) {
        return b1; 
    }
    BigInt temp = pow(a % mod, n / b2, mod) % mod;
    if(n % b2 == b1) {
        return ((temp * temp) % mod * (a % mod)) % mod; 
    }
    return (temp * temp) % mod; 
}

BigInt BigInt::gcd(BigInt a, BigInt b) {
    BigInt b0("0");
    while (b > b0) {
        BigInt r = a % b;
        a = b;
        b = r;
    }
    return a; 
}

std::ostream& operator<<(std::ostream &cout, const BigInt &cur) {
    if(cur.sign) std::cout << '-';  
    std::cout << cur.s;
    return cout;
}
