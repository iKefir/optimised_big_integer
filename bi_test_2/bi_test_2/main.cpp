#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#define ull unsigned long long
#define ul unsigned int

#include <iosfwd>
#include <vector>
#include <string>
#include <iostream>


struct big_integer
{
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    big_integer(unsigned long long a);
    explicit big_integer(std::string const& str);
    ~big_integer();
    
    big_integer& operator=(big_integer const& other);
    
    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);
    //
    //    big_integer& operator&=(big_integer const& rhs);
    //    big_integer& operator|=(big_integer const& rhs);
    //    big_integer& operator^=(big_integer const& rhs);
    //
    //    big_integer& operator<<=(int rhs);
    //    big_integer& operator>>=(int rhs);
    //
    //    big_integer operator+() const;
    //    big_integer operator-() const;
    //    big_integer operator~() const;
    
    big_integer& operator++();
    big_integer operator++(int);
    
    big_integer& operator--();
    big_integer operator--(int);
    
    friend int compare(big_integer const& a, big_integer const& b);
    
        friend bool operator==(big_integer const& a, big_integer const& b);
        friend bool operator!=(big_integer const& a, big_integer const& b);
        friend bool operator<(big_integer const& a, big_integer const& b);
        friend bool operator>(big_integer const& a, big_integer const& b);
        friend bool operator<=(big_integer const& a, big_integer const& b);
        friend bool operator>=(big_integer const& a, big_integer const& b);
    
    friend std::string to_string(big_integer const& a);
    
private:
    std::vector <ul> data;
    int sign;
    const unsigned long long base = (unsigned long long) (1 << 16);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);
//
//big_integer operator&(big_integer a, big_integer const& b);
//big_integer operator|(big_integer a, big_integer const& b);
//big_integer operator^(big_integer a, big_integer const& b);
//
//big_integer operator<<(big_integer a, int b);
//big_integer operator>>(big_integer a, int b);
//
int compare(big_integer const& a, big_integer const& b);
bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif // BIG_INTEGER_H

#include <stdexcept>

big_integer::big_integer()
{
    sign = 1;
    data.resize(0);
}

big_integer::big_integer(big_integer const& other)
{
    sign = other.sign;
    data.resize(other.data.size());
    for (int i = 0; i < (int) data.size(); i++) {
        data[i] = other.data[i];
    }
}

big_integer::big_integer(int a)
{
    sign = 1;
    if (a < 0) {
        a = -a;
        sign = -1;
    }
    int cache;
    if (a == 0) data.push_back(0);
    while (a > 0) {
        cache = a % base;
        a /= base;
        data.push_back(cache);
    }
}

big_integer::big_integer(ull a)
{
    sign = 1;
    data.resize(2);
    data[0] = (ul) a;
    data[1] = a >> 32;
    if (data[1] == 0) data.pop_back();
}

big_integer::big_integer(std::string const& str)
{
    int start = 0;
    *this = big_integer(0);
    int postsign = 1;
    std::string a = &str[0];
    if (a[0] == '-') {
        postsign = -1;
        start = 1;
    }
    int cache = 0;
    big_integer TEN(100000000);
    for (int i = start; i < (int) a.length(); i++) {
        cache *= 10;
        cache += a[i] - 48;
        if (i % 8 == 7) {
            *this *= TEN;
            *this += big_integer(cache);
            cache = 0;
        }
    }
    int sval = a.length() % 8;
    int mult = 1;
    for (int i = 0; i < sval; i++) {
        mult *= 10;
    }
    *this *= big_integer(mult);
    *this += big_integer(cache);
    sign = postsign;
}

big_integer::~big_integer()
{
}

