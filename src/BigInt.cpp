#include "../include/BigInt.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

BigInt::BigInt() {
    s = ""; 
}
BigInt::BigInt(std::string s) {
    int i = 0; 
    while(i < s.length() && s[i] == '0') {
        i++; 
    }
    s.erase(0, i);
    if(s == "") s = "0"; 
    this->s = s; 
}

BigInt BigInt::operator+(const BigInt &x) const {
    BigInt res; 
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
};

BigInt BigInt::operator*(const BigInt &x) const {
    if(s == "0" || x.s == "0") {
        return BigInt("0");
    }
    BigInt res;
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
    return x.s == s;
}

bool BigInt::operator<(const BigInt &x) const {
    if(s.length() != x.s.length()) {
        return s.length() < x.s.length();
    }
    return s < x.s; 
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

// Works only for positive results (a > b)
BigInt BigInt::operator-(const BigInt &x) const {
    if(*this < x) {
        throw std::invalid_argument("First argument must be greater than second");
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

std::ostream& operator<<(std::ostream &cout, const BigInt &cur) {
    cout << cur.s;
    return cout;
}

int main() {
    BigInt x("123"), y("987");
    std::cout << y - x;
}