#include <stdio.h>
#include <assert.h>

#include "util/vector.h"
#include "util/_macro_vararg.h"

#define NDATA 15

/* Vector test suite (-Grissess) */

/* Make a test permutation [0, NDATA) */
int make_data(vector *v) {
    size_t i;
    int *datum;
    vec_init(v);
    for(i = 0; i < NDATA; i++) {
        datum = malloc(sizeof(int));
        assert(datum);
        *datum = i;
        vec_insert(v, i, datum);
    }
    return v->len == NDATA;
}

void free_data(vector *v) {
    vec_foreach(v, (vec_iter_f) free);
}

void print_out(int *a) {
    printf("%d ", *a);
}

int *mul_three(int *a) {
    int *ret = malloc(sizeof(int));
    assert(ret);
    *ret = *a * 3;
    return ret;
}

int *sum(int *accum, int *elem) {
    int *ret = malloc(sizeof(int));
    assert(ret);
    *ret = *accum + *elem;
    free(accum);
    return ret;
}

int is_ident_permuted(void *ud, int *elem, vector *v, size_t idx) {
    if(*elem == idx) return 0;
    return 1;
}

int int_equal(int *a, int *b) {
    return *a == *b;
}

int ptrs_equal(void *a, void *b) {
    return ((size_t) a) == ((size_t) b);
}

int *identity(int *a) {
    int *ret = malloc(sizeof(int));
    assert(ret);
    *ret = *a;
    return ret;
}

/* Test cases */
/* Note that a lot of these contain boilerplate (like the cleanup section at the end).
 * That's not an expected use case; that'll only happen when you free the stuff. The
 * reason it's here is because each of these test cases is totally standalone and
 * operates on at least one new, fresh vector--I'm looking to exercise malloc() and
 * friends with this test as well, mostly just to give valgrind a workout :P .
 */

#define expect(cond) do {if(!(cond)) return 0;} while(0)
#define expect_msg(cond, ...) do {if(!(cond)) {fprintf(stderr, __VA_ARGS__); return 0;}} while(0)
#define pass() do {return 1;} while(0)

int test_empty() {
    vector v;
    vec_init(&v);
    expect_msg(v.len == 0, "Expected length 0, got %ld\n", v.len);
    expect_msg(v.cap == 0, "Expected cap 0, got %ld\n", v.cap);
    expect_msg(v.buf == NULL, "Expected a NULL buffer, got %p\n", v.buf);
    pass();
}

int test_data() {
    vector v;
    expect_msg(make_data(&v), "make_data failed\n");
    expect_msg(v.len == NDATA, "Expected length %d, got %ld\n", NDATA, v.len);
    printf("Following should be a identity permutation from 0 to %d inclusive:\n", NDATA-1);
    vec_foreach(&v, (vec_iter_f) print_out);
    printf("\nIf that wasn't the case, it's broken, and I don't know what to tell you :)\n");
    free_data(&v);
    vec_clear(&v);
    expect_msg(v.len == 0, "Expected length 0, got %ld\n", v.len);
    expect_msg(v.cap == 0, "Expected cap 0, got %ld\n", v.cap);
    expect_msg(v.buf == NULL, "Expected a NULL buffer, got %p\n", v.buf);
    pass();
}

int test_remove() {
    vector v;
    size_t lastlen;
    int *elem, *popped;
    expect(make_data(&v));
    free(vec_remove(&v, 0));
    expect(v.len == NDATA - 1);
    expect(*vec_get(&v, 0, int) == 1);
    free(vec_remove(&v, v.len - 1));
    expect(*vec_get(&v, 0, int) == 1);
    expect(v.len == NDATA - 2);
    lastlen = v.len;
    while(v.len > 0) {
        elem = vec_get(&v, 0, int);
        popped = (int *) vec_remove(&v, 0);
        expect(elem == popped);
        if(v.len > 0) expect(*vec_get(&v, 0, int) == *elem + 1);
        expect(v.len == lastlen - 1);
        lastlen = v.len;
    }
    free_data(&v);
    vec_clear(&v);
    pass();
}

int test_alloc() {
    vector v;
    size_t i;
    int *elem;
    vec_init(&v);
    vec_alloc(&v, 2*NDATA);
    expect_msg(v.cap == 2*NDATA, "vec_alloc: Expected cap %d, got %ld\n", 2*NDATA, v.cap);
    for(i = 0; i < NDATA; i++) {
        elem = malloc(sizeof(int));
        *elem = i;
        vec_insert(&v, v.len, elem);
    }
    expect_msg(v.cap == 2*NDATA, "vec_insert: Expected cap %d, got %ld\n", 2*NDATA, v.cap);
    expect_msg(v.len == NDATA, "vec_insert: Expected len %d, got %ld\n", NDATA, v.len);
    free_data(&v);
    vec_clear(&v);
    pass();
}

