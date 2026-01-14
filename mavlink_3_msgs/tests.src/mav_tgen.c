/*
 * mav_tgen.c - Generate MAVLink test messages
 *
 * Ben Cavanagh
 * 10-19-2025
 * Description: Generates passing and failing tests
 *              ./mav_tgen <ver> <msgID>
 *
 *              Fails mean bad length field, header
 *              flags, or values outside of enumeration
 */

#include "mav_tgen.h"

int main(int argc, char *argv[]) {
    // -------------------------------------------------------------------
    // ARG CHECKING
    // -------------------------------------------------------------------
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <msg_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Check currently supported messages
    int supported[] = {MAVLINK_MSG_ID_HEARTBEAT, MAVLINK_MSG_ID_SYS_STATUS,
                       MAVLINK_MSG_ID_VFR_HUD};
    int num_supported = sizeof(supported)/sizeof(int);

    int msg_id = atoi(argv[1]);
    int i; bool valid = false;
    for (i=0; i < num_supported; i++) {
        if (msg_id == supported[i]) {
            valid = true;
        }
    }
    if (!valid) {
        fprintf(stderr, "Unsupported msgID. Usage: %s <msg_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    #ifdef MAKEFILE_SET_V1
    int ver = 1;
    #else
    int ver = 2;
    #endif

    // -------------------------------------------------------------------
    // INITIALIZE MESSAGE STRUCT AND HEADERS
    // -------------------------------------------------------------------
    mavlink_message_t message;
    int pass_ct = 0; int fail_ct = 0;

    // HEADER FLAGS
    // stx   --> auto set
    // len   --> auto set
    // inc   --> auto set w/ or w/o signature
    // cmp   --> any byte
    // seq   --> any byte
    // sysID --> nonzero
    uint8_t system_id = 239; // 0xEF
    // compID --> nonzero (https://mavlink.io/en/messages/common.html#MAV_COMPONENT)
    uint8_t comp_id = MAV_TYPE_GENERIC_MULTIROTOR; // 0x2B
    // msgID --> cli arg, see above for supported

    // -------------------------------------------------------------------
    // MESSAGE SIGNING SETUP (MAVLink v2)
    // -------------------------------------------------------------------
    // To add a signature, simply pass the &signing pointer to fuzz()
    #ifndef MAKEFILE_SET_V1
    uint8_t secret_key[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    mavlink_signing_t signing;
    memset(&signing, 0, sizeof(signing));
    memcpy(signing.secret_key, secret_key, 32);

    signing.link_id = (uint8_t)MAVLINK_COMM_0;
    signing.timestamp = (uint64_t)0;
    signing.flags = MAVLINK_SIGNING_FLAG_SIGN_OUTGOING;
    #else
    // MAVLink v1 can't have signatures but this placeholder prevents errors
    mavlink_signing_t signing;
    #endif

    // -------------------------------------------------------------------
    // HEARTBEAT (0) TESTS
    // -------------------------------------------------------------------
    if (msg_id == MAVLINK_MSG_ID_HEARTBEAT) {
        /* PAYLOAD (in serialization order)
         * uint32_t custom_mode;            --> 0 to UINT32_MAX
         * uint8_t type;                    --> 0 to 49
         * uint8_t autopilot;               --> 0 to 20
         * uint8_t base_mode;               --> 0 to UINT8_MAX
         * uint8_t system_status;           --> 0 to 8
         * uint8_t mavlink_version;         --> auto set
         */

        mavlink_heartbeat_t payload = {0, 0, 0, 0, 0};
        fuzz_args_t args = {system_id, comp_id, &message, &payload,
            (void(*)(uint8_t, uint8_t, mavlink_message_t*, void*))mavlink_msg_heartbeat_encode,
            ver, "hb", &pass_ct, &fail_ct};

        // ---------------------------------------------------------------

        fuzz(PASSING, 0, UINT32_MAX, 16777215, (void *)&payload.custom_mode, UINT32_T, NULL, &args);
        fuzz(PASSING, 0, 49, 1, (void *)&payload.type, UINT8_T, NULL, &args);
        fuzz(PASSING, 0, 20, 1, (void *)&payload.autopilot, UINT8_T, NULL, &args);
        fuzz(PASSING, 0, UINT8_MAX, 1, (void *)&payload.base_mode, UINT8_T, &signing, &args);
        fuzz(PASSING, 0, 8, 1, (void *)&payload.system_status, UINT8_T, &signing, &args);

        // ---------------------------------------------------------------

        fuzz(FAILING, 50, UINT8_MAX, 5, (void *)&payload.type, UINT8_T, NULL, &args);
        fuzz(RESET, 0, 1, 1, (void *)&payload.type, UINT8_T, NULL, &args);
        fuzz(FAILING, 21, UINT8_MAX, 5, (void *)&payload.autopilot, UINT8_T, NULL, &args);
        fuzz(RESET, 0, 1, 1, (void *)&payload.autopilot, UINT8_T, NULL, &args);
        fuzz(FAILING, 9, UINT8_MAX, 5, (void *)&payload.system_status, UINT8_T, &signing, &args);

    // -------------------------------------------------------------------
    // SYS_STATUS (1) TESTS
    // -------------------------------------------------------------------
    } else if (msg_id == MAVLINK_MSG_ID_SYS_STATUS) {
        /* PAYLOAD (in serialization order)
         * uint32_t onboard_control_sensors_present                 // 0 to UINT32_MAX
         * uint32_t onboard_control_sensors_enabled                 // 0 to UINT32_MAX
         * uint32_t onboard_control_sensors_health                  // 0 to UINT32_MAX
         * uint16_t load                                            // 0 to 1000
         * uint16_t voltage_battery                                 // 0 to UINT16_MAX
         * int16_t current_battery                                  // INT16_MIN to INT16_MAX
         * uint16_t drop_rate_comm                                  // 0 to 10000
         * uint16_t errors_comm                                     // 0 to UINT16_MAX
         * uint16_t errors_count1                                   // 0 to UINT16_MAX
         * uint16_t errors_count2                                   // 0 to UINT16_MAX
         * uint16_t errors_count3                                   // 0 to UINT16_MAX
         * uint16_t errors_count4                                   // 0 to UINT16_MAX
         * int8_t battery_remaining                                 // 0 to 100 or -1
         */

        // MAVLink 2 supports "extension fields" so the structs differ
        // depending on the imported library being v1/v2
        #ifdef MAKEFILE_SET_V1
        mavlink_sys_status_t payload = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        #else
        mavlink_sys_status_t payload = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        #endif
        fuzz_args_t args = {system_id, comp_id, &message, &payload,
            (void(*)(uint8_t, uint8_t, mavlink_message_t*, void*))mavlink_msg_sys_status_encode,
            ver, "ss", &pass_ct, &fail_ct};

        // ---------------------------------------------------------------

        fuzz(PASSING, 0, UINT32_MAX, 8388607, (void *)&payload.onboard_control_sensors_present, UINT32_T, NULL, &args);
        fuzz(PASSING, 0, UINT32_MAX, 8388607, (void *)&payload.onboard_control_sensors_enabled, UINT32_T, NULL, &args);
        fuzz(PASSING, 0, UINT32_MAX, 8388607, (void *)&payload.onboard_control_sensors_health, UINT32_T, NULL, &args);
        fuzz(PASSING, 0, 1000, 2, (void *)&payload.load, UINT16_T, NULL, &args);
        fuzz(PASSING, 0, UINT16_MAX, 128, (void *)&payload.voltage_battery, UINT16_T, NULL, &args);
        fuzz(PASSING, INT16_MIN, INT16_MAX, 128, (void *)&payload.current_battery, INT16_T, NULL, &args);
        fuzz(PASSING, 0, 10000, 20, (void *)&payload.drop_rate_comm, UINT16_T, NULL, &args);
        fuzz(PASSING, 0, UINT16_MAX, 128, (void *)&payload.errors_comm, UINT16_T, NULL, &args);
        fuzz(PASSING, 0, UINT16_MAX, 128, (void *)&payload.errors_count1, UINT16_T, NULL, &args);
        fuzz(PASSING, 0, UINT16_MAX, 128, (void *)&payload.errors_count2, UINT16_T, NULL, &args);
        fuzz(PASSING, 0, UINT16_MAX, 128, (void *)&payload.errors_count3, UINT16_T, NULL, &args);
        fuzz(PASSING, 0, UINT16_MAX, 128, (void *)&payload.errors_count4, UINT16_T, &signing, &args);
        fuzz(PASSING, -1, 100, 1, (void *)&payload.battery_remaining, INT8_T, &signing, &args);

        // ---------------------------------------------------------------

        fuzz(FAILING, 1001, UINT16_MAX, 128, (void *)&payload.load, UINT16_T, NULL, &args);
        fuzz(RESET, 0, 1, 1, (void *)&payload.load, UINT16_T, NULL, &args);
        fuzz(FAILING, 10001, UINT16_MAX, 128, (void *)&payload.drop_rate_comm, UINT16_T, NULL, &args);
        fuzz(RESET, 0, 1, 1, (void *)&payload.drop_rate_comm, UINT16_T, NULL, &args);
        fuzz(FAILING, 101, INT8_MAX, 1, (void *)&payload.battery_remaining, INT8_T, &signing, &args);
        fuzz(FAILING, INT8_MIN, -2, 1, (void *)&payload.battery_remaining, INT8_T, &signing, &args);


    // -------------------------------------------------------------------
    // VFR_HUD (74) TESTS
    // -------------------------------------------------------------------
    } else if (msg_id == MAVLINK_MSG_ID_VFR_HUD) {
        /* PAYLOAD (in serialization order)
         * float airspeed           // float, m/s
         * float groundspeed        // float, m/s
         * float alt                // float, m/s
         * float climb              // float, m/s
         * int16_t heading          // 0 to 360
         * uint16_t throttle        // 0 to 100
         */

        mavlink_vfr_hud_t payload = {0, 0, 0, 0, 0, 0};
        fuzz_args_t args = {system_id, comp_id, &message, &payload,
            (void(*)(uint8_t, uint8_t, mavlink_message_t*, void*))mavlink_msg_vfr_hud_encode,
            ver, "vfr_hud", &pass_ct, &fail_ct};

        // ---------------------------------------------------------------

        fuzz(PASSING, 0, 10, 1, (void *)&payload.airspeed, FLOAT, NULL, &args);
        fuzz(PASSING, 0, 10, 1, (void *)&payload.groundspeed, FLOAT, NULL, &args);
        fuzz(PASSING, 0, 10, 1, (void *)&payload.alt, FLOAT, NULL, &args);
        fuzz(PASSING, 0, 10, 1, (void *)&payload.climb, FLOAT, NULL, &args);
        fuzz(PASSING, 0, 360, 1, (void *)&payload.heading, INT16_T, &signing, &args);
        fuzz(PASSING, 0, 100, 1, (void *)&payload.throttle, UINT16_T, &signing, &args);

        // ---------------------------------------------------------------

        fuzz(FAILING, INT16_MIN, -1, 128, (void *)&payload.heading, INT16_T, &signing, &args);
        fuzz(FAILING, 361, INT16_MAX, 128, (void *)&payload.heading, INT16_T, &signing, &args);
        fuzz(RESET, 0, 1, 1, (void *)&payload.heading, INT16_T, &signing, &args);
        fuzz(FAILING, 101, UINT16_MAX, 128, (void *)&payload.throttle, UINT16_T, &signing, &args);

    }

    exit(EXIT_SUCCESS);
}

// -----------------------------------------------------------------------
// HELPER FUNCTIONS
// -----------------------------------------------------------------------
// Function to fuzz a field in a message, start/end values inclusive
// Based on location in memory, goes in and changes payload field (payloads are just structs)
// Packs and writes to file
void fuzz(Mode mode, int start, int end, int step, void *field_loc, Type field_type, mavlink_signing_t *signing, fuzz_args_t *args) {
    int val = start;
    // Random float values to fuzz
    float float_tvals[] = {0.0, 1.0, 1.12351, -1.0, -1.32412,
        612.132, -234.12, FLT_MAX, -FLT_MAX, FLT_MIN};
    int num_ftvs = sizeof(float_tvals)/sizeof(float);

    while (val <= end) {
        switch (field_type) {
            case UINT8_T :
                *(uint8_t*)field_loc = (uint8_t)val;
                break;
            case UINT16_T :
                *(uint16_t*)field_loc = (uint16_t)val;
                break;
            case UINT32_T :
                *(uint32_t*)field_loc = (uint32_t)val;
                break;
            case INT8_T :
                *(int8_t*)field_loc = (int8_t)val;
                break;
            case INT16_T :
                *(int16_t*)field_loc = (int16_t)val;
                break;
            case INT32_T :
                *(int32_t*)field_loc = (int32_t)val;
                break;
            case FLOAT :
                if (val >= 0 && val < num_ftvs) {
                    *(float*)field_loc = (float)float_tvals[val];
                } else {
                    return;
                }
                break;
            default :
                break;
        }

        // Set the signing status of the default channel to add a signature/dont do so
        #ifndef MAKEFILE_SET_V1
        mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if (signing != NULL) {
            status->signing = signing;
        } else {
            status->signing = NULL;
        }
        #endif

        // Use message-specific encoding function
        args->encode_fn(args->sys_id, args->comp_id, args->message, args->struct_loc);

        if (mode != RESET) {
            // Package and write to file
            uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
            int msg_len = mavlink_msg_to_send_buffer(buffer, args->message);

            if (msg_len <= 0) {
                fprintf(stderr, "MAVLink buffer packing failed (msg_len=%d)\n", msg_len);
                return;
            }

            write_message(mode, buffer, msg_len, args->abbr, args->ver, args->pass_ct, args->fail_ct);

            if (mode == PASSING) {
                scramble(buffer, msg_len, args->abbr, args->ver, args->fail_ct);
            }
        }
        val += step;
    }
}

// -----------------------------------------------------------------------
// Function to scramble non-payload header fields to create failing tests
// To create failing tests for enumerated or bounded fields in a message
// payload, just use fuzz().
void scramble(uint8_t *buffer, int msg_len, char *abbr, int ver, int *fail_ct) {
    // Scramble magic number
    if (buffer[0] == 0xFD) {
        buffer[0] = 0xFE;
        write_message(SCRAMBLE, buffer, msg_len, abbr, ver, 0, fail_ct);
        buffer[0] = 0xFD;
    } else {
        buffer[0] = 0xFD;
        write_message(SCRAMBLE, buffer, msg_len, abbr, ver, 0, fail_ct);
        buffer[0] = 0xFE;
    }

    // Scramble payload length +/- 1
    buffer[1] += 1;
    write_message(SCRAMBLE, buffer, msg_len, abbr, ver, 0, fail_ct);
    buffer[1] -= 2;
    write_message(SCRAMBLE, buffer, msg_len, abbr, ver, 0, fail_ct);
    buffer[1] += 1;

    #ifdef MAKEFILE_SET_V1
    // Other components which could be scrambled:
    /*
     * [2] Packet sequence number
     *  --> N/A for parsers which handle messages one at a time.
     * [3] System ID
     *  --> Mission-specific
     * [4] Component ID
     *  --> Mission-specific
     * [5] Message ID
     *  --> Just changing message ID does not always guarantee
     *      an invalid message
     */

    #else
    // Scramble incompatibility flag
    if (buffer[2] == 0x00) {
        buffer[2] = 0x01;
        write_message(SCRAMBLE, buffer, msg_len, abbr, ver, 0, fail_ct);
        buffer[2] = 0x00;
    } else {
        buffer[2] = 0x00;
        write_message(SCRAMBLE, buffer, msg_len, abbr, ver, 0, fail_ct);
        buffer[2] = 0x01;
    }

    // Scramble compatibility flag
    // NOTE: MAVLink currently allows for safe ignoring of flags which are
    // not understood. Uncomment to test a parser which doesn't allow this.
    /*
    buffer[3] = 0x01;
    write_message(SCRAMBLE, buffer, msg_len, abbr, ver, 0, fail_ct);
    buffer[3] = 0x00;
    */

    // Other components which could be scrambled:
    /*
     * [4] Packet sequence number
     *  --> N/A for parsers which handle messages one at a time.
     * [5] System ID
     *  --> Mission-specific
     * [6] Component ID
     *  --> Mission-specific
     * [5] Message ID
     *  --> Just changing message ID does not always guarantee
     *      an invalid message
     */

    #endif

}

// -----------------------------------------------------------------------
// Write message to MAVLink buffer and then to file
void write_message(Mode mode, uint8_t *buffer, int msg_len, char *abbr, int ver, int *pass_ct, int *fail_ct) {
    FILE *ofile;
    char fname[100];

    if (mode == PASSING) {
        sprintf(fname, "pass.v%d.%s.%d", ver, abbr, *pass_ct);
        *pass_ct += 1;
    } else if (mode == SCRAMBLE) {
        sprintf(fname, "fail.hdr.v%d.%s.%d", ver, abbr, *fail_ct);
        *fail_ct += 1;
    } else {
        sprintf(fname, "fail.pyl.v%d.%s.%d", ver, abbr, *fail_ct);
        *fail_ct += 1;
    }

    if ((ofile = fopen(fname, "r")) != NULL) {
        fprintf(stderr, "Naming err. Overwrote existing test case %s\n", fname);
    }
    if ((ofile = fopen(fname, "wb")) == NULL) {
        fprintf(stderr, "Couldn't open %s\n", fname);
        return;
    }

    // Write exact length of message (truncate rest of buffer)
    fwrite(buffer, 1, msg_len, ofile);
    fclose(ofile);
}

