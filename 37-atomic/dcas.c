


#include <stdatomic.h>


struct Data64 {
    union {
        struct d32 {
            atomic_int l;
            atomic_int h;
        } _d32;
        struct d64 {
            atomic_long d;
        } _d64;
    } _data;
};

int main() {
    struct Data64 data;
    long expected = __c11_atomic_load(&data._data._d64.d, __ATOMIC_RELAXED);
    long update = 10;
    /*
     * 
     * https://en.cppreference.com/w/c/atomic/atomic_compare_exchange
     * _Bool atomic_compare_exchange_strong_explicit( volatile A* obj,
                                               C* expected, C desired,
                                               memory_order succ,
                                               memory_order fail );
     */
    __c11_atomic_compare_exchange_strong(&data._data._d64.d, &expected, update, __ATOMIC_ACQUIRE,  __ATOMIC_RELAXED);
}

