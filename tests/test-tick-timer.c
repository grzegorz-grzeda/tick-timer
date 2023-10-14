/*
 * MIT License
 *
 * Copyright (c) 2023 Grzegorz Grzęda
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include "cmocka.h"

#include "tick-timer.h"
void* __real_calloc(size_t number_of_elements, size_t element_size);
void __real_free(void* ptr);

void* __wrap_calloc(size_t number_of_elements, size_t element_size) {
    return __real_calloc(number_of_elements, element_size);
}

void __wrap_free(void* ptr) {
    __real_free(ptr);
}

static void test_dummy(void** state) {}

static void test_invalid_create_tick_timer(void** state) {
    tick_timer_t* timer = tick_timer_create(0);
    assert_ptr_equal(timer, NULL);
}

static void test_create_destroy_tick_timer(void** state) {
    tick_timer_t* timer = tick_timer_create(10);
    assert_ptr_not_equal(timer, NULL);
    tick_timer_destroy(timer);
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_invalid_create_tick_timer),
        cmocka_unit_test(test_create_destroy_tick_timer),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}