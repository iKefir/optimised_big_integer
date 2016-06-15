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
    
    friend big_integer fuldivsmall(big_integer left, big_integer right, bool do_div);
    friend big_integer fuldiv(big_integer left, big_integer right, bool do_div);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);
    
    friend big_integer shapeshift(big_integer a);
    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);
    //
    //    big_integer& operator<<=(int rhs);
    //    big_integer& operator>>=(int rhs);
    //
    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;
    
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
    const unsigned long long base = (unsigned long long) (1 << 30) * 4;
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);

big_integer fuldivsmall(big_integer left, big_integer right, bool do_div);
big_integer fuldiv(big_integer left, big_integer right, bool do_div);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer shapeshift(big_integer a);
big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);
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

#define absolute(a) (a.sign > 0 ? a : -a)
#define is_null(a) (a.data.size() == 1 && a.data[0] == 0)
#include <stdexcept>

big_integer::big_integer()
{
    sign = 1;
    data.resize(1);
    data[0] = 0;
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
    long long la = a;
    sign = 1;
    if (la < 0) {
        la = -la;
        sign = -1;
    }
    long long cache;
    if (la == 0) data.push_back(0);
    while (la > 0) {
        cache = la % base;
        la /= base;
        data.push_back((ul) cache);
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
        long long cache = 0, midres;
        int msize = (int) std::max(data.size(), rhs.data.size()) + 1;
        data.resize(msize);
        for (int i = 0; i < msize; i++) {
            midres = (long long) data[i] + (long long) ((int) rhs.data.size() > i ? rhs.data[i] : 0) + cache;
            cache = midres / base;
            data[i] = (ul) (midres % base);
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
            long long cache = 0, midres;
            for (int i = 0; i < (int) data.size(); i++) {
                midres = (long long) data[i] - (long long) ((int) rhs.data.size() > i ? rhs.data[i] : 0) - cache;
                cache = 0;
                if (midres < 0) {
                    midres += base;
                    cache = 1;
                }
                data[i] = (ul) midres;
            }
            while (*(data.end() - 1) == 0) data.pop_back();
        }
        if (comp * sign == -1) {
            data.resize(rhs.data.size());
            long long cache = 0, midres;
            for (int i = 0; i < (int) data.size(); i++) {
                midres = ((long long) rhs.data[i] - (long long) data[i] - cache);
                cache = 0;
                if (midres < 0) {
                    midres += base;
                    cache = 1;
                }
                data[i] = (ul) midres;
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
    if (is_null((*this)) || is_null(rhs)) {
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
            res.data[i + rhs.data.size()] = (ul)cache;
        }
        *this = res;
        while (*(data.end() - 1) == 0) data.pop_back();
    }
    
    return *this;
}

big_integer fuldivsmall(big_integer left, big_integer right, bool do_div) {
    ull cache(0), divizor(right.data[0]), mres;
    for (int i = (int) left.data.size() - 1; i >= 0; i--) {
        mres = cache * left.base + left.data[i];
        left.data[i] = (ul) (mres / divizor);
        cache = mres % divizor;
    }
    if (do_div) {
        while (*(left.data.end() - 1) == 0) left.data.pop_back();
        return left;
    } else {
        right.data[0] = (ul) cache;
        return right;
    }
}

big_integer fuldiv(big_integer left, big_integer right, bool do_div)
{
    int divsgn = left.sign * right.sign, modsgn = left.sign;
    right = absolute(right);
    left = absolute(left);
    if (compare(left, right) == -1) {
        if (do_div) {
            return big_integer(0);
        } else {
            left.sign = modsgn;
            return left;
        }
    }
    if (right.data.size() == 1) {
        if (right.data[0] == 1) {
            if (do_div) {
                left.sign = divsgn;
                return left;
            } else {
                return big_integer(0);
            }
        } else {
            if (do_div) {
                left = fuldivsmall(left, right, do_div);
                left.sign = divsgn;
                return left;
            } else {
                right = fuldivsmall(left, right, do_div);
                right.sign = (is_null(right) ? 1 : modsgn);
                return right;
            }
        }
    }
    big_integer ONE(1), lb(0), rb(left), m, ans;
    long long cache, newcache;
    long long adder = m.base >> 2;
    while (rb - lb > ONE) {
        m = (rb + lb);
        cache = 0;
        for (int i = (int) m.data.size() - 1; i >= 0; i--) {
            newcache = m.data[i] % 2;
            m.data[i] /= 2;
            m.data[i] += (2 * adder) * cache;
            cache = newcache;
        }
        while (*(m.data.end() - 1) == 0) m.data.pop_back();
        if (right * m > left) {
            rb = m;
        } else {
            lb = m;
        }
    }
    if (do_div) {
        ans = lb;
        ans.sign = divsgn;
    } else {
        ans = left - lb * right;
        ans.sign = (is_null(ans) ? 1 : modsgn);
    }
    return ans;
}

big_integer& big_integer::operator/=(big_integer const& rhs)
{
    return *this = fuldiv(*this, rhs, true);
}

big_integer& big_integer::operator%=(big_integer const& rhs)
{
    return *this = fuldiv(*this, rhs, false);
}

big_integer shapeshift(big_integer a) {
    for (int i = (int) a.data.size() - 1; i >= 0; i--) {
        a.data[i] = ~a.data[i];
    }
    if (a.sign == 1) {
        ++a;
        a.sign = -a.sign;
    } else {
        a.sign = -a.sign;
        ++a;
    }
    return a;
}

big_integer& big_integer::operator&=(big_integer const& rhs)
{
    if (sign == -1) {
        *this = shapeshift(*this);
    }
    big_integer res;
    if (rhs.sign == -1) res = shapeshift(rhs);
    else res = rhs;
    if (data.size() < res.data.size()) data.resize(res.data.size());
    if (res.data.size() < data.size()) res.data.resize(data.size());
    for (int i = (int) data.size() - 1; i >= 0; i--) data[i] &= res.data[i];
    if (data[data.size() - 1] >= (base / 2)) *this = shapeshift(*this);
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs)
{
    if (sign == -1) {
        *this = shapeshift(*this);
    }
    big_integer res;
    if (rhs.sign == -1) res = shapeshift(rhs);
    else res = rhs;
    if (data.size() < res.data.size()) data.resize(res.data.size());
    if (res.data.size() < data.size()) res.data.resize(data.size());
    for (int i = (int) data.size() - 1; i >= 0; i--) data[i] |= res.data[i];
    if (data[data.size() - 1] >= (base / 2)) *this = shapeshift(*this);
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs)
{
    if (sign == -1) {
        *this = shapeshift(*this);
    }
    big_integer res;
    if (rhs.sign == -1) res = shapeshift(rhs);
    else res = rhs;
    if (data.size() < res.data.size()) data.resize(res.data.size());
    if (res.data.size() < data.size()) res.data.resize(data.size());
    for (int i = (int) data.size() - 1; i >= 0; i--) data[i] ^= res.data[i];
    if (data[data.size() - 1] >= (base / 2)) *this = shapeshift(*this);
    return *this;
}

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

big_integer big_integer::operator+() const
{
    return *this;
}

big_integer big_integer::operator-() const
{
    big_integer r = *this;
    if (!(r.data.size() == 1 && r.data[0] == 0)) {
        r.sign = -r.sign;
    }
    return r;
}

big_integer big_integer::operator~() const
{
    big_integer a = *this;
    a = -a;
    --a;
    return a;
}

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

big_integer operator&(big_integer a, big_integer const& b)
{
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b)
{
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b)
{
    return a ^= b;
}

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
    big_integer bmul(1000000000);
    if (is_null(divizor)) {
        res = "0";
        sign = "";
    }
    while (!is_null(divizor)) {
        someres = divizor % bmul;
        divizor /= bmul;
        someress = std::to_string(someres.data[0]);
        if (!is_null(divizor)) while (someress.length() < 9) someress = "0" + someress;
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


int main() {
    string a, b;
    cin >> a >> b;
    cout << big_integer(a) / big_integer(b) << endl;
}

