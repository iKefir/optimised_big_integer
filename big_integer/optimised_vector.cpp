#include "optimised_vector.h"

optimised_vector::optimised_vector() {
    a = 0;
    vec_size = 0;
}

optimised_vector::~optimised_vector() {
    if (is_big()) safe_delete(b);
}

optimised_vector::optimised_vector(optimised_vector const & other) {
    if (other.is_big()) {
        b = get_copy(other.b);
    } else {
        a = other.a;
    }
    vec_size = other.vec_size;
}

optimised_vector& optimised_vector::operator=(optimised_vector const & other) {
    //MARK: need make_copy done
    if (is_big()) {
        safe_delete(b);
    }
    if (other.is_big()) {
        b = get_copy(other.b);
    } else {
        a = other.a;
    }
    vec_size = other.vec_size;
    return *this;
}

size_t optimised_vector::size() const {
    return vec_size;
}

void optimised_vector::resize(size_t new_size) {
    //MARK: need make_copy done
    if (new_size > 1) {
        if (!is_big()) {
            uint32_t cache = a;
            b = new cow_vector;
            b -> data.push_back(cache);
        }
        else b = make_copy(b);
        b->data.resize(new_size);
    } else if (is_big()) {
        uint32_t cache = b -> data[0];
        safe_delete(b);
        a = cache;
    }
    vec_size = new_size;
}

uint32_t optimised_vector::back() {
    return (vec_size > 1 ? b -> data.back() : a);
}

uint32_t& optimised_vector::operator[](size_t ind) {
    //MARK: need make_copy done
    if (is_big()) {
        b = make_copy(b);
        return b -> data[ind];
    }
    return a;
}

uint32_t const& optimised_vector::operator[](size_t ind) const {
    return (is_big() ? b -> data[ind] : a);
}

void optimised_vector::push_back(const uint32_t & buffer) {
    //MARK: need make_copy done
    if (vec_size == 0) {
        a = buffer;
    } else if (vec_size == 1) {
        int some_var = a;
        b = new cow_vector;
        b -> data.push_back(some_var);
        b -> data.push_back(buffer);
    } else {
        b = make_copy(b);
        b -> data.push_back(buffer);
    }
    ++vec_size;
}

void optimised_vector::pop_back() {
    //MARK: need make_copy done
    if (vec_size == 2) {
        uint32_t some_var = b -> data[0];
        safe_delete(b);
        a = some_var;
    } else if (vec_size > 2) {
        b = make_copy(b);
        b -> data.pop_back();
    }
    if (vec_size > 0) --vec_size;
}

bool optimised_vector::is_big() const{
    return vec_size > 1;
}

//cow optimisation

optimised_vector::cow_vector * optimised_vector::get_copy(cow_vector * b) {
    ++(b -> link_count);
    return b;
}

optimised_vector::cow_vector * optimised_vector::make_copy(cow_vector * b) {
    if (b -> link_count > 0) {
        cow_vector * a = new cow_vector();
        a -> data = b -> data;
        safe_delete(b);
        return a;
    }
    return b;
}

void optimised_vector::safe_delete(cow_vector * b) {
    if (b -> link_count > 0) --(b -> link_count);
    else delete b;
}




