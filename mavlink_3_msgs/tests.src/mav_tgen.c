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
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ver> <msg_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Check version number
    int ver = atoi(argv[1]);
    if (ver != 1 && ver != 2) {
        fprintf(stderr, "Bad version. Usage: %s <ver> <msg_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Check currently supported messages
    int supported[] = {MAVLINK_MSG_ID_HEARTBEAT, MAVLINK_MSG_ID_SYS_STATUS,
                       MAVLINK_MSG_ID_VFR_HUD};
    int num_supported = sizeof(supported)/sizeof(int);

    int msg_id = atoi(argv[2]);
    int i; bool valid = false;
    for (i=0; i < num_supported; i++) {
        if (msg_id == supported[i]) {
            valid = true;
        }
    }
    if (!valid) {
        fprintf(stderr, "Unsupported msgID. Usage: %s <ver> <msg_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // -------------------------------------------------------------------
    // INITIALIZE MESSAGE STRUCT AND HEADERS
    // -------------------------------------------------------------------
    mavlink_message_t message;
    int ct = 0;

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
    // HEARTBEAT (0) TESTS
    // -------------------------------------------------------------------
    if (msg_id == MAVLINK_MSG_ID_HEARTBEAT) {
        /* PAYLOAD (in serialization order)
         * uint32_t custom_mode;            --> 0 to 2^32 -1
         * uint8_t type;                    --> 0 to 49
         * uint8_t autopilot;               --> 0 to 20
         * uint8_t base_mode;               --> 0 to 255
         * uint8_t system_status;           --> 0 to 8
         * uint8_t mavlink_version;         --> auto set
         */

        mavlink_heartbeat_t payload = {0, 0, 0, 0, 0};
        fuzz_args_t args = {system_id, comp_id, &message, &payload,
            (void(*)(uint8_t, uint8_t, mavlink_message_t*, void*))mavlink_msg_heartbeat_encode,
             ver, "hb", &ct};

        fuzz(0, 256, 8421504, (void *)&payload.custom_mode, UINT32_T, &args);
        fuzz(0, 50, 1, (void *)&payload.type, UINT8_T, &args);
        fuzz(0, 21, 1, (void *)&payload.autopilot, UINT8_T, &args);
        fuzz(0, 256, 1, (void *)&payload.base_mode, UINT8_T, &args);
        fuzz(0, 9, 1, (void *)&payload.system_status, UINT8_T, &args);

    // -------------------------------------------------------------------
    // SYS_STATUS (1) TESTS
    // -------------------------------------------------------------------
    } else if (msg_id == MAVLINK_MSG_ID_SYS_STATUS) {
        /* PAYLOAD (in serialization order)
         * uint32_t onboard_control_sensors_present                 // 0 to 2^32 -1
         * uint32_t onboard_control_sensors_enabled                 // 0 to 2^32 -1
         * uint32_t onboard_control_sensors_health                  // 0 to 2^32 -1
         * uint16_t load                                            // 0 to 1000
         * uint16_t voltage_battery                                 // 0 to 2^16 -1
         * int16_t current_battery                                  // -2^-15 to 2^15 -1
         * uint16_t drop_rate_comm                                  // 0 to 10000
         * uint16_t errors_comm                                     // 0 to 2^16 -1
         * uint16_t errors_count1                                   // 0 to 2^16 -1
         * uint16_t errors_count2                                   // 0 to 2^16 -1
         * uint16_t errors_count3                                   // 0 to 2^16 -1
         * uint16_t errors_count4                                   // 0 to 2^16 -1
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
             ver, "ss", &ct};

        fuzz(0, 256, 8421504, (void *)&payload.onboard_control_sensors_present, UINT32_T, &args);
        fuzz(0, 256, 8421504, (void *)&payload.onboard_control_sensors_enabled, UINT32_T, &args);
        fuzz(0, 256, 8421504, (void *)&payload.onboard_control_sensors_health, UINT32_T, &args);
        fuzz(0, 1001, 1, (void *)&payload.load, UINT16_T, &args);
        fuzz(0, 256, 257, (void *)&payload.voltage_battery, UINT16_T, &args);
        fuzz(-128, 128, 128, (void *)&payload.current_battery, INT16_T, &args);
        fuzz(0, 1001, 10, (void *)&payload.drop_rate_comm, UINT16_T, &args);
        fuzz(0, 256, 257, (void *)&payload.errors_count1, UINT16_T, &args);
        fuzz(0, 256, 257, (void *)&payload.errors_count2, UINT16_T, &args);
        fuzz(0, 256, 257, (void *)&payload.errors_count3, UINT16_T, &args);
        fuzz(0, 256, 257, (void *)&payload.errors_count4, UINT16_T, &args);
        fuzz(-1, 101, 1, (void *)&payload.battery_remaining, INT8_T, &args);


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
         * uint16_t throttle        // 0 t0 100
         */

        mavlink_vfr_hud_t payload = {0, 0, 0, 0, 0, 0};
        fuzz_args_t args = {system_id, comp_id, &message, &payload,
            (void(*)(uint8_t, uint8_t, mavlink_message_t*, void*))mavlink_msg_vfr_hud_encode,
             ver, "vfr_hud", &ct};

        fuzz(0, 0, 0, (void *)&payload.airspeed, FLOAT, &args);
        fuzz(0, 0, 0, (void *)&payload.groundspeed, FLOAT, &args);
        fuzz(0, 0, 0, (void *)&payload.alt, FLOAT, &args);
        fuzz(0, 0, 0, (void *)&payload.climb, FLOAT, &args);
        fuzz(0, 361, 1, (void *)&payload.heading, INT16_T, &args);
        fuzz(0, 101, 1, (void *)&payload.throttle, UINT16_T, &args);
    }

    exit(EXIT_SUCCESS);
}

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// Function to fuzz a field in a message
// Based on location in memory, goes in and changes payload field (payloads are just structs)
// Packs and writes to file
void fuzz(int start, int end, int mult, void *field_loc, int field_type, fuzz_args_t *args) {
    int i;
    // Random float values to fuzz
    float float_tvals[] = {0.0, 1.0, 1.12351, -1.0, -1.32412,
        612.132, -234.12, FLT_MAX, -FLT_MAX, FLT_MIN};
    int num_ftvs = sizeof(float_tvals)/sizeof(float);

    for (i = start; i < end; i++) {
        long val = i * mult;
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
                if (i >= 0 && i < num_ftvs) {
                    val = float_tvals[i];
                    *(float*)field_loc = (float)val;
                } else {
                    return;
                }
                break;
            default :
                break;
        }
        args->encode_fn(args->sys_id, args->comp_id, args->message, args->struct_loc);
        write_message(args->message, args->abbr, args->ver, *(args->ct));
        *(args->ct) += 1;
    }
}

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// Write message to MAVLink buffer and then to file
void write_message(mavlink_message_t *message, char *abbrev, int ver, int ct){
    FILE *ofile;
    char fname[100];
    sprintf(fname, "pass.v%d.%s.%d", ver, abbrev, ct);
    if ((ofile = fopen(fname, "wb")) == NULL) {
        fprintf(stderr, "Couldn't open %s.\n", fname);
        return;
    }

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    int msg_len = mavlink_msg_to_send_buffer(buffer, message);
    if (msg_len <= 0) {
        fprintf(stderr, "MAVLink encoding failed (msg_len=%d)\n", msg_len);
        return;
    }

    if (ct == 2337) {
        fprintf(stderr, "written bytes size %d, message len %d\n", msg_len, message->len);
    }

    // Write exact length of message (truncate rest of buffer)
    fwrite(buffer, 1, msg_len, ofile);
    fclose(ofile);
}
