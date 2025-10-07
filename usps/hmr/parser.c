/*
 * hmr.c -- Hammer parser for USPS addresses
 *
 * Author: Josh Meise
 *
 * Oct. 7, 2025
 *
 */
#include "parser.h"

HParser *init_parser() {
  HParser *hmr_parser;

  H_RULE(LF, h_ch('\n'));

  H_RULE(CR, h_ch('\r'));

  H_RULE(CRLF, h_choice(LF, CR, NULL));

  H_RULE(SP, h_ch(' '));

  H_RULE(HTAB, h_ch('\t'));

  H_RULE(WSPS, h_choice(SP, HTAB, NULL));

  H_RULE(WSP, h_many1(WSPS));

  H_RULE(DIGIT, h_ch_range('0', '9'));

  H_RULE(ALPHA, h_choice(h_ch_range('a', 'z'), h_ch_range('A', 'Z'), NULL));

  H_RULE(word, h_many1(ALPHA));

  HParser *sentence = h_indirect();
  
  h_bind_indirect(sentence, h_choice(word, h_sequence(sentence, WSP, word, NULL), NULL));

  H_RULE(multi_digit, h_many1(DIGIT));

  H_RULE(multi_digits, h_choice(h_epsilon_p(), h_sequence(multi_digit, WSP, NULL), h_sequence(WSP, multi_digit, NULL), NULL));

  H_RULE(address, h_choice(h_sequence(multi_digits, sentence, multi_digits, NULL), h_sequence(multi_digits, sentence, SP, h_ch('#'), multi_digits, NULL), NULL));

  H_RULE(rule, h_sequence(sentence, CRLF, address, CRLF, sentence, multi_digits, NULL));
  
  hmr_parser = h_sequence(rule, h_end_p(),NULL);

  return hmr_parser;
}

