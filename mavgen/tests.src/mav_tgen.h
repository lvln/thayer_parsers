/*
 * mav_tgen.h - Generate MAVLink test messages
 *
 * Ben Cavanagh
 * 10-19-2025
 * Description: Generates passing and failing tests
 *              ./mav_tgen <ver> <msgID>
 *
 *              Fails mean bad length field, header
 *              flags, or values outside of enumeration
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <common/mavlink.h> // Either v1 or v2 as decided by Makefile

enum Types {
    UINT8_T, UINT16_T, UINT32_T,
    INT8_T, INT16_T, INT32_T, FLOAT
};

typedef struct {
    uint8_t sys_id;
    uint8_t comp_id;
    mavlink_message_t *message;
    void *struct_loc;
    void (*encode_fn)(uint8_t, uint8_t, mavlink_message_t*, void*);
    int ver;
    char *abbr;
    int *ct;
} fuzz_args_t;

void fuzz(int start, int end, int mult, void *field_loc, int field_type, fuzz_args_t *args);

void write_message(mavlink_message_t *msg, char *abbrev, int ver, int ct);

