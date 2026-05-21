#include <string>
#include <iostream>

class BigInt {
private: 
    bool sign = false; 
    std::string s;
public:
    BigInt();
    BigInt(std::string s);

    BigInt operator-() const; 

    BigInt operator+(const BigInt &x) const;
    BigInt operator*(const BigInt &x) const;
    BigInt operator-(const BigInt &x) const;
    BigInt operator/(const BigInt &x) const;
    BigInt operator%(const BigInt &x) const;

    BigInt& operator+=(const BigInt &x);
    BigInt& operator*=(const BigInt &x);
    BigInt& operator-=(const BigInt &x);
    BigInt& operator/=(const BigInt &x);
    BigInt& operator%=(const BigInt &x);

    bool operator==(const BigInt &x) const;
    bool operator!=(const BigInt &x) const;
    bool operator<(const BigInt &x) const;
    bool operator<=(const BigInt &x) const;
    bool operator>(const BigInt &x) const;
    bool operator>=(const BigInt &x) const;

    static BigInt pow(BigInt a, BigInt n, const BigInt &mod); 
    static BigInt gcd(BigInt a, BigInt b);

    friend std::ostream& operator<<(std::ostream &cout, const BigInt &cur);
};
