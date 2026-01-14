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

// Create empty MAVLink signature struct for v1 to prevent function issues
#ifdef MAKEFILE_SET_V1
    typedef struct mavlink_signing {
    } mavlink_signing_t;
#endif

typedef enum {
    UINT8_T, UINT16_T, UINT32_T,
    INT8_T, INT16_T, INT32_T, FLOAT
} Type;

typedef enum {
    PASSING, FAILING, SCRAMBLE, RESET
} Mode;

typedef struct {
    uint8_t sys_id;
    uint8_t comp_id;
    mavlink_message_t *message;
    void *struct_loc;
    void (*encode_fn)(uint8_t, uint8_t, mavlink_message_t*, void*);
    int ver;
    char *abbr;
    int *pass_ct;
    int *fail_ct;
} fuzz_args_t;

void fuzz(Mode mode, int start, int end, int mult, void *field_loc, Type field_type, mavlink_signing_t *signing, fuzz_args_t *args);

void scramble(uint8_t *buffer, int msg_len, char *abbr, int ver, int *fail_ct);

void write_message(Mode mode, uint8_t *buffer, int msg_len, char *abbr, int ver, int *pass_ct, int *fail_ct);

