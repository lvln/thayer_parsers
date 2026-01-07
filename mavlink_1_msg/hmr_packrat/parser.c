/*
 * parser.c - Hammer parser for 1 MAVLink message type
 *
 * Ben Cavanagh
 * 10-29-2025
 * Description: Hammer parser for MAVLink HEARTBEAT messages
 *
 */

#include "parser.h"

HParser *init_parser() {
    HParser *mav_1_enum_parser;

    /* Numbers and hex ranges */
    H_RULE(allhex1b, h_ch_range(0x00,0xFF));
    H_RULE(allhex2b, h_sequence(h_ch_range(0x00,0xFF), h_ch_range(0x00,0xFF), NULL));
    H_RULE(allhex4b, h_sequence(h_ch_range(0x00,0xFF), h_ch_range(0x00,0xFF),
                                h_ch_range(0x00,0xFF), h_ch_range(0x00,0xFF), NULL));

    H_RULE(nonzero1b, h_ch_range(0x01,0xFF));

    H_RULE(len_L09, h_ch(0x09));

    /* Header fields */
    H_RULE(stx_v1, h_ch(0xFE));
    H_RULE(stx_v2, h_ch(0xFD));

    //                        sequence  systemID   compID
    H_RULE(v1_SSC, h_sequence(allhex1b, nonzero1b, nonzero1b, NULL));
    //                          inc flgs    cmp flgs  sequence  systemID   compID
    H_RULE(us_ICSSC, h_sequence(h_ch(0x00), allhex1b, allhex1b, nonzero1b, nonzero1b, NULL));
    H_RULE(sg_ICSSC, h_sequence(h_ch(0x01), allhex1b, allhex1b, nonzero1b, nonzero1b, NULL));

    H_RULE(crc, allhex2b);
    H_RULE(sig, h_sequence(allhex4b, allhex4b, allhex4b, allhex1b, NULL));

    /* -------------------------------------------------------------- */
    /* BEGIN HEARTBEAT (0) MESSAGE */
    /* -------------------------------------------------------------- */
    H_RULE(v1_hb_ID, h_sequence(h_ch(0x00), NULL));
    H_RULE(v2_hb_ID, h_sequence(h_ch(0x00), h_ch(0x00), h_ch(0x00), NULL));

    /* HEARTBEAT Payload Fields */
    H_RULE(hb_PL_cust, allhex4b);
    H_RULE(hb_PL_type, h_ch_range(0x00,0x31));
    H_RULE(hb_PL_auto, h_ch_range(0x00,0x14));
    H_RULE(hb_PL_base, allhex1b);
    H_RULE(hb_PL_stat, h_ch_range(0x00,0x08));
    H_RULE(hb_PL_mvrs, allhex1b);

    /* HEARTBEAT v1/v2 Payloads */
    /* v1 HEARTBEAT PAYLOAD - payload always 9 bytes */
    H_RULE(v1_hb_PL, h_sequence(hb_PL_cust, hb_PL_type, hb_PL_auto,
                                hb_PL_base, hb_PL_stat, hb_PL_mvrs, NULL));
    /* v2 HEARTBEAT PAYLOAD - payload always 9 bytes  */
    H_RULE(v2_hb_PL_L09, h_sequence(hb_PL_cust, hb_PL_type, hb_PL_auto,
                                    hb_PL_base, hb_PL_stat, hb_PL_mvrs, NULL));

    /* -------------------------------------------------------------- */
    /* HEARTBEAT us/sg Length Options - always 9 bytes */
    H_RULE(us_hb_L09, h_sequence(len_L09, us_ICSSC, v2_hb_ID, v2_hb_PL_L09, NULL));
    H_RULE(sg_hb_L09, h_sequence(len_L09, sg_ICSSC, v2_hb_ID, v2_hb_PL_L09, NULL));

    /* -------------------------------------------------------------- */
    /* HEARTBEAT Top Level Message Structures: v1/us/sg */

    /* v1 HEARTBEAT - payload always 9 bytes */
    H_RULE(v1_hb, h_sequence(len_L09, v1_SSC, v1_hb_ID, v1_hb_PL, NULL));
    /* us HEARTBEAT - payload always 9 bytes */
    H_RULE(us_hb, h_sequence(us_hb_L09, NULL));
    /* sg HEARTBEAT - payload always 9 bytes */
    H_RULE(sg_hb, h_sequence(sg_hb_L09, NULL));

    /* -------------------------------------------------------------- */
    /* END HEARTBEAT (0) MESSAGE */
    /* -------------------------------------------------------------- */

    /* -------------------------------------------------------------- */
    /* 1 supported message type: */
    /* HEARTBEAT (0) */
    /* -------------------------------------------------------------- */
    H_RULE(msg_v1, h_sequence(stx_v1, v1_hb, crc, NULL));
    H_RULE(msg_sg, h_sequence(stx_v2, sg_hb, crc, sig, NULL));
    H_RULE(msg_us, h_sequence(stx_v2, us_hb, crc, NULL));

    /* -------------------------------------------------------------- */
    /* Top-level MAVLink structures */
    /* -------------------------------------------------------------- */
    /* Message is either v1, v2 signed, or v2 unsigned */
    H_RULE(msg_v2, h_choice(msg_sg, msg_us, NULL));

    /* Message is either MAVLink v1 or MAVLink v2 */
    H_RULE(message, h_choice(msg_v1, msg_v2, NULL));

    mav_1_enum_parser = h_sequence(message, h_end_p(), NULL);

    return(mav_1_enum_parser);
}

