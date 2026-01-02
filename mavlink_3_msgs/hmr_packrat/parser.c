/*
 * parser.c - Hammer parser for 3 MAVLink message types
 *
 * Ben Cavanagh
 * 10-29-2025
 * Description: Hammer parser for MAVLink HEARTBEAT, SYS_STATUS,
 *              and VFR_HUD messages
 *
 */

#include "parser.h"

HParser *init_parser() {
    HParser *mav_3_enum_parser;

    /* Numbers and hex ranges */
    H_RULE(allhex1b, h_ch_range(0x00,0xFF));
    H_RULE(allhex2b, h_sequence(h_ch_range(0x00,0xFF), h_ch_range(0x00,0xFF), NULL));
    H_RULE(allhex3b, h_sequence(h_ch_range(0x00,0xFF), h_ch_range(0x00,0xFF),
                                h_ch_range(0x00,0xFF), NULL));
    H_RULE(allhex4b, h_sequence(h_ch_range(0x00,0xFF), h_ch_range(0x00,0xFF),
                                h_ch_range(0x00,0xFF), h_ch_range(0x00,0xFF), NULL));

    H_RULE(nonzero1b, h_ch_range(0x01,0xFF));

    H_RULE(len_L01, h_ch(0x01));
    H_RULE(len_L02, h_ch(0x02));
    H_RULE(len_L03, h_ch(0x03));
    H_RULE(len_L04, h_ch(0x04));
    H_RULE(len_L05, h_ch(0x05));
    H_RULE(len_L06, h_ch(0x06));
    H_RULE(len_L07, h_ch(0x07));
    H_RULE(len_L08, h_ch(0x08));
    H_RULE(len_L09, h_ch(0x09));
    H_RULE(len_L10, h_ch(0x0A));
    H_RULE(len_L11, h_ch(0x0B));
    H_RULE(len_L12, h_ch(0x0C));
    H_RULE(len_L13, h_ch(0x0D));
    H_RULE(len_L14, h_ch(0x0E));
    H_RULE(len_L15, h_ch(0x0F));
    H_RULE(len_L16, h_ch(0x10));
    H_RULE(len_L17, h_ch(0x11));
    H_RULE(len_L18, h_ch(0x12));
    H_RULE(len_L19, h_ch(0x13));
    H_RULE(len_L20, h_ch(0x14));
    H_RULE(len_L21, h_ch(0x15));
    H_RULE(len_L22, h_ch(0x16));
    H_RULE(len_L23, h_ch(0x17));
    H_RULE(len_L24, h_ch(0x18));
    H_RULE(len_L25, h_ch(0x19));
    H_RULE(len_L26, h_ch(0x1A));
    H_RULE(len_L27, h_ch(0x1B));
    H_RULE(len_L28, h_ch(0x1C));
    H_RULE(len_L29, h_ch(0x1D));
    H_RULE(len_L30, h_ch(0x1E));
    H_RULE(len_L31, h_ch(0x1F));

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
    /* BEGIN SYS_STATUS (1) MESSAGE */
    /* -------------------------------------------------------------- */
    H_RULE(v1_ss_ID, h_sequence(h_ch(0x01), NULL));
    H_RULE(v2_ss_ID, h_sequence(h_ch(0x01), h_ch(0x00), h_ch(0x00), NULL));

    /* SYS_STATUS Payload Fields */
    H_RULE(ss_PL_ocsp, allhex4b);
    H_RULE(ss_PL_ocse, allhex4b);
    H_RULE(ss_PL_ocsh, allhex4b);
    H_RULE(ss_PL_load, h_with_endianness(BYTE_LITTLE_ENDIAN, h_int_range(h_int16(), 0, 1000)));
    H_RULE(ss_PL_vbat, allhex2b); /* 0x0000 to 0xFFFE*/
    H_RULE(ss_PL_cbat, allhex2b);
    H_RULE(ss_PL_drc, h_with_endianness(BYTE_LITTLE_ENDIAN, h_int_range(h_int16(), 0, 10000)));
    H_RULE(ss_PL_errc, allhex2b);
    H_RULE(ss_PL_cnt1, allhex2b);
    H_RULE(ss_PL_cnt2, allhex2b);
    H_RULE(ss_PL_cnt3, allhex2b);
    H_RULE(ss_PL_cnt4, allhex2b);
    H_RULE(ss_PL_batr, h_with_endianness(BYTE_LITTLE_ENDIAN, h_int_range(h_int8(), -1, 100)));

    /* SYS_STATUS v1/v2 Payloads */
    /* v1 SYS_STATUS PAYLOAD - payload always 31 bytes */
    H_RULE(v1_ss_PL, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh,
                                ss_PL_load, ss_PL_vbat, ss_PL_cbat,
                                ss_PL_drc,  ss_PL_errc, ss_PL_cnt1,
                                ss_PL_cnt2, ss_PL_cnt3, ss_PL_cnt4,
                                ss_PL_batr, NULL));
    /* v2 SYS_STATUS PAYLOAD - payload 1-31 bytes  */
    H_RULE(v2_ss_PL_L01, allhex1b);
    H_RULE(v2_ss_PL_L02, allhex2b);
    H_RULE(v2_ss_PL_L03, allhex3b);
    H_RULE(v2_ss_PL_L04, ss_PL_ocsp);
    H_RULE(v2_ss_PL_L05, h_sequence(ss_PL_ocsp, allhex1b, NULL));
    H_RULE(v2_ss_PL_L06, h_sequence(ss_PL_ocsp, allhex2b, NULL));
    H_RULE(v2_ss_PL_L07, h_sequence(ss_PL_ocsp, allhex3b, NULL));
    H_RULE(v2_ss_PL_L08, h_sequence(ss_PL_ocsp, ss_PL_ocse, NULL));
    H_RULE(v2_ss_PL_L09, h_sequence(ss_PL_ocsp, ss_PL_ocse, allhex1b, NULL));
    H_RULE(v2_ss_PL_L10, h_sequence(ss_PL_ocsp, ss_PL_ocse, allhex2b, NULL));
    H_RULE(v2_ss_PL_L11, h_sequence(ss_PL_ocsp, ss_PL_ocse, allhex3b, NULL));
    H_RULE(v2_ss_PL_L12, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, NULL));
    H_RULE(v2_ss_PL_L13, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, allhex1b, NULL));
    H_RULE(v2_ss_PL_L14, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, NULL));
    H_RULE(v2_ss_PL_L15, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, allhex1b, NULL));
    H_RULE(v2_ss_PL_L16, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, NULL));
    H_RULE(v2_ss_PL_L17, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, allhex1b, NULL));
    H_RULE(v2_ss_PL_L18, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, NULL));
    H_RULE(v2_ss_PL_L19, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, allhex1b, NULL));
    H_RULE(v2_ss_PL_L20, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, NULL));
    H_RULE(v2_ss_PL_L21, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, allhex1b, NULL));
    H_RULE(v2_ss_PL_L22, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, NULL));
    H_RULE(v2_ss_PL_L23, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, allhex1b, NULL));
    H_RULE(v2_ss_PL_L24, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, ss_PL_cnt1, NULL));
    H_RULE(v2_ss_PL_L25, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, ss_PL_cnt1, allhex1b, NULL));
    H_RULE(v2_ss_PL_L26, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, ss_PL_cnt1, ss_PL_cnt2, NULL));
    H_RULE(v2_ss_PL_L27, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, ss_PL_cnt1, ss_PL_cnt2, allhex1b, NULL));
    H_RULE(v2_ss_PL_L28, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, ss_PL_cnt1, ss_PL_cnt2, ss_PL_cnt3, NULL));
    H_RULE(v2_ss_PL_L29, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, ss_PL_cnt1, ss_PL_cnt2, ss_PL_cnt3, allhex1b, NULL));
    H_RULE(v2_ss_PL_L30, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, ss_PL_cnt1, ss_PL_cnt2, ss_PL_cnt3, ss_PL_cnt4, NULL));
    H_RULE(v2_ss_PL_L31, h_sequence(ss_PL_ocsp, ss_PL_ocse, ss_PL_ocsh, ss_PL_load, ss_PL_vbat, ss_PL_cbat, ss_PL_drc, ss_PL_errc, ss_PL_cnt1, ss_PL_cnt2, ss_PL_cnt3, ss_PL_cnt4, ss_PL_batr, NULL));

    /* -------------------------------------------------------------- */
    /* SYS_STATUS us/sg Length Options - 1-31 bytes */

    H_RULE(us_ss_L01, h_sequence(len_L01, us_ICSSC, v2_ss_ID, v2_ss_PL_L01, NULL));
    H_RULE(us_ss_L02, h_sequence(len_L02, us_ICSSC, v2_ss_ID, v2_ss_PL_L02, NULL));
    H_RULE(us_ss_L03, h_sequence(len_L03, us_ICSSC, v2_ss_ID, v2_ss_PL_L03, NULL));
    H_RULE(us_ss_L04, h_sequence(len_L04, us_ICSSC, v2_ss_ID, v2_ss_PL_L04, NULL));
    H_RULE(us_ss_L05, h_sequence(len_L05, us_ICSSC, v2_ss_ID, v2_ss_PL_L05, NULL));
    H_RULE(us_ss_L06, h_sequence(len_L06, us_ICSSC, v2_ss_ID, v2_ss_PL_L06, NULL));
    H_RULE(us_ss_L07, h_sequence(len_L07, us_ICSSC, v2_ss_ID, v2_ss_PL_L07, NULL));
    H_RULE(us_ss_L08, h_sequence(len_L08, us_ICSSC, v2_ss_ID, v2_ss_PL_L08, NULL));
    H_RULE(us_ss_L09, h_sequence(len_L09, us_ICSSC, v2_ss_ID, v2_ss_PL_L09, NULL));
    H_RULE(us_ss_L10, h_sequence(len_L10, us_ICSSC, v2_ss_ID, v2_ss_PL_L10, NULL));
    H_RULE(us_ss_L11, h_sequence(len_L11, us_ICSSC, v2_ss_ID, v2_ss_PL_L11, NULL));
    H_RULE(us_ss_L12, h_sequence(len_L12, us_ICSSC, v2_ss_ID, v2_ss_PL_L12, NULL));
    H_RULE(us_ss_L13, h_sequence(len_L13, us_ICSSC, v2_ss_ID, v2_ss_PL_L13, NULL));
    H_RULE(us_ss_L14, h_sequence(len_L14, us_ICSSC, v2_ss_ID, v2_ss_PL_L14, NULL));
    H_RULE(us_ss_L15, h_sequence(len_L15, us_ICSSC, v2_ss_ID, v2_ss_PL_L15, NULL));
    H_RULE(us_ss_L16, h_sequence(len_L16, us_ICSSC, v2_ss_ID, v2_ss_PL_L16, NULL));
    H_RULE(us_ss_L17, h_sequence(len_L17, us_ICSSC, v2_ss_ID, v2_ss_PL_L17, NULL));
    H_RULE(us_ss_L18, h_sequence(len_L18, us_ICSSC, v2_ss_ID, v2_ss_PL_L18, NULL));
    H_RULE(us_ss_L19, h_sequence(len_L19, us_ICSSC, v2_ss_ID, v2_ss_PL_L19, NULL));
    H_RULE(us_ss_L20, h_sequence(len_L20, us_ICSSC, v2_ss_ID, v2_ss_PL_L20, NULL));
    H_RULE(us_ss_L21, h_sequence(len_L21, us_ICSSC, v2_ss_ID, v2_ss_PL_L21, NULL));
    H_RULE(us_ss_L22, h_sequence(len_L22, us_ICSSC, v2_ss_ID, v2_ss_PL_L22, NULL));
    H_RULE(us_ss_L23, h_sequence(len_L23, us_ICSSC, v2_ss_ID, v2_ss_PL_L23, NULL));
    H_RULE(us_ss_L24, h_sequence(len_L24, us_ICSSC, v2_ss_ID, v2_ss_PL_L24, NULL));
    H_RULE(us_ss_L25, h_sequence(len_L25, us_ICSSC, v2_ss_ID, v2_ss_PL_L25, NULL));
    H_RULE(us_ss_L26, h_sequence(len_L26, us_ICSSC, v2_ss_ID, v2_ss_PL_L26, NULL));
    H_RULE(us_ss_L27, h_sequence(len_L27, us_ICSSC, v2_ss_ID, v2_ss_PL_L27, NULL));
    H_RULE(us_ss_L28, h_sequence(len_L28, us_ICSSC, v2_ss_ID, v2_ss_PL_L28, NULL));
    H_RULE(us_ss_L29, h_sequence(len_L29, us_ICSSC, v2_ss_ID, v2_ss_PL_L29, NULL));
    H_RULE(us_ss_L30, h_sequence(len_L30, us_ICSSC, v2_ss_ID, v2_ss_PL_L30, NULL));
    H_RULE(us_ss_L31, h_sequence(len_L31, us_ICSSC, v2_ss_ID, v2_ss_PL_L31, NULL));

    H_RULE(sg_ss_L01, h_sequence(len_L01, sg_ICSSC, v2_ss_ID, v2_ss_PL_L01, NULL));
    H_RULE(sg_ss_L02, h_sequence(len_L02, sg_ICSSC, v2_ss_ID, v2_ss_PL_L02, NULL));
    H_RULE(sg_ss_L03, h_sequence(len_L03, sg_ICSSC, v2_ss_ID, v2_ss_PL_L03, NULL));
    H_RULE(sg_ss_L04, h_sequence(len_L04, sg_ICSSC, v2_ss_ID, v2_ss_PL_L04, NULL));
    H_RULE(sg_ss_L05, h_sequence(len_L05, sg_ICSSC, v2_ss_ID, v2_ss_PL_L05, NULL));
    H_RULE(sg_ss_L06, h_sequence(len_L06, sg_ICSSC, v2_ss_ID, v2_ss_PL_L06, NULL));
    H_RULE(sg_ss_L07, h_sequence(len_L07, sg_ICSSC, v2_ss_ID, v2_ss_PL_L07, NULL));
    H_RULE(sg_ss_L08, h_sequence(len_L08, sg_ICSSC, v2_ss_ID, v2_ss_PL_L08, NULL));
    H_RULE(sg_ss_L09, h_sequence(len_L09, sg_ICSSC, v2_ss_ID, v2_ss_PL_L09, NULL));
    H_RULE(sg_ss_L10, h_sequence(len_L10, sg_ICSSC, v2_ss_ID, v2_ss_PL_L10, NULL));
    H_RULE(sg_ss_L11, h_sequence(len_L11, sg_ICSSC, v2_ss_ID, v2_ss_PL_L11, NULL));
    H_RULE(sg_ss_L12, h_sequence(len_L12, sg_ICSSC, v2_ss_ID, v2_ss_PL_L12, NULL));
    H_RULE(sg_ss_L13, h_sequence(len_L13, sg_ICSSC, v2_ss_ID, v2_ss_PL_L13, NULL));
    H_RULE(sg_ss_L14, h_sequence(len_L14, sg_ICSSC, v2_ss_ID, v2_ss_PL_L14, NULL));
    H_RULE(sg_ss_L15, h_sequence(len_L15, sg_ICSSC, v2_ss_ID, v2_ss_PL_L15, NULL));
    H_RULE(sg_ss_L16, h_sequence(len_L16, sg_ICSSC, v2_ss_ID, v2_ss_PL_L16, NULL));
    H_RULE(sg_ss_L17, h_sequence(len_L17, sg_ICSSC, v2_ss_ID, v2_ss_PL_L17, NULL));
    H_RULE(sg_ss_L18, h_sequence(len_L18, sg_ICSSC, v2_ss_ID, v2_ss_PL_L18, NULL));
    H_RULE(sg_ss_L19, h_sequence(len_L19, sg_ICSSC, v2_ss_ID, v2_ss_PL_L19, NULL));
    H_RULE(sg_ss_L20, h_sequence(len_L20, sg_ICSSC, v2_ss_ID, v2_ss_PL_L20, NULL));
    H_RULE(sg_ss_L21, h_sequence(len_L21, sg_ICSSC, v2_ss_ID, v2_ss_PL_L21, NULL));
    H_RULE(sg_ss_L22, h_sequence(len_L22, sg_ICSSC, v2_ss_ID, v2_ss_PL_L22, NULL));
    H_RULE(sg_ss_L23, h_sequence(len_L23, sg_ICSSC, v2_ss_ID, v2_ss_PL_L23, NULL));
    H_RULE(sg_ss_L24, h_sequence(len_L24, sg_ICSSC, v2_ss_ID, v2_ss_PL_L24, NULL));
    H_RULE(sg_ss_L25, h_sequence(len_L25, sg_ICSSC, v2_ss_ID, v2_ss_PL_L25, NULL));
    H_RULE(sg_ss_L26, h_sequence(len_L26, sg_ICSSC, v2_ss_ID, v2_ss_PL_L26, NULL));
    H_RULE(sg_ss_L27, h_sequence(len_L27, sg_ICSSC, v2_ss_ID, v2_ss_PL_L27, NULL));
    H_RULE(sg_ss_L28, h_sequence(len_L28, sg_ICSSC, v2_ss_ID, v2_ss_PL_L28, NULL));
    H_RULE(sg_ss_L29, h_sequence(len_L29, sg_ICSSC, v2_ss_ID, v2_ss_PL_L29, NULL));
    H_RULE(sg_ss_L30, h_sequence(len_L30, sg_ICSSC, v2_ss_ID, v2_ss_PL_L30, NULL));
    H_RULE(sg_ss_L31, h_sequence(len_L31, sg_ICSSC, v2_ss_ID, v2_ss_PL_L31, NULL));

    /* -------------------------------------------------------------- */
    /* SYS_STATUS Top Level Message Structures: v1/us/sg */

    /* v1 SYS_STATUS - payload always 31 bytes */
    H_RULE(v1_ss, h_sequence(len_L31, v1_SSC, v1_ss_ID, v1_ss_PL, NULL));
    /* us SYS_STATUS - payload 1-31 bytes */
    H_RULE(us_ss, h_choice(us_ss_L01, us_ss_L02, us_ss_L03, us_ss_L04, us_ss_L05, us_ss_L06, us_ss_L07, us_ss_L08, us_ss_L09, us_ss_L10, us_ss_L11, us_ss_L12, us_ss_L13, us_ss_L14, us_ss_L15, us_ss_L16, us_ss_L17, us_ss_L18, us_ss_L19, us_ss_L20, us_ss_L21, us_ss_L22, us_ss_L23, us_ss_L24, us_ss_L25, us_ss_L26, us_ss_L27, us_ss_L28, us_ss_L29, us_ss_L30, us_ss_L31, NULL));
    /* sg SYS_STATUS - payload 1-31 bytes */
    H_RULE(sg_ss, h_choice(sg_ss_L01, sg_ss_L02, sg_ss_L03, sg_ss_L04, sg_ss_L05, sg_ss_L06, sg_ss_L07, sg_ss_L08, sg_ss_L09, sg_ss_L10, sg_ss_L11, sg_ss_L12, sg_ss_L13, sg_ss_L14, sg_ss_L15, sg_ss_L16, sg_ss_L17, sg_ss_L18, sg_ss_L19, sg_ss_L20, sg_ss_L21, sg_ss_L22, sg_ss_L23, sg_ss_L24, sg_ss_L25, sg_ss_L26, sg_ss_L27, sg_ss_L28, sg_ss_L29, sg_ss_L30, sg_ss_L31, NULL));

    /* -------------------------------------------------------------- */
    /* END SYS_STATUS (1) MESSAGE */
    /* -------------------------------------------------------------- */

    /* -------------------------------------------------------------- */
    /* BEGIN VFR_HUD (74) MESSAGE */
    /* -------------------------------------------------------------- */
    H_RULE(v1_hud_ID, h_sequence(h_ch(0x4A), NULL));
    H_RULE(v2_hud_ID, h_sequence(h_ch(0x4A), h_ch(0x00), h_ch(0x00), NULL));

    /* VFR_HUD Payload Fields */
    H_RULE(hud_PL_aspd, allhex4b);
    H_RULE(hud_PL_gspd, allhex4b);
    H_RULE(hud_PL_alt,  allhex4b);
    H_RULE(hud_PL_clmb, allhex4b);
    H_RULE(hud_PL_hdng, h_with_endianness(BYTE_LITTLE_ENDIAN, h_int_range(h_int16(), 0, 360)));
    H_RULE(hud_PL_thtl, h_with_endianness(BYTE_LITTLE_ENDIAN, h_int_range(h_uint16(), 0, 100)));

    /* VFR_HUD v1/v2 Payloads */
    /* v1 VFR_HUD PAYLOAD - payload always 20 bytes */
    H_RULE(v1_hud_PL, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt,
                                 hud_PL_clmb, hud_PL_hdng, hud_PL_thtl, NULL));

    /* v2 VFR_HUD PAYLOAD - paylohud_PL_thtl,ad 1-20 bytes  */
    H_RULE(v2_hud_PL_L01, allhex1b);
    H_RULE(v2_hud_PL_L02, allhex2b);
    H_RULE(v2_hud_PL_L03, allhex3b);
    H_RULE(v2_hud_PL_L04, hud_PL_aspd);
    H_RULE(v2_hud_PL_L05, h_sequence(hud_PL_aspd, allhex1b, NULL));
    H_RULE(v2_hud_PL_L06, h_sequence(hud_PL_aspd, allhex2b, NULL));
    H_RULE(v2_hud_PL_L07, h_sequence(hud_PL_aspd, allhex3b, NULL));
    H_RULE(v2_hud_PL_L08, h_sequence(hud_PL_aspd, hud_PL_gspd, NULL));
    H_RULE(v2_hud_PL_L09, h_sequence(hud_PL_aspd, hud_PL_gspd, allhex1b, NULL));
    H_RULE(v2_hud_PL_L10, h_sequence(hud_PL_aspd, hud_PL_gspd, allhex2b, NULL));
    H_RULE(v2_hud_PL_L11, h_sequence(hud_PL_aspd, hud_PL_gspd, allhex3b, NULL));
    H_RULE(v2_hud_PL_L12, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, NULL));
    H_RULE(v2_hud_PL_L13, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, allhex1b, NULL));
    H_RULE(v2_hud_PL_L14, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, allhex2b, NULL));
    H_RULE(v2_hud_PL_L15, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, allhex3b, NULL));
    H_RULE(v2_hud_PL_L16, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, hud_PL_clmb, NULL));
    H_RULE(v2_hud_PL_L17, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, hud_PL_clmb, allhex1b, NULL));
    H_RULE(v2_hud_PL_L18, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, hud_PL_clmb, hud_PL_hdng, NULL));
    H_RULE(v2_hud_PL_L19, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, hud_PL_clmb, hud_PL_hdng, allhex1b, NULL));
    H_RULE(v2_hud_PL_L20, h_sequence(hud_PL_aspd, hud_PL_gspd, hud_PL_alt, hud_PL_clmb, hud_PL_hdng, hud_PL_thtl, NULL));

    /* -------------------------------------------------------------- */
    /* VFR_HUD us/sg Length Options - 1-20 bytes */

    H_RULE(us_hud_L01, h_sequence(len_L01, us_ICSSC, v2_hud_ID, v2_hud_PL_L01, NULL));
    H_RULE(us_hud_L02, h_sequence(len_L02, us_ICSSC, v2_hud_ID, v2_hud_PL_L02, NULL));
    H_RULE(us_hud_L03, h_sequence(len_L03, us_ICSSC, v2_hud_ID, v2_hud_PL_L03, NULL));
    H_RULE(us_hud_L04, h_sequence(len_L04, us_ICSSC, v2_hud_ID, v2_hud_PL_L04, NULL));
    H_RULE(us_hud_L05, h_sequence(len_L05, us_ICSSC, v2_hud_ID, v2_hud_PL_L05, NULL));
    H_RULE(us_hud_L06, h_sequence(len_L06, us_ICSSC, v2_hud_ID, v2_hud_PL_L06, NULL));
    H_RULE(us_hud_L07, h_sequence(len_L07, us_ICSSC, v2_hud_ID, v2_hud_PL_L07, NULL));
    H_RULE(us_hud_L08, h_sequence(len_L08, us_ICSSC, v2_hud_ID, v2_hud_PL_L08, NULL));
    H_RULE(us_hud_L09, h_sequence(len_L09, us_ICSSC, v2_hud_ID, v2_hud_PL_L09, NULL));
    H_RULE(us_hud_L10, h_sequence(len_L10, us_ICSSC, v2_hud_ID, v2_hud_PL_L10, NULL));
    H_RULE(us_hud_L11, h_sequence(len_L11, us_ICSSC, v2_hud_ID, v2_hud_PL_L11, NULL));
    H_RULE(us_hud_L12, h_sequence(len_L12, us_ICSSC, v2_hud_ID, v2_hud_PL_L12, NULL));
    H_RULE(us_hud_L13, h_sequence(len_L13, us_ICSSC, v2_hud_ID, v2_hud_PL_L13, NULL));
    H_RULE(us_hud_L14, h_sequence(len_L14, us_ICSSC, v2_hud_ID, v2_hud_PL_L14, NULL));
    H_RULE(us_hud_L15, h_sequence(len_L15, us_ICSSC, v2_hud_ID, v2_hud_PL_L15, NULL));
    H_RULE(us_hud_L16, h_sequence(len_L16, us_ICSSC, v2_hud_ID, v2_hud_PL_L16, NULL));
    H_RULE(us_hud_L17, h_sequence(len_L17, us_ICSSC, v2_hud_ID, v2_hud_PL_L17, NULL));
    H_RULE(us_hud_L18, h_sequence(len_L18, us_ICSSC, v2_hud_ID, v2_hud_PL_L18, NULL));
    H_RULE(us_hud_L19, h_sequence(len_L19, us_ICSSC, v2_hud_ID, v2_hud_PL_L19, NULL));
    H_RULE(us_hud_L20, h_sequence(len_L20, us_ICSSC, v2_hud_ID, v2_hud_PL_L20, NULL));

    H_RULE(sg_hud_L01, h_sequence(len_L01, sg_ICSSC, v2_hud_ID, v2_hud_PL_L01, NULL));
    H_RULE(sg_hud_L02, h_sequence(len_L02, sg_ICSSC, v2_hud_ID, v2_hud_PL_L02, NULL));
    H_RULE(sg_hud_L03, h_sequence(len_L03, sg_ICSSC, v2_hud_ID, v2_hud_PL_L03, NULL));
    H_RULE(sg_hud_L04, h_sequence(len_L04, sg_ICSSC, v2_hud_ID, v2_hud_PL_L04, NULL));
    H_RULE(sg_hud_L05, h_sequence(len_L05, sg_ICSSC, v2_hud_ID, v2_hud_PL_L05, NULL));
    H_RULE(sg_hud_L06, h_sequence(len_L06, sg_ICSSC, v2_hud_ID, v2_hud_PL_L06, NULL));
    H_RULE(sg_hud_L07, h_sequence(len_L07, sg_ICSSC, v2_hud_ID, v2_hud_PL_L07, NULL));
    H_RULE(sg_hud_L08, h_sequence(len_L08, sg_ICSSC, v2_hud_ID, v2_hud_PL_L08, NULL));
    H_RULE(sg_hud_L09, h_sequence(len_L09, sg_ICSSC, v2_hud_ID, v2_hud_PL_L09, NULL));
    H_RULE(sg_hud_L10, h_sequence(len_L10, sg_ICSSC, v2_hud_ID, v2_hud_PL_L10, NULL));
    H_RULE(sg_hud_L11, h_sequence(len_L11, sg_ICSSC, v2_hud_ID, v2_hud_PL_L11, NULL));
    H_RULE(sg_hud_L12, h_sequence(len_L12, sg_ICSSC, v2_hud_ID, v2_hud_PL_L12, NULL));
    H_RULE(sg_hud_L13, h_sequence(len_L13, sg_ICSSC, v2_hud_ID, v2_hud_PL_L13, NULL));
    H_RULE(sg_hud_L14, h_sequence(len_L14, sg_ICSSC, v2_hud_ID, v2_hud_PL_L14, NULL));
    H_RULE(sg_hud_L15, h_sequence(len_L15, sg_ICSSC, v2_hud_ID, v2_hud_PL_L15, NULL));
    H_RULE(sg_hud_L16, h_sequence(len_L16, sg_ICSSC, v2_hud_ID, v2_hud_PL_L16, NULL));
    H_RULE(sg_hud_L17, h_sequence(len_L17, sg_ICSSC, v2_hud_ID, v2_hud_PL_L17, NULL));
    H_RULE(sg_hud_L18, h_sequence(len_L18, sg_ICSSC, v2_hud_ID, v2_hud_PL_L18, NULL));
    H_RULE(sg_hud_L19, h_sequence(len_L19, sg_ICSSC, v2_hud_ID, v2_hud_PL_L19, NULL));
    H_RULE(sg_hud_L20, h_sequence(len_L20, sg_ICSSC, v2_hud_ID, v2_hud_PL_L20, NULL));

    /* -------------------------------------------------------------- */
    /* VFR_HUD Top Level Message Structures: v1/us/sg */

    /* v1 VFR_HUD - payload always 20 bytes */
    H_RULE(v1_hud, h_sequence(len_L20, v1_SSC, v1_hud_ID, v1_hud_PL, NULL));
    /* us VFR_HUD - payload 1-20 bytes */
    H_RULE(us_hud, h_choice(us_hud_L01, us_hud_L02, us_hud_L03, us_hud_L04, us_hud_L05, us_hud_L06, us_hud_L07, us_hud_L08, us_hud_L09, us_hud_L10, us_hud_L11, us_hud_L12, us_hud_L13, us_hud_L14, us_hud_L15, us_hud_L16, us_hud_L17, us_hud_L18, us_hud_L19, us_hud_L20, NULL));
    /* sg VFR_HUD - payload 1-20 bytes */
    H_RULE(sg_hud, h_choice(sg_hud_L01, sg_hud_L02, sg_hud_L03, sg_hud_L04, sg_hud_L05, sg_hud_L06, sg_hud_L07, sg_hud_L08, sg_hud_L09, sg_hud_L10, sg_hud_L11, sg_hud_L12, sg_hud_L13, sg_hud_L14, sg_hud_L15, sg_hud_L16, sg_hud_L17, sg_hud_L18, sg_hud_L19, sg_hud_L20, NULL));

    /* -------------------------------------------------------------- */
    /* END VFR_HUD (74) MESSAGE */
    /* -------------------------------------------------------------- */

    /* -------------------------------------------------------------- */
    /* 3 supported message types: */
    /* HEARTBEAT (0), SYS_STATUS (1), VFR_HUD (74) */
    /* -------------------------------------------------------------- */
    H_RULE(msg_v1, h_sequence(stx_v1, h_choice(v1_hb, v1_ss, v1_hud, NULL), crc, NULL));
    H_RULE(msg_sg, h_sequence(stx_v2, h_choice(sg_hb, sg_ss, sg_hud, NULL), crc, sig, NULL));
    H_RULE(msg_us, h_sequence(stx_v2, h_choice(us_hb, us_ss, us_hud, NULL), crc, NULL));

    /* -------------------------------------------------------------- */
    /* Top-level MAVLink structures */
    /* -------------------------------------------------------------- */
    /* Message is either v1, v2 signed, or v2 unsigned */
    H_RULE(message, h_choice(msg_v1, msg_sg, msg_us, NULL));

    mav_3_enum_parser = h_sequence(message, h_end_p(), NULL);

    return(mav_3_enum_parser);
}

