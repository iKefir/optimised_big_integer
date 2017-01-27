#include "big_integer.h"
#define absolute(a) (a.sign > 0 ? a : -a)
#define is_null(a) (a.data.size() == 1 && a.data[0] == 0)
#include <stdexcept>

const uint64_t base = ((ull) (1 << 30) * 4);

big_integer::big_integer()
{
    sign = 1;
    data.resize(1);
    data[0] = 0;
}

big_integer::big_integer(big_integer const& other)
{
    sign = other.sign;
    data = other.data;
}

big_integer::big_integer(int a)
{
    int64_t la = a;
    sign = 1;
    if (la < 0) {
        la = -la;
        sign = -1;
    }
    int64_t cache;
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
    int32_t start = 0;
    *this = big_integer(0);
    int32_t postsign = 1;
    std::string a = &str[0];
    if (a[0] == '-') {
        postsign = -1;
        start = 1;
    }
    int32_t cache = 0;
    big_integer TEN(100000000);
    for (size_t i = start; i < a.length(); i++) {
        cache *= 10;
        cache += a[i] - 48;
        if (i % 8 == 7) {
            *this *= TEN;
            *this += big_integer(cache);
            cache = 0;
        }
    }
    int32_t sval = a.length() % 8;
    int32_t mult = 1;
    for (int32_t i = 0; i < sval; i++) {
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
    data = other.data;
    sign = other.sign;
    
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& rhs)
{
    if (sign == rhs.sign) {
        int64_t cache = 0, midres;
        size_t msize = std::max(data.size(), rhs.data.size()) + 1;
        data.resize(msize);
        for (size_t i = 0; i < msize; i++) {
            midres = (int64_t) data[i] + (int64_t) (rhs.data.size() > i ? rhs.data[i] : 0) + cache;
            cache = midres / base;
            data[i] = (ul) (midres % base);
        }
        if (data.back() == 0) data.pop_back();
    } else {
        sign = -sign;
        *this = rhs - *this;
    }
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs)
{
    if (sign == rhs.sign) {
        int32_t comp = compare(*this, rhs);
        if (comp == 0) {
            *this = big_integer(0);
        }
        if (comp * sign == 1) {
            int64_t cache = 0, midres;
            for (size_t i = 0; i < data.size(); i++) {
                midres = (int64_t) data[i] - (int64_t) (rhs.data.size() > i ? rhs.data[i] : 0) - cache;
                cache = 0;
                if (midres < 0) {
                    midres += base;
                    cache = 1;
                }
                data[i] = (ul) midres;
            }
            while (data.back() == 0) data.pop_back();
        }
        if (comp * sign == -1) {
            data.resize(rhs.data.size());
            int64_t cache = 0, midres;
            for (size_t i = 0; i < data.size(); i++) {
                midres = ((int64_t) rhs.data[i] - (int64_t) data[i] - cache);
                cache = 0;
                if (midres < 0) {
                    midres += base;
                    cache = 1;
                }
                data[i] = (ul) midres;
            }
            while (data.back() == 0) data.pop_back();
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
        ull cache, mres;
        res.data.resize(data.size() + rhs.data.size());
        for (size_t i = 0; i < data.size(); i++) {
            cache = 0;
            for (int32_t j = 0; j < rhs.data.size(); j++) {
                mres = (ull) res.data[i + j] + (ull) data[i] * (ull) rhs.data[j] + cache;
                cache = mres / base;
                res.data[i + j] = (ul) (mres % base);
            }
            res.data[i + rhs.data.size()] = (ul)cache;
        }
        *this = res;
        while (data.back() == 0) data.pop_back();
    }
    
    return *this;
}

big_integer fuldivsmall(big_integer left, big_integer right, bool do_div) {
    ull cache(0), divizor(right.data[0]), mres;
    for (int32_t i = (int) left.data.size() - 1; i >= 0; i--) {
        mres = cache * base + left.data[i];
        left.data[i] = (ul) (mres / divizor);
        cache = mres % divizor;
    }
    if (do_div) {
        while (left.data.back() == 0) left.data.pop_back();
        return left;
    } else {
        right.data[0] = (ul) cache;
        return right;
    }
}

big_integer fuldiv(big_integer u, big_integer right, bool do_div)
{
    int32_t divsgn = u.sign * right.sign, modsgn = u.sign;
    right = absolute(right);
    u = absolute(u);
    if (compare(u, right) == -1) {
        if (do_div) {
            return big_integer(0);
        } else {
            u.sign = modsgn;
            return u;
        }
    }
    if (right.data.size() == 1) {
        if (right.data[0] == 1) {
            if (do_div) {
                u.sign = divsgn;
                return u;
            } else {
                return big_integer(0);
            }
        } else {
            if (do_div) {
                u = fuldivsmall(u, right, do_div);
                u.sign = divsgn;
                return u;
            } else {
                right = fuldivsmall(u, right, do_div);
                right.sign = (is_null(right) ? 1 : modsgn);
                return right;
            }
        }
    }
    big_integer q;
    int64_t n = right.data.size(), m = u.data.size() - right.data.size(), uJ, vJ, i, t3;
    ull guess, grem, carry, borrow, t1, t2, t;
    ull scale;
    q.data.resize(m + 1);
    scale = base / (right.data[n - 1] + 1);
    if (scale > 1) {
        u *= scale;
        right *= scale;
    }
    u.data.resize(u.data.size() + 1);
    for (vJ = m, uJ = n + vJ; vJ >= 0; --vJ, --uJ) {
        guess = (u.data[uJ] * base + u.data[uJ - 1]) / right.data[n - 1];
        grem =  (u.data[uJ] * base + u.data[uJ - 1]) % right.data[n - 1];
        
        while (grem < base) {
            t2 = guess * right.data[n - 2];
            t1 = grem * base + u.data[uJ - 2];
            
            if ((t2 > t1) || (guess == base)) {
                --guess;
                grem += right.data[n - 1];
            } else break;
        }
        
        carry = 0;
        borrow = 0;
        
        for (i = 0; i < n; i++) {
            t1 = guess * right.data[i] + carry;
            carry = t1 / base;
            t1 -= carry * base;
            
            t3 = u.data[vJ + i] - t1 - borrow;
            if (t3 < 0) {
                u.data[vJ + i] = (ul) (t3 + base);
                borrow = 1;
            } else {
                u.data[vJ + i] = (ul) t3;
                borrow = 0;
            }
        }
        
        t3 = u.data[vJ + i] - carry - borrow;
        if (t3 < 0) {
            u.data[vJ + i] = (ul) (t3 + base);
            borrow = 1;
        } else {
            u.data[vJ + i] = (ul) t3;
            borrow = 0;
        }
        
        if (borrow == 0) {
            q.data[vJ] = (ul) guess;
        } else {
            q.data[vJ] = (ul) (guess - 1);
            
            carry = 0;
            for (i = 0; i < n; i++) {
                t = u.data[vJ + i] + right.data[i] + carry;
                if (t >= base) {
                    u.data[vJ + i] = (ul) (t - base);
                    carry = 1;
                } else {
                    u.data[vJ + i] = (ul) t;
                    carry = 0;
                }
            }
            u.data[vJ + i] = (ul) (u.data[vJ + i] + carry - base);
        }
        
        i = u.data.size() - 1;
        while ((i > 0) && (u.data[i] == 0)) i--;
        u.data.resize(i + 1);
    }
    
    while ((m > 0) && (q.data[m] == 0)) m--;
    q.data.resize(m + 1);
    
    if (do_div) {
        q.sign = divsgn;
        return q;
    } else {
        u.sign = modsgn;
        if (scale > 1) {
            return u / scale;
        } else return u;
    }
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
    for (int32_t i = (int) a.data.size() - 1; i >= 0; i--) {
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
    big_integer res(rhs);
    if (data.size() > res.data.size()) {
        data.resize(data.size() + 1);
        res.data.resize(data.size());
    }
    else if (res.data.size() > data.size()) {
        res.data.resize(res.data.size() + 1);
        data.resize(res.data.size());
    } else {
        data.resize(data.size() + 1);
        res.data.resize(data.size());
    }
    
    if (rhs.sign == -1) res = shapeshift(res);
    if (sign == -1) *this = shapeshift(*this);
    
    for (int32_t i = (int) data.size() - 1; i >= 0; i--) data[i] &= res.data[i];
    if (data[data.size() - 1] >= (base / 2)) *this = shapeshift(*this);
    while (data.back() == 0 && data.size() > 1) data.pop_back();
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs)
{
    big_integer res(rhs);
    if (data.size() > res.data.size()) {
        data.resize(data.size() + 1);
        res.data.resize(data.size());
    }
    else if (res.data.size() > data.size()) {
        res.data.resize(res.data.size() + 1);
        data.resize(res.data.size());
    } else {
        data.resize(data.size() + 1);
        res.data.resize(data.size());
    }
    
    if (rhs.sign == -1) res = shapeshift(res);
    if (sign == -1) *this = shapeshift(*this);
    
    for (int32_t i = (int) data.size() - 1; i >= 0; i--) data[i] |= res.data[i];
    if (data[data.size() - 1] >= (base / 2)) *this = shapeshift(*this);
    while (data.back() == 0 && data.size() > 1) data.pop_back();
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs)
{
    big_integer res(rhs);
    if (data.size() > res.data.size()) {
        data.resize(data.size() + 1);
        res.data.resize(data.size());
    }
    else if (res.data.size() > data.size()) {
        res.data.resize(res.data.size() + 1);
        data.resize(res.data.size());
    } else {
        data.resize(data.size() + 1);
        res.data.resize(data.size());
    }
    
    if (rhs.sign == -1) res = shapeshift(res);
    if (sign == -1) *this = shapeshift(*this);

    for (int32_t i = (int) data.size() - 1; i >= 0; i--) data[i] ^= res.data[i];
    if (data[data.size() - 1] >= (base / 2)) *this = shapeshift(*this);
    while (data.back() == 0 && data.size() > 1) data.pop_back();
    return *this;
}

big_integer& big_integer::operator<<=(int rhs)
{
    int32_t blamount = rhs / 32;
    int32_t bitamount = rhs % 32;
    int64_t divizor = 1, multiplier = 1;
    for (int32_t i = 0; i < bitamount; i++) divizor *= 2;
    for (int32_t i = 0; i < (32 - bitamount); i++) multiplier *= 2;
    int64_t cache = 0, newcache;
    for (int32_t i = 0; i < (int) data.size(); ++i) {
        newcache = data[i] / multiplier;
        data[i] = (ul) ((data[i] * divizor) + cache);
        cache = newcache;
    }
    data.resize(data.size() + blamount);
    if (blamount != 0) for (int32_t i = (int) data.size() - blamount - 1; i >= 0; --i) {
        data[i + blamount] = data[i];
        data[i] = 0;
    }
    while (data.back() == 0 && data.size() > 1) data.pop_back();
    return *this;
}

big_integer& big_integer::operator>>=(int rhs)
{
    bool wasneg = false;
    if (sign < 0) {
        wasneg = true;
        *this += 1;
    }
    int32_t blamount = rhs / 32;
    int32_t bitamount = rhs % 32;
    int64_t divizor = 1, multiplier = 1;
    for (int32_t i = 0; i < bitamount; i++) divizor *= 2;
    for (int32_t i = 0; i < (32 - bitamount); i++) multiplier *= 2;
    int64_t cache = 0, newcache;
    for (int32_t i = (int) data.size() - 1; i >= 0; --i) {
        newcache = data[i] % divizor;
        data[i] = (ul) ((data[i] / divizor) + cache * multiplier) % base;
        cache = newcache;
    }
    if (blamount != 0) for (int32_t i = blamount; i < (int) data.size(); ++i) {
        data[i - blamount] = data[i];
    }
    for (int32_t i = (int) data.size() - blamount; i < (int) data.size(); ++i) data[i] = 0;
    while (data.back() == 0 && data.size() > 1) data.pop_back();
    if (wasneg) {
        *this -= 1;
    }
    return *this;
}


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

big_integer operator<<(big_integer a, int b)
{
    return a <<= b;
}

big_integer operator>>(big_integer a, int b)
{
    return a >>= b;
}

int32_t compare(big_integer const& a, big_integer const& b) {
    if (a.sign > b.sign) return 1;
    if (a.sign < b.sign) return -1;
    if (a.data.size() > b.data.size()) return a.sign;
    if (a.data.size() < b.data.size()) return -a.sign;
    for (int32_t i = (int) a.data.size() - 1; i >= 0; i--) {
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

