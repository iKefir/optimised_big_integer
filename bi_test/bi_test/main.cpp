#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iosfwd>
#include <vector>
#include <string>
#include <iostream>


struct big_integer
{
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    explicit big_integer(std::string const& str);
    ~big_integer();
    
    big_integer& operator=(big_integer const& other);
    
    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    //    big_integer& operator/=(big_integer const& rhs);
    //    big_integer& operator%=(big_integer const& rhs);
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
    //
    //    friend bool operator==(big_integer const& a, big_integer const& b);
    //    friend bool operator!=(big_integer const& a, big_integer const& b);
    //    friend bool operator<(big_integer const& a, big_integer const& b);
    //    friend bool operator>(big_integer const& a, big_integer const& b);
    //    friend bool operator<=(big_integer const& a, big_integer const& b);
    //    friend bool operator>=(big_integer const& a, big_integer const& b);
    //
    friend std::string to_string(big_integer const& a);
    
private:
    std::vector <unsigned long> data;
    int sign;
    const int base = 10;
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
//big_integer operator/(big_integer a, big_integer const& b);
//big_integer operator%(big_integer a, big_integer const& b);
//
//big_integer operator&(big_integer a, big_integer const& b);
//big_integer operator|(big_integer a, big_integer const& b);
//big_integer operator^(big_integer a, big_integer const& b);
//
//big_integer operator<<(big_integer a, int b);
//big_integer operator>>(big_integer a, int b);
//
int compare(big_integer const& a, big_integer const& b);
//bool operator==(big_integer const& a, big_integer const& b);
//bool operator!=(big_integer const& a, big_integer const& b);
//bool operator<(big_integer const& a, big_integer const& b);
//bool operator>(big_integer const& a, big_integer const& b);
//bool operator<=(big_integer const& a, big_integer const& b);
//bool operator>=(big_integer const& a, big_integer const& b);
//
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

big_integer::big_integer(std::string const& str)
{
    int start = 0;
    sign = 1;
    std::string a = &str[0];
    if (a[0] == '-') {
        sign = -1;
        start = 1;
    }
    data.resize(a.length() - start);
    for (int i = 0; i < (int) a.length() - start; i++) {
        data[i] = a[a.length() - i - 1] - 48;
    }
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
                    data[i] = rhs.data[i] + base - data[i] - cache;
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
        long long cache;
        res.data.resize(data.size() + rhs.data.size());
        for (int i = 0; i < data.size(); i++) {
            cache = 0;
            for (int j = 0; j < rhs.data.size(); j++) {
                res.data[i + j] += data[i] * rhs.data[j] + cache;
                cache = res.data[i + j] / base;
                res.data[i + j] %= base;
            }
            res.data[i + rhs.data.size()] += cache;
        }
        *this = res;
        while (*(data.end() - 1) == 0) data.pop_back();
    }
    
    return *this;
}

//big_integer& big_integer::operator/=(big_integer const& rhs)
//{
//    mpz_tdiv_q(mpz, mpz, rhs.mpz);
//    return *this;
//}
//
//big_integer& big_integer::operator%=(big_integer const& rhs)
//{
//    mpz_tdiv_r(mpz, mpz, rhs.mpz);
//    return *this;
//}

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

//big_integer operator/(big_integer a, big_integer const& b)
//{
//    return a /= b;
//}
//
//big_integer operator%(big_integer a, big_integer const& b)
//{
//    return a %= b;
//}
//
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

//bool operator==(big_integer const& a, big_integer const& b)
//{
//    return mpz_cmp(a.mpz, b.mpz) == 0;
//}
//
//bool operator!=(big_integer const& a, big_integer const& b)
//{
//    return mpz_cmp(a.mpz, b.mpz) != 0;
//}
//
//bool operator<(big_integer const& a, big_integer const& b)
//{
//    return mpz_cmp(a.mpz, b.mpz) < 0;
//}
//
//bool operator>(big_integer const& a, big_integer const& b)
//{
//    return mpz_cmp(a.mpz, b.mpz) > 0;
//}
//
//bool operator<=(big_integer const& a, big_integer const& b)
//{
//    return mpz_cmp(a.mpz, b.mpz) <= 0;
//}
//
//bool operator>=(big_integer const& a, big_integer const& b)
//{
//    return mpz_cmp(a.mpz, b.mpz) >= 0;
//}
//
std::string to_string(big_integer const& a)
{
    std::string res = (a.sign == 1 ? "" : "-");
    char c;
    for (int i = (int) a.data.size() - 1; i >= 0; i--) {
        c = a.data[i] + 48;
        res += c;
    }
    return res;
}

std::ostream& operator<<(std::ostream& s, big_integer const& a)
{
    return s << to_string(a);
}

using namespace std;

int main() {
    int n, nmod, pn;
    cin >> n;
    vector <big_integer> aa;
    if (n == 0) {
        cout << 1 << endl;
    } else {
        aa.resize(n);
        for (int i = 1; i <= n; i++) {
            aa[i - 1] = big_integer(i);
        }
        while (n > 1) {
            nmod = n % 2;
            pn = n;
            n /= 2;
            for (int i = 0; i < n; i++) {
                aa[i] = aa[2 * i] * aa[2 * i + 1];
            }
            if (nmod == 1) {
                aa[n] = aa[2 * n];
            }
            n += nmod;
        }
        cout << aa[0] << endl;
    }
}
