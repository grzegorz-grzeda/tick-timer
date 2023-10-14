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
#ifndef TICK_TIMER_H
#define TICK_TIMER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @defgroup tick_timer Tick timer
 * @brief Small yet efficient tick-timer library in C
 * @{
 */

/**
 * @brief Tick timer structure declaration
 *
 * The timer object is referenced only by pointers, so the user does not need to
 * know the internals of this object.
 */
typedef struct tick_timer tick_timer_t;

/**
 * @brief Tick timer instance declaration
 *
 * The timer instance object is referenced only by pointers, so the user does
 * not need to know the internals of this object.
 */
typedef struct tick_timer_instance tick_timer_instance_t;

/**
 * @brief Tick timer instance timeout handler
 *
 * @param[in] instance pointer to the @ref tick_timer_instance_t structure
 */
typedef void (*tick_timer_instance_handler_t)(tick_timer_instance_t* instance,
                                              void* context);

tick_timer_t* tick_timer_create(size_t max_number_of_instances);

void tick_timer_destroy(tick_timer_t* timer);

void tick_timer_tick_1ms(tick_timer_t* timer);

bool tick_timer_register_instance(tick_timer_t* timer,
                                  uint32_t period_ms,
                                  uint32_t delay_ms,
                                  tick_timer_instance_handler_t handler,
                                  void* context);

bool tick_timer_start_instance(tick_timer_instance_t* instance);

bool tick_timer_stop_instance(tick_timer_instance_t* instance);

/**
 * @}
 */
#endif  // TICK_TIMER_H