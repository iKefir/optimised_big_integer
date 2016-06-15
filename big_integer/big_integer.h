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
    
        big_integer& operator<<=(int rhs);
        big_integer& operator>>=(int rhs);
    
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

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

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
