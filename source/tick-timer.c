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
#include "tick-timer.h"
#include <stdlib.h>

typedef struct tick_timer_instance {
    tick_timer_instance_handler_t handler;
    void* handler_context;
    bool is_active;
    uint32_t current_value_ticks;
    uint32_t period_ticks;
    uint32_t delay_ticks;
} tick_timer_instance_t;

typedef struct tick_timer {
    tick_timer_instance_t* instances;
    size_t instances_capacity;
    size_t instances_count;
} tick_timer_t;

static void process_instance(tick_timer_instance_t* instance) {
    if (!instance->is_active) {
        return;
    }
    if (instance->current_value_ticks > 0) {
        instance->current_value_ticks--;
        return;
    }
    if (instance->handler) {
        instance->handler(instance, instance->handler_context);
    }
    if (!instance->period_ticks) {
        instance->is_active = false;
    } else {
        instance->current_value_ticks = (instance->period_ticks - 1);
    }
}

tick_timer_t* tick_timer_create(size_t max_number_of_instances) {
    tick_timer_t* timer = calloc(1, sizeof(tick_timer_t));
    if (!timer || (max_number_of_instances == 0)) {
        return NULL;
    }
    timer->instances =
        calloc(max_number_of_instances, sizeof(tick_timer_instance_t));
    if (!timer->instances) {
        free(timer);
        return NULL;
    }
    timer->instances_capacity = max_number_of_instances;
    return timer;
}

void tick_timer_destroy(tick_timer_t* timer) {
    if (!timer) {
        return;
    }
    free(timer->instances);
    free(timer);
}

void tick_timer_tick_1ms(tick_timer_t* timer) {
    if (!timer) {
        return;
    }
    for (size_t i = 0; i < timer->instances_count; i++) {
        process_instance(timer->instances + i);
    }
}

bool tick_timer_register_instance(tick_timer_t* timer,
                                  uint32_t period_ticks,
                                  uint32_t delay_ticks,
                                  tick_timer_instance_handler_t handler,
                                  void* context) {
    if (!timer || !handler) {
        return false;
    }
    if ((timer->instances_count >= timer->instances_capacity) ||
        ((period_ticks == 0) && (delay_ticks == 0))) {
        return false;
    }
    timer->instances_count++;
    tick_timer_instance_t* instance = timer->instances + timer->instances_count;
    instance->is_active = false;
    instance->period_ticks = period_ticks;
    instance->delay_ticks = delay_ticks;
    instance->handler = handler;
    instance->handler_context = context;
    return true;
}

bool tick_timer_start_instance(tick_timer_instance_t* instance) {
    if (!instance || instance->is_active) {
        return false;
    }
    instance->current_value_ticks = instance->delay_ticks;
    instance->is_active = true;
    return true;
}

bool tick_timer_stop_instance(tick_timer_instance_t* instance) {
    if (!instance) {
        return false;
    }
    instance->is_active = false;
    return true;
}