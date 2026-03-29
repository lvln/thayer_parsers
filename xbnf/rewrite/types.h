/*
 * types.h - 
 *
 * Josh Meise
 * 03-29-2026
 * Description: 
 *
 */

#pragma once
#include <stdint.h>

typedef enum {
    byte,
    uint8,
    int8,
    uint16_le,
    int16_le,
    uint32_le,
    int32_le,
    uint64_le,
    int64_le,
    uint16_be,
    int16_be,
    uint32_be,
    int32_be,
    uint64_be,
    int64_be
} type;

typedef union {
    uint8_t uint8;
    int8_t int8;
    uint16_t uint16;
    int16_t int16;
    uint32_t uint32;
    int32_t int32;
    uint64_t uint64;
    int64_t int64;
} data_t;

typedef struct {
    data_t val;
    type t;
} val_t;

typedef struct {
    int num_vals;
    val_t* vals;
} enum_t;

typedef struct {
    val_t min;
    val_t max;
} range_t;

