/*
 * hmr.c -- Hammer parser for JSON numbers
 *
 * Author: Stephen Taylor & Brandon Guzman
 *
 *  * Aug, 31, 2020
 *
 */
#include "parser.h"

HParser *init_parser() {
	HParser *response_parser;

	H_RULE(INV_TARGET, h_ch('\x40'));                   
  H_RULE(WPN_AWAY,   h_ch('\x50'));                    
  H_RULE(FIRING_FLT, h_ch('\x60'));
	H_RULE(CYBER_ALRT, h_ch('\x70'));                   
  H_RULE(ACK,        h_ch('\x80'));                    
  H_RULE(NAK,        h_ch('\x81'));

	H_RULE(MSGID, h_uint8());
	
	H_RULE(r1, h_sequence(INV_TARGET, MSGID, NULL));
	H_RULE(r2, h_sequence(WPN_AWAY, MSGID, NULL));
	H_RULE(r3, h_sequence(FIRING_FLT, MSGID, NULL));
	H_RULE(r4, h_sequence(CYBER_ALRT, MSGID, NULL));
	H_RULE(r5, h_sequence(ACK, MSGID, NULL));
	H_RULE(r6, h_sequence(NAK, MSGID, NULL));
	H_RULE(RSPN, h_choice(r1,r2,r3,r4,r5,r6, NULL));

	response_parser = h_sequence(RSPN, h_end_p(),NULL);
	return response_parser;
}