big_integer& big_integer::operator=(big_integer const& other)
{
    sign = other.sign;
    data.resize(other.data.size());
    for (int i = 0; i < (int) data.size(); i++) {
        data[i] = other.data[i];
    }
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& rhs)
{
    if (sign == rhs.sign) {
        long long cache = 0;
        int msize = (int) std::max(data.size(), rhs.data.size()) + 1;
        data.resize(msize);
        for (int i = 0; i < msize; i++) {
            data[i] += ((int) rhs.data.size() > i ? rhs.data[i] : 0) + cache;
            cache = data[i] / base;
            data[i] %= base;
        }
        if (*(data.end() - 1) == 0) data.pop_back();
    } else {
        sign = -sign;
        *this = rhs - *this;
    }
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs)
{
    if (sign == rhs.sign) {
        int comp = compare(*this, rhs);
        if (comp == 0) {
            *this = big_integer(0);
        }
        if (comp * sign == 1) {
            int cache = 0;
            for (int i = 0; i < (int) data.size(); i++) {
                if (data[i] < ((int) rhs.data.size() > i ? rhs.data[i] : 0) + cache)
                {
                    data[i] += base;
                    data[i] -= ((int) rhs.data.size() > i ? rhs.data[i] : 0) + cache;
                    cache = 1;
                } else {
                    data[i] -= ((int) rhs.data.size() > i ? rhs.data[i] : 0) + cache;
                    cache = 0;
                }
            }
            while (*(data.end() - 1) == 0) data.pop_back();
        }
        if (comp * sign == -1) {
            data.resize(rhs.data.size());
            int cache = 0;
            for (int i = 0; i < (int) data.size(); i++) {
                if (rhs.data[i] < data[i] + cache) {
                    data[i] = (ul)(rhs.data[i] + base - data[i] - cache);
                    cache = 1;
                } else {
                    data[i] = rhs.data[i] - data[i] - cache;
                    cache = 0;
                }
            }
            while (*(data.end() - 1) == 0) data.pop_back();
            sign = -sign;
        }
    } else {
        sign = -sign;
        *this = *this + rhs;
        sign = -sign;
    }
    return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs)
{
    if ((data.size() == 1 && data[0] == 0) || (rhs.data.size() == 1 && rhs.data[0] == 0)) {
        *this = big_integer(0);
    } else {
        big_integer res = big_integer(0);
        res.sign = sign * rhs.sign;
        long long cache, mres;
        res.data.resize(data.size() + rhs.data.size());
        for (int i = 0; i < data.size(); i++) {
            cache = 0;
            for (int j = 0; j < rhs.data.size(); j++) {
                mres = (ull) res.data[i + j] + (ull) data[i] * (ull) rhs.data[j] + cache;
                cache = mres / base;
                res.data[i + j] = (ul) (mres % base);
            }
            res.data[i + rhs.data.size()] += cache;
        }
        *this = res;
        while (*(data.end() - 1) == 0) data.pop_back();
    }
    
    return *this;
}

big_integer& big_integer::operator/=(big_integer const& rhs)
{
    big_integer right = rhs;
    big_integer ans(0);
    big_integer bmul(base);
    int somesgn = sign * right.sign;
    sign = right.sign;
    unsigned long div = 0;
    if (data.size() > right.data.size()) {
        div = data.size() - right.data.size();
        right.data.resize(data.size());
        for (unsigned long i = right.data.size() - 1; i >= div; i--) {
            right.data[i] = right.data[i - div];
        }
        for (unsigned long i = div - 1; i != 0; i--) {
            right.data[i] = 0;
        }
        right.data[0] = 0;
    }
    ull sch;
    int remsg;
    while (div > 0) {
        sch = 0;
        remsg = sign;
        while (sign == remsg) {
            *this -= right;
            ++sch;
        }
        *this += right;
        --sch;
        ans *= bmul;
        ans += big_integer(sch);
        --div;
        for (unsigned long i = div; i < right.data.size() - 1; i++) {
            right.data[i] = right.data[i + 1];
        }
        right.data.pop_back();
    }
    sch = 0;
    remsg = sign;
    while (sign == remsg) {
        *this -= right;
        ++sch;
    }
    if (!(data.size() == 1 && data[0] == 0)) {
        *this += right;
        --sch;
    }
    ans *= bmul;
    ans += big_integer(sch);
    ans.sign = somesgn;
    return *this = ans;
}

big_integer& big_integer::operator%=(big_integer const& rhs)
{
    big_integer right = rhs;
    big_integer ans(0);
    big_integer bmul(base);
    int somesgn = sign;
    ans.sign = sign * rhs.sign;
    sign = right.sign;
    unsigned long div = 0;
    if (data.size() > right.data.size()) {
        div = data.size() - right.data.size();
        right.data.resize(data.size());
        for (unsigned long i = right.data.size() - 1; i >= div; i--) {
            right.data[i] = right.data[i - div];
        }
        for (unsigned long i = div - 1; i != 0; i--) {
            right.data[i] = 0;
        }
        right.data[0] = 0;
    }
    ull sch;
    int remsg;
    while (div > 0) {
        sch = 0;
        remsg = sign;
        while (sign == remsg) {
            *this -= right;
            ++sch;
        }
        *this += right;
        --sch;
        ans *= bmul;
        ans += big_integer(sch);
        --div;
        for (unsigned long i = div; i < right.data.size() - 1; i++) {
            right.data[i] = right.data[i + 1];
        }
        right.data.pop_back();
    }
    sch = 0;
    remsg = sign;
    while (sign == remsg) {
        *this -= right;
        ++sch;
    }
    *this += right;
    --sch;
    ans *= bmul;
    ans += big_integer(sch);
    sign = 1;
    return *this;
}

