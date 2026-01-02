/*
 * parser.c - Hammer parser for SPP packets
 *
 * Ben Cavanagh
 * 11-28-2025
 * Description: Hammer parser for Space Packet Protocol messages
 *              Parses primary headers and verifies following
 *              data fits into an integral number of octets
 *
 */

#include "parser.h"

bool match_x00(HParseResult *p, void *user_data);
bool match_x01(HParseResult *p, void *user_data);
bool match_idle_apid(HParseResult *p, void *user_data);
HParsedToken *plus_one(const HParseResult *p, void *user_data);

HParser *init_parser() {
    HParser *spp_parser;

    /* Utility: match a single bit */
    H_RULE(zero_bit, h_attr_bool(h_bits(1, false), match_x00, NULL));
    H_RULE(one_bit, h_attr_bool(h_bits(1, false), match_x01, NULL));

    /* ------------------------------------------------------------- */
    /* SPP packet data fields */
    /* ------------------------------------------------------------- */
    /* Parses length field as TT_UINT and tries to parse that many
     * octets + 1 worth of data (unspecified value/structure) */
    H_RULE(length, h_action(h_uint16(), plus_one, NULL));
    H_RULE(unspec_data, h_length_value(length, h_uint8()));

    /* Example idle packet; specified as mission-specific */
    /* Chose 1010 1010 1010 1010 for this parser */
    H_RULE(idle_apid, h_attr_bool(h_bits(11, false), match_idle_apid, NULL));
    H_RULE(len_C01, h_ch(0x01)); // Count = 1 = 2 bytes - 1
    H_RULE(idle_data, h_sequence(h_ch(0xAA), h_ch(0xAA), NULL));

    /* ------------------------------------------------------------- */
    /* Top-level SPP bit-wise header fields */
    /* ------------------------------------------------------------- */
    H_RULE(version, h_repeat_n(zero_bit, 3));
    H_RULE(type, h_bits(1, false));

    /* Parse 1-bit sec hdr flag, returns a TT_UINT which must match enum */
    H_RULE(no_shf, zero_bit);
    H_RULE(has_shf, one_bit);

    H_RULE(other_apid, h_bits(11, false));
    H_RULE(seqf, h_bits(2, false));
    H_RULE(seqc, h_bits(14, false));

    /* ------------------------------------------------------------- */
    /* Top-level SPP packet strucrure */
    /* ------------------------------------------------------------- */
    /* Example idle packet */
    H_RULE(idle, h_sequence(version, type, no_shf, idle_apid, seqf, seqc, len_C01, idle_data, NULL));
    /* Generic data packet */
    H_RULE(other, h_sequence(version, type, no_shf, other_apid, seqf, seqc, unspec_data, NULL));

    /* Packet does not contain secondary header */
    H_RULE(no_sec_hdr, h_choice(idle, other, NULL));
    /* Packet contains secondary header */
    H_RULE(has_sec_hdr, h_sequence(version, type, has_shf, other_apid, seqf, seqc, unspec_data, NULL));

    H_RULE(packet, h_choice(has_sec_hdr, no_sec_hdr, NULL));
    spp_parser = h_sequence(packet, h_end_p(), NULL);

    return(spp_parser);
}

/* ------------------------------------------------------------- */
/* Validation functions to match bit fields to enum values*/
/* ------------------------------------------------------------- */
bool match_x00(HParseResult *p, void *user_data) {
    if (TT_UINT != p->ast->token_type) return false;
    return(0 == p->ast->uint);
}
bool match_x01(HParseResult *p, void *user_data) {
    if (TT_UINT != p->ast->token_type) return false;
    return(1 == p->ast->uint);
}

/* Idle packet APID is 1111 1111 111 = 2047 */
bool match_idle_apid(HParseResult *p, void *user_data) {
    if (TT_UINT != p->ast->token_type) return false;
    return(2047 == p->ast->uint);
}

/* Normal parse_length_value() doesn't work because of off-by-one,
 * so add 1 to the parse result */
HParsedToken *plus_one(const HParseResult *p, void *user_data) {
    uint16_t value = p->ast->uint + 1;
    HParsedToken *t = H_MAKE_UINT(value);
    return t;
}
