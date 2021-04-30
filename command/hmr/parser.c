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
	HParser *command_parser;
	//commands
	H_RULE(SET_AOR, h_ch('\x10'));
	H_RULE(SET_EZ, h_ch('\x20'));
	H_RULE(TARGET, h_ch('\x30'));
	
	H_RULE(MSGID, h_uint8());

	H_RULE(MIN, h_int_range(h_uint8(),0,60));	//min and seconds for lat and long 
	H_RULE(SEC, h_int_range(h_uint8(),0,60));

	H_RULE(POS, h_int_range(h_uint8(), 0,180)); 	// postive range of 0 - 180                  

	//negative range of -1 to -180
	H_RULE(NEG_L, h_int_range(h_uint8(), 76, 127)); //case of 0
	H_RULE(NEG_H, h_int_range(h_int8(), 128, 255));
  H_RULE(NEG, h_choice(NEG_L,NEG_H,NULL));

	H_RULE(ZEROS, h_ch('\x00')); 
	H_RULE(ONES,  h_ch('\xFF'));

	H_RULE(NEG_SEQ, h_sequence(NEG,ONES,NULL)); //111111 neg values -> reversed order so neg values 1111111 
	H_RULE(POS_SEQ, h_sequence(POS,ZEROS,NULL)); //same logic for postiive numbers pos values 0000000
	H_RULE(LONG, h_choice(NEG_SEQ,POS_SEQ,NULL)); 
	 
	H_RULE(BOT, h_int_range(h_int8(), -90,  -2)); //range for lat which is -90 to 90 in int8_t 
	H_RULE(TOP, h_int_range(h_int8(), 0,  90));
	H_RULE(N_ONE, h_int_range(h_int8(), 255, 255));  //value for -1
	H_RULE(LAT, h_choice(BOT, N_ONE,TOP, NULL));

	H_RULE(COORD, h_sequence(LAT,MIN,SEC,LONG,MIN,SEC, NULL)); 	//full coordinate sequence 
	 	
	H_RULE(WPNID, h_int_range(h_uint8(), 1, 2)); //weapon id which is 1 or 2
	
	H_RULE(c1, h_sequence(SET_AOR, COORD, COORD, MSGID, NULL));
	H_RULE(c2, h_sequence(SET_EZ, COORD, COORD, MSGID, NULL));
	H_RULE(c3, h_sequence(TARGET, COORD, WPNID, MSGID, NULL));
	H_RULE(command, h_choice(c1,c2,c3,NULL));

	command_parser = h_sequence(command, h_end_p(), NULL);
	return command_parser;
}