//big_integer& big_integer::operator&=(big_integer const& rhs)
//{
//    mpz_and(mpz, mpz, rhs.mpz);
//    return *this;
//}
//
//big_integer& big_integer::operator|=(big_integer const& rhs)
//{
//    mpz_ior(mpz, mpz, rhs.mpz);
//    return *this;
//}
//
//big_integer& big_integer::operator^=(big_integer const& rhs)
//{
//    mpz_xor(mpz, mpz, rhs.mpz);
//    return *this;
//}
//
//big_integer& big_integer::operator<<=(int rhs)
//{
//    mpz_mul_2exp(mpz, mpz, rhs);
//    return *this;
//}
//
//big_integer& big_integer::operator>>=(int rhs)
//{
//    mpz_div_2exp(mpz, mpz, rhs);
//    return *this;
//}
//
//big_integer big_integer::operator+() const
//{
//    return *this;
//}
//
//big_integer big_integer::operator-() const
//{
//    big_integer r;
//    mpz_neg(r.mpz, mpz);
//    return r;
//}
//
//big_integer big_integer::operator~() const
//{
//    big_integer r;
//    mpz_com(r.mpz, mpz);
//    return r;
//}
//
big_integer& big_integer::operator++()
{
    *this += big_integer(1);
    return *this;
}

big_integer big_integer::operator++(int)
{
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer& big_integer::operator--()
{
    *this -= big_integer(1);
    return *this;
}

big_integer big_integer::operator--(int)
{
    big_integer r = *this;
    --*this;
    return r;
}

big_integer operator+(big_integer a, big_integer const& b)
{
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b)
{
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b)
{
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b)
{
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b)
{
    return a %= b;
}

//big_integer operator&(big_integer a, big_integer const& b)
//{
//    return a &= b;
//}
//
//big_integer operator|(big_integer a, big_integer const& b)
//{
//    return a |= b;
//}
//
//big_integer operator^(big_integer a, big_integer const& b)
//{
//    return a ^= b;
//}
//
//big_integer operator<<(big_integer a, int b)
//{
//    return a <<= b;
//}
//
//big_integer operator>>(big_integer a, int b)
//{
//    return a >>= b;
//}
//
int compare(big_integer const& a, big_integer const& b) {
    if (a.sign > b.sign) return 1;
    if (a.sign < b.sign) return -1;
    if (a.data.size() > b.data.size()) return a.sign;
    if (a.data.size() < b.data.size()) return -a.sign;
    for (int i = (int) a.data.size() - 1; i >= 0; i--) {
        if (a.data[i] > b.data[i]) return a.sign;
        if (a.data[i] < b.data[i]) return -a.sign;
    }
    return 0;
}

bool operator==(big_integer const& a, big_integer const& b)
{
    return (compare(a, b) == 0);
}

bool operator!=(big_integer const& a, big_integer const& b)
{
    return (compare(a, b) != 0);
}

bool operator<(big_integer const& a, big_integer const& b)
{
    return (compare(a, b) == -1);
}

bool operator>(big_integer const& a, big_integer const& b)
{
    return (compare(a, b) == 1);
}

bool operator<=(big_integer const& a, big_integer const& b)
{
    return !(compare(a, b) == 1);
}

bool operator>=(big_integer const& a, big_integer const& b)
{
    return !(compare(a, b) == -1);
}

std::string to_string(big_integer const& a)
{
    std::string res, sign = (a.sign == 1 ? "" : "-"), someress;
    big_integer someres;
    big_integer divizor = a;
    big_integer bmul(10000);
    if (divizor.data.size() == 1 && divizor.data[0] == 0) {
        res = "0";
        sign = "";
    }
    while (divizor.data.size() > 1 || divizor.data[0] != 0) {
        someres = divizor % bmul;
        divizor /= bmul;
        someress = std::to_string(someres.data[0]);
        if (divizor.data.size() > 1 || divizor.data[0] != 0) while (someress.length() < 4) someress = "0" + someress;
        res = someress + res;
    }
    res = sign + res;
    return res;
}

std::ostream& operator<<(std::ostream& s, big_integer const& a)
{
    return s << to_string(a);
}

using namespace std;

int main(int argc, const char * argv[]) {
    string a, b;
    cin >> a >> b;
    bool ans =((big_integer(-20) % big_integer(5)) == big_integer(0));
    cout << ans << endl;
    return 0;
}
