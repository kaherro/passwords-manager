#include <string>

class BigInt {
private: 
    std::string s;
public:
    BigInt();
    BigInt(std::string s);

    BigInt operator+(const BigInt &x) const;
    BigInt operator*(const BigInt &x) const;

    friend std::ostream& operator<<(std::ostream &cout, const BigInt &cur);
};