int test_map() {
    vector v, vmap;
    size_t i;
    expect_msg(make_data(&v), "make_data failed\n");
    vec_init(&vmap);
    vec_map(&v, &vmap, (vec_map_f) mul_three, NULL);
    expect_msg(v.len == vmap.len, "Expected length %ld, got %ld\n", v.len, vmap.len);
    for(i = 0; i < v.len; i++) {
        expect_msg(*vec_get(&vmap, i, int) == 3 * *vec_get(&v, i, int), "Element %ld, value %d -- expected %d\n", i, *vec_get(&vmap, i, int), 3 * *vec_get(&v, i, int));
    }
    free_data(&v);
    free_data(&vmap);
    vec_clear(&v);
    vec_clear(&vmap);
    pass();
}

int test_reduce() {
    vector v;
    int *datum = malloc(sizeof(int));
    assert(datum);
    *datum = 0;
    expect_msg(make_data(&v), "make_data failed\n");
    datum = (int *) vec_reduce(&v, (vec_reduce_f) sum, datum); /* NB: *init is consumed by the free in reduce (= sum) */
    /* Who'da thunk triangular numbers would be useful :P */
    expect_msg(*datum == NDATA * (NDATA - 1) / 2, "Expected result %d, got %d\n", NDATA * (NDATA - 1) / 2, *datum);
    free(datum);
    free_data(&v);
    vec_clear(&v);
    pass();
}

int test_test() {
    vector v, vmap;
    vec_init(&vmap);
    expect(make_data(&v));
    expect(!vec_test(&v, (vec_test_f) is_ident_permuted, NULL));
    vec_map(&v, &vmap, (vec_map_f) mul_three, NULL);
    expect(1 == vec_test(&vmap, (vec_test_f) is_ident_permuted, NULL));
    free_data(&v);
    free_data(&vmap);
    vec_clear(&v);
    vec_clear(&vmap);
    pass();
}

int test_equal() {
    vector v, v2, vmap;
    vec_init(&vmap);
    expect(make_data(&v));
    expect(make_data(&v2));
    vec_map(&v, &vmap, (vec_map_f) mul_three, NULL);
    expect(vec_equal(&v, &v2, (vec_eq_f) int_equal));
    expect(!vec_equal(&v, &vmap, (vec_eq_f) int_equal));
    expect(!vec_equal(&v2, &vmap, (vec_eq_f) int_equal));
    free_data(&v);
    free_data(&v2);
    free_data(&vmap);
    vec_clear(&v);
    vec_clear(&v2);
    vec_clear(&vmap);
    pass();
}

int test_search() {
    vector v;
    expect(make_data(&v));
    ssize_t i;
    for(i = 0; i < v.len; i++) {
        expect(i == vec_search(&v, vec_get(&v, i)));
    }
    free_data(&v);
    vec_clear(&v);
    pass();
}

int test_copy() {
    vector v, v2;
    vec_init(&v2);
    expect_msg(make_data(&v), "make_data failed\n");
    vec_copy(&v, &v2); /* NB: Copies pointers verbatim--might lead to reference difficulties. */
    expect_msg(v.len == v2.len, "direct copy: Expected length %ld, got %ld\n", v.len, v2.len);
    expect_msg(vec_equal(&v, &v2, (vec_eq_f) int_equal), "direct copy: integers not equal\n");
    expect_msg(vec_equal(&v, &v2, (vec_eq_f) ptrs_equal), "direct copy: pointers not equal\n");
    /* Not even going to bother to clear it--that would dangle some pointers :P */
    vec_map(&v, &v2, (vec_map_f) identity, NULL); /* The "right" way to do it to maintain single-user data */
    expect_msg(v.len == v2.len, "map copy: Expected length %ld, got %ld\n", v.len, v2.len);
    expect_msg(vec_equal(&v, &v2, (vec_eq_f) int_equal), "map copy: integers not equal\n");
    expect_msg(!vec_equal(&v, &v2, (vec_eq_f) ptrs_equal), "map copy: references shared\n");
    /* However, we're responsible for cleanup this time */
    free_data(&v);
    free_data(&v2);
    vec_clear(&v);
    vec_clear(&v2);
    pass();
}

/* Main function */

int main() {
    assert(test_empty());
    assert(test_data());
    assert(test_remove());
    assert(test_alloc());
    assert(test_map());
    assert(test_reduce());
    assert(test_test());
    assert(test_equal());
    assert(test_search());
    assert(test_copy());
    return 0;
}
