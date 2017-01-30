#ifndef optimized_vector_h
#define optimized_vector_h

#include <cstdint>
#include <vector>
#include <stdio.h>

struct optimised_vector {
    
    optimised_vector();
    ~optimised_vector();
    
    optimised_vector(optimised_vector const &);
    optimised_vector& operator=(optimised_vector const &);
    size_t size() const;
    void resize(size_t);
    uint32_t& operator[](size_t);
    uint32_t const& operator[](size_t) const;
    void push_back(const uint32_t &);
    void pop_back();
    uint32_t back();
    
    bool is_big() const;
    
    //cow optimisation
    struct cow_vector {
        std::vector<uint32_t> data;
        uint32_t link_count;
    };
    cow_vector * get_copy(cow_vector *);
    cow_vector * make_copy(cow_vector *);
    void safe_delete(cow_vector *);
    
private:
    union {
        uint32_t a;
        cow_vector * b;
    };
    size_t vec_size;
};

#endif /* optimised_vector_h */
