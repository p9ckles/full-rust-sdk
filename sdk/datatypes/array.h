#pragma once

#include "include.h"

struct transform_access_read {
    uint64_t transform_data;
};

struct transform_data {
    uint64_t transform_array;
    uint64_t transform_indices;
};

struct types {
    __m128 first;
    __m128 second;
    __m128 third;
};

struct dynamic_array {
    uint64_t base;
    uint64_t id;
    uint64_t sz;
    uint64_t cap;
};