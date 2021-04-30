/*
 * hmr.c -- Hammer parser for JSON numbers
 *
 * Author: Stephen Taylor
 *
 *  * Aug, 31, 2020
 *
 */
#include "parser.h"

HParser *init_parser() {
	HParser *json_parser;
	/* numbers */

	H_RULE(bin0, h_int_range(h_uint8(), 0,0));
	H_RULE(bin1, h_int_range(h_uint8(), 1,1));
	H_RULE(bin2, h_int_range(h_uint8(), 2,2));
	H_RULE(bin3, h_int_range(h_uint8(), 3,3));
	H_RULE(bin4, h_int_range(h_uint8(), 4,4));
	H_RULE(bin5, h_int_range(h_uint8(), 5,5));
	H_RULE(bin6, h_int_range(h_uint8(), 6,6));
	H_RULE(bin7, h_int_range(h_uint8(), 7,7));
	H_RULE(bin8, h_int_range(h_uint8(), 8,8));
	H_RULE(bin9, h_int_range(h_uint8(), 9,9));

	H_RULE(bin10, h_int_range(h_uint8(), 10,10));
	H_RULE(bin11, h_int_range(h_uint8(), 11,11));
	H_RULE(bin12, h_int_range(h_uint8(), 12,12));
	H_RULE(bin13, h_int_range(h_uint8(), 13,13));
	H_RULE(bin14, h_int_range(h_uint8(), 14,14));
	H_RULE(bin15, h_int_range(h_uint8(), 15,15));
	H_RULE(bin16, h_int_range(h_uint8(), 16,16));
	H_RULE(bin17, h_int_range(h_uint8(), 17,17));
	H_RULE(bin18, h_int_range(h_uint8(), 18,18));
	H_RULE(bin19, h_int_range(h_uint8(), 19,19));

	H_RULE(bin20, h_int_range(h_uint8(), 20,20));
	H_RULE(bin21, h_int_range(h_uint8(), 21,21));
	H_RULE(bin22, h_int_range(h_uint8(), 22,22));
	H_RULE(bin23, h_int_range(h_uint8(), 23,23));
	H_RULE(bin24, h_int_range(h_uint8(), 24,24));
	H_RULE(bin25, h_int_range(h_uint8(), 25,25));
	H_RULE(bin26, h_int_range(h_uint8(), 26,26));
	H_RULE(bin27, h_int_range(h_uint8(), 27,27));
	H_RULE(bin28, h_int_range(h_uint8(), 28,28));
	H_RULE(bin29, h_int_range(h_uint8(), 29,29));

	H_RULE(bin30, h_int_range(h_uint8(), 30,30));
	H_RULE(bin31, h_int_range(h_uint8(), 31,31));
	H_RULE(bin32, h_int_range(h_uint8(), 32,32));
	H_RULE(bin33, h_int_range(h_uint8(), 33,33));
	H_RULE(bin34, h_int_range(h_uint8(), 34,34));
	H_RULE(bin35, h_int_range(h_uint8(), 35,35));
	H_RULE(bin36, h_int_range(h_uint8(), 36,36));
	H_RULE(bin37, h_int_range(h_uint8(), 37,37));
	H_RULE(bin38, h_int_range(h_uint8(), 38,38));
	H_RULE(bin39, h_int_range(h_uint8(), 39,39));

	H_RULE(bin40, h_int_range(h_uint8(), 40,40));
	H_RULE(bin41, h_int_range(h_uint8(), 41,41));
	H_RULE(bin42, h_int_range(h_uint8(), 42,42));
	H_RULE(bin43, h_int_range(h_uint8(), 43,43));
	H_RULE(bin44, h_int_range(h_uint8(), 44,44));
	H_RULE(bin45, h_int_range(h_uint8(), 45,45));
	H_RULE(bin46, h_int_range(h_uint8(), 46,46));
	H_RULE(bin47, h_int_range(h_uint8(), 47,47));
	H_RULE(bin48, h_int_range(h_uint8(), 48,48));
	H_RULE(bin49, h_int_range(h_uint8(), 49,49));

	H_RULE(bin50, h_int_range(h_uint8(), 50,50));
	H_RULE(bin51, h_int_range(h_uint8(), 51,51));
	H_RULE(bin52, h_int_range(h_uint8(), 52,52));
	H_RULE(bin53, h_int_range(h_uint8(), 53,53));
	H_RULE(bin54, h_int_range(h_uint8(), 54,54));
	H_RULE(bin55, h_int_range(h_uint8(), 55,55));
	H_RULE(bin56, h_int_range(h_uint8(), 56,56));
	H_RULE(bin57, h_int_range(h_uint8(), 57,57));
	H_RULE(bin58, h_int_range(h_uint8(), 58,58));
	H_RULE(bin59, h_int_range(h_uint8(), 59,59));

	H_RULE(bin60, h_int_range(h_uint8(), 60,60));
	H_RULE(bin61, h_int_range(h_uint8(), 61,61));
	H_RULE(bin62, h_int_range(h_uint8(), 62,62));
	H_RULE(bin63, h_int_range(h_uint8(), 63,63));
	H_RULE(bin64, h_int_range(h_uint8(), 64,64));
	H_RULE(bin65, h_int_range(h_uint8(), 65,65));
	H_RULE(bin66, h_int_range(h_uint8(), 66,66));
	H_RULE(bin67, h_int_range(h_uint8(), 67,67));
	H_RULE(bin68, h_int_range(h_uint8(), 68,68));
	H_RULE(bin69, h_int_range(h_uint8(), 69,69));

	H_RULE(bin70, h_int_range(h_uint8(), 70,70));
	H_RULE(bin71, h_int_range(h_uint8(), 71,71));
	H_RULE(bin72, h_int_range(h_uint8(), 72,72));
	H_RULE(bin73, h_int_range(h_uint8(), 73,73));
	H_RULE(bin74, h_int_range(h_uint8(), 74,74));
	H_RULE(bin75, h_int_range(h_uint8(), 75,75));
	H_RULE(bin76, h_int_range(h_uint8(), 76,76));
	H_RULE(bin77, h_int_range(h_uint8(), 77,77));
	H_RULE(bin78, h_int_range(h_uint8(), 78,78));
	H_RULE(bin79, h_int_range(h_uint8(), 79,79));

	H_RULE(bin80, h_int_range(h_uint8(), 80,80));
	H_RULE(bin81, h_int_range(h_uint8(), 81,81));
	H_RULE(bin82, h_int_range(h_uint8(), 82,82));
	H_RULE(bin83, h_int_range(h_uint8(), 83,83));
	H_RULE(bin84, h_int_range(h_uint8(), 84,84));
	H_RULE(bin85, h_int_range(h_uint8(), 85,85));
	H_RULE(bin86, h_int_range(h_uint8(), 86,86));
	H_RULE(bin87, h_int_range(h_uint8(), 87,87));
	H_RULE(bin88, h_int_range(h_uint8(), 88,88));
	H_RULE(bin89, h_int_range(h_uint8(), 89,89));

	H_RULE(bin90, h_int_range(h_uint8(), 90,90));
	H_RULE(bin91, h_int_range(h_uint8(), 91,91));
	H_RULE(bin92, h_int_range(h_uint8(), 92,92));
	H_RULE(bin93, h_int_range(h_uint8(), 93,93));
	H_RULE(bin94, h_int_range(h_uint8(), 94,94));
	H_RULE(bin95, h_int_range(h_uint8(), 95,95));
	H_RULE(bin96, h_int_range(h_uint8(), 96,96));
	H_RULE(bin97, h_int_range(h_uint8(), 97,97));
	H_RULE(bin98, h_int_range(h_uint8(), 98,98));
	H_RULE(bin99, h_int_range(h_uint8(), 99,99));

	H_RULE(bin100, h_int_range(h_uint8(), 100,100));
	H_RULE(bin101, h_int_range(h_uint8(), 101,101));
	H_RULE(bin102, h_int_range(h_uint8(), 102,102));
	H_RULE(bin103, h_int_range(h_uint8(), 103,103));
	H_RULE(bin104, h_int_range(h_uint8(), 104,104));
	H_RULE(bin105, h_int_range(h_uint8(), 105,105));
	H_RULE(bin106, h_int_range(h_uint8(), 106,106));
	H_RULE(bin107, h_int_range(h_uint8(), 107,107));
	H_RULE(bin108, h_int_range(h_uint8(), 108,108));
	H_RULE(bin109, h_int_range(h_uint8(), 109,109));

	H_RULE(bin110, h_int_range(h_uint8(), 110,110));
	H_RULE(bin111, h_int_range(h_uint8(), 111,111));
	H_RULE(bin112, h_int_range(h_uint8(), 112,112));
	H_RULE(bin113, h_int_range(h_uint8(), 113,113));
	H_RULE(bin114, h_int_range(h_uint8(), 114,114));
	H_RULE(bin115, h_int_range(h_uint8(), 115,115));
	H_RULE(bin116, h_int_range(h_uint8(), 116,116));
	H_RULE(bin117, h_int_range(h_uint8(), 117,117));
	H_RULE(bin118, h_int_range(h_uint8(), 118,118));
	H_RULE(bin119, h_int_range(h_uint8(), 119,119));

	H_RULE(bin120, h_int_range(h_uint8(), 120,120));
	H_RULE(bin121, h_int_range(h_uint8(), 121,121));
	H_RULE(bin122, h_int_range(h_uint8(), 122,122));
	H_RULE(bin123, h_int_range(h_uint8(), 123,123));
	H_RULE(bin124, h_int_range(h_uint8(), 124,124));
	H_RULE(bin125, h_int_range(h_uint8(), 125,125));
	H_RULE(bin126, h_int_range(h_uint8(), 126,126));
	H_RULE(bin127, h_int_range(h_uint8(), 127,127));
	H_RULE(bin128, h_int_range(h_uint8(), 128,128));
	H_RULE(bin129, h_int_range(h_uint8(), 129,129));

	H_RULE(bin130, h_int_range(h_uint8(), 130,130));
	H_RULE(bin131, h_int_range(h_uint8(), 131,131));
	H_RULE(bin132, h_int_range(h_uint8(), 132,132));
	H_RULE(bin133, h_int_range(h_uint8(), 133,133));
	H_RULE(bin134, h_int_range(h_uint8(), 134,134));
	H_RULE(bin135, h_int_range(h_uint8(), 135,135));
	H_RULE(bin136, h_int_range(h_uint8(), 136,136));
	H_RULE(bin137, h_int_range(h_uint8(), 137,137));
	H_RULE(bin138, h_int_range(h_uint8(), 138,138));
	H_RULE(bin139, h_int_range(h_uint8(), 139,139));

	H_RULE(bin140, h_int_range(h_uint8(), 140,140));
	H_RULE(bin141, h_int_range(h_uint8(), 141,141));
	H_RULE(bin142, h_int_range(h_uint8(), 142,142));
	H_RULE(bin143, h_int_range(h_uint8(), 143,143));
	H_RULE(bin144, h_int_range(h_uint8(), 144,144));
	H_RULE(bin145, h_int_range(h_uint8(), 145,145));
	H_RULE(bin146, h_int_range(h_uint8(), 146,146));
	H_RULE(bin147, h_int_range(h_uint8(), 147,147));
	H_RULE(bin148, h_int_range(h_uint8(), 148,148));
	H_RULE(bin149, h_int_range(h_uint8(), 149,149));

	H_RULE(bin150, h_int_range(h_uint8(), 150,150));
	H_RULE(bin151, h_int_range(h_uint8(), 151,151));
	H_RULE(bin152, h_int_range(h_uint8(), 152,152));
	H_RULE(bin153, h_int_range(h_uint8(), 153,153));
	H_RULE(bin154, h_int_range(h_uint8(), 154,154));
	H_RULE(bin155, h_int_range(h_uint8(), 155,155));
	H_RULE(bin156, h_int_range(h_uint8(), 156,156));
	H_RULE(bin157, h_int_range(h_uint8(), 157,157));
	H_RULE(bin158, h_int_range(h_uint8(), 158,158));
	H_RULE(bin159, h_int_range(h_uint8(), 159,159));

	H_RULE(bin160, h_int_range(h_uint8(), 160,160));
	H_RULE(bin161, h_int_range(h_uint8(), 161,161));
	H_RULE(bin162, h_int_range(h_uint8(), 162,162));
	H_RULE(bin163, h_int_range(h_uint8(), 163,163));
	H_RULE(bin164, h_int_range(h_uint8(), 164,164));
	H_RULE(bin165, h_int_range(h_uint8(), 165,165));
	H_RULE(bin166, h_int_range(h_uint8(), 166,166));
	H_RULE(bin167, h_int_range(h_uint8(), 167,167));
	H_RULE(bin168, h_int_range(h_uint8(), 168,168));
	H_RULE(bin169, h_int_range(h_uint8(), 169,169));

	H_RULE(bin170, h_int_range(h_uint8(), 170,170));
	H_RULE(bin171, h_int_range(h_uint8(), 171,171));
	H_RULE(bin172, h_int_range(h_uint8(), 172,172));
	H_RULE(bin173, h_int_range(h_uint8(), 173,173));
	H_RULE(bin174, h_int_range(h_uint8(), 174,174));
	H_RULE(bin175, h_int_range(h_uint8(), 175,175));
	H_RULE(bin176, h_int_range(h_uint8(), 176,176));
	H_RULE(bin177, h_int_range(h_uint8(), 177,177));
	H_RULE(bin178, h_int_range(h_uint8(), 178,178));
	H_RULE(bin179, h_int_range(h_uint8(), 179,179));

	H_RULE(bin180, h_int_range(h_uint8(), 180,180));
	H_RULE(bin181, h_int_range(h_uint8(), 181,181));
	H_RULE(bin182, h_int_range(h_uint8(), 182,182));
	H_RULE(bin183, h_int_range(h_uint8(), 183,183));
	H_RULE(bin184, h_int_range(h_uint8(), 184,184));
	H_RULE(bin185, h_int_range(h_uint8(), 185,185));
	H_RULE(bin186, h_int_range(h_uint8(), 186,186));
	H_RULE(bin187, h_int_range(h_uint8(), 187,187));
	H_RULE(bin188, h_int_range(h_uint8(), 188,188));
	H_RULE(bin189, h_int_range(h_uint8(), 189,189));

	H_RULE(bin190, h_int_range(h_uint8(), 190,190));
	H_RULE(bin191, h_int_range(h_uint8(), 191,191));
	H_RULE(bin192, h_int_range(h_uint8(), 192,192));
	H_RULE(bin193, h_int_range(h_uint8(), 193,193));
	H_RULE(bin194, h_int_range(h_uint8(), 194,194));
	H_RULE(bin195, h_int_range(h_uint8(), 195,195));
	H_RULE(bin196, h_int_range(h_uint8(), 196,196));
	H_RULE(bin197, h_int_range(h_uint8(), 197,197));
	H_RULE(bin198, h_int_range(h_uint8(), 198,198));
	H_RULE(bin199, h_int_range(h_uint8(), 199,199));

	H_RULE(bin200, h_int_range(h_uint8(), 200,200));
	H_RULE(bin201, h_int_range(h_uint8(), 201,201));
	H_RULE(bin202, h_int_range(h_uint8(), 202,202));
	H_RULE(bin203, h_int_range(h_uint8(), 203,203));
	H_RULE(bin204, h_int_range(h_uint8(), 204,204));
	H_RULE(bin205, h_int_range(h_uint8(), 205,205));
	H_RULE(bin206, h_int_range(h_uint8(), 206,206));
	H_RULE(bin207, h_int_range(h_uint8(), 207,207));
	H_RULE(bin208, h_int_range(h_uint8(), 208,208));
	H_RULE(bin209, h_int_range(h_uint8(), 209,209));

	H_RULE(bin210, h_int_range(h_uint8(), 210,210));
	H_RULE(bin211, h_int_range(h_uint8(), 211,211));
	H_RULE(bin212, h_int_range(h_uint8(), 212,212));
	H_RULE(bin213, h_int_range(h_uint8(), 213,213));
	H_RULE(bin214, h_int_range(h_uint8(), 214,214));
	H_RULE(bin215, h_int_range(h_uint8(), 215,215));
	H_RULE(bin216, h_int_range(h_uint8(), 216,216));
	H_RULE(bin217, h_int_range(h_uint8(), 217,217));
	H_RULE(bin218, h_int_range(h_uint8(), 218,218));
	H_RULE(bin219, h_int_range(h_uint8(), 219,219));

	H_RULE(bin220, h_int_range(h_uint8(), 220,220));
	H_RULE(bin221, h_int_range(h_uint8(), 221,221));
	H_RULE(bin222, h_int_range(h_uint8(), 222,222));
	H_RULE(bin223, h_int_range(h_uint8(), 223,223));
	H_RULE(bin224, h_int_range(h_uint8(), 224,224));
	H_RULE(bin225, h_int_range(h_uint8(), 225,225));
	H_RULE(bin226, h_int_range(h_uint8(), 226,226));
	H_RULE(bin227, h_int_range(h_uint8(), 227,227));
	H_RULE(bin228, h_int_range(h_uint8(), 228,228));
	H_RULE(bin229, h_int_range(h_uint8(), 229,229));

	H_RULE(bin230, h_int_range(h_uint8(), 230,230));
	H_RULE(bin231, h_int_range(h_uint8(), 231,231));
	H_RULE(bin232, h_int_range(h_uint8(), 232,232));
	H_RULE(bin233, h_int_range(h_uint8(), 233,233));
	H_RULE(bin234, h_int_range(h_uint8(), 234,234));
	H_RULE(bin235, h_int_range(h_uint8(), 235,235));
	H_RULE(bin236, h_int_range(h_uint8(), 236,236));
	H_RULE(bin237, h_int_range(h_uint8(), 237,237));
	H_RULE(bin238, h_int_range(h_uint8(), 238,238));
	H_RULE(bin239, h_int_range(h_uint8(), 239,239));

	H_RULE(bin240, h_int_range(h_uint8(), 240,240));
	H_RULE(bin241, h_int_range(h_uint8(), 241,241));
	H_RULE(bin242, h_int_range(h_uint8(), 242,242));
	H_RULE(bin243, h_int_range(h_uint8(), 243,243));
	H_RULE(bin244, h_int_range(h_uint8(), 244,244));
	H_RULE(bin245, h_int_range(h_uint8(), 245,245));
	H_RULE(bin246, h_int_range(h_uint8(), 246,246));
	H_RULE(bin247, h_int_range(h_uint8(), 247,247));
	H_RULE(bin248, h_int_range(h_uint8(), 248,248));
	H_RULE(bin249, h_int_range(h_uint8(), 249,249));

	H_RULE(bin250, h_int_range(h_uint8(), 250,250));
	H_RULE(bin251, h_int_range(h_uint8(), 251,251));
	H_RULE(bin252, h_int_range(h_uint8(), 252,252));
	H_RULE(bin253, h_int_range(h_uint8(), 253,253));
	H_RULE(bin254, h_int_range(h_uint8(), 254,254));
	H_RULE(bin255, h_int_range(h_uint8(), 255,255));
	
	H_RULE(r0,h_sequence(bin0,bin1,bin2,bin3,bin4,bin5,bin6,bin7,bin8,bin9,NULL));
	H_RULE(r10,h_sequence(bin10,bin11,bin12,bin13,bin14,bin15,bin16,bin17,bin18,bin19,NULL));
	H_RULE(r20,h_sequence(bin20,bin21,bin22,bin23,bin24,bin25,bin26,bin27,bin28,bin29,NULL));
	H_RULE(r30,h_sequence(bin30,bin31,bin32,bin33,bin34,bin35,bin36,bin37,bin38,bin39,NULL));
	H_RULE(r40,h_sequence(bin40,bin41,bin42,bin43,bin44,bin45,bin46,bin47,bin48,bin49,NULL));
	H_RULE(r50,h_sequence(bin50,bin51,bin52,bin53,bin54,bin55,bin56,bin57,bin58,bin59,NULL));
	H_RULE(r60,h_sequence(bin60,bin61,bin62,bin63,bin64,bin65,bin66,bin67,bin68,bin69,NULL));
	H_RULE(r70,h_sequence(bin70,bin71,bin72,bin73,bin74,bin75,bin76,bin77,bin78,bin79,NULL));
	H_RULE(r80,h_sequence(bin80,bin81,bin82,bin83,bin84,bin85,bin86,bin87,bin88,bin89,NULL));
	H_RULE(r90,h_sequence(bin90,bin91,bin92,bin93,bin94,bin95,bin96,bin97,bin98,bin99,NULL));
	H_RULE(r100,h_sequence(bin100,bin101,bin102,bin103,bin104,bin105,bin106,bin107,bin108,bin109,NULL));
	H_RULE(r110,h_sequence(bin110,bin111,bin112,bin113,bin114,bin115,bin116,bin117,bin118,bin119,NULL));
	H_RULE(r120,h_sequence(bin120,bin121,bin122,bin123,bin124,bin125,bin126,bin127,bin128,bin129,NULL));
	H_RULE(r130,h_sequence(bin130,bin131,bin132,bin133,bin134,bin135,bin136,bin137,bin138,bin139,NULL));
	H_RULE(r140,h_sequence(bin140,bin141,bin142,bin143,bin144,bin145,bin146,bin147,bin148,bin149,NULL));
	H_RULE(r150,h_sequence(bin150,bin151,bin152,bin153,bin154,bin155,bin156,bin157,bin158,bin159,NULL));
	H_RULE(r160,h_sequence(bin160,bin161,bin162,bin163,bin164,bin165,bin166,bin167,bin168,bin169,NULL));
	H_RULE(r170,h_sequence(bin170,bin171,bin172,bin173,bin174,bin175,bin176,bin177,bin178,bin179,NULL));
	H_RULE(r180,h_sequence(bin180,bin181,bin182,bin183,bin184,bin185,bin186,bin187,bin188,bin189,NULL));
	H_RULE(r190,h_sequence(bin190,bin191,bin192,bin193,bin194,bin195,bin196,bin197,bin198,bin199,NULL));
	H_RULE(r200,h_sequence(bin200,bin201,bin202,bin203,bin204,bin205,bin206,bin207,bin208,bin209,NULL));
	H_RULE(r210,h_sequence(bin210,bin211,bin212,bin213,bin214,bin215,bin216,bin217,bin218,bin219,NULL));
	H_RULE(r220,h_sequence(bin220,bin221,bin222,bin223,bin224,bin225,bin226,bin227,bin228,bin229,NULL));
	H_RULE(r230,h_sequence(bin230,bin231,bin232,bin233,bin234,bin235,bin236,bin237,bin238,bin239,NULL));
	H_RULE(r240,h_sequence(bin240,bin241,bin242,bin243,bin244,bin245,bin246,bin247,bin248,bin249,NULL));
	H_RULE(r250,h_sequence(bin250,bin251,bin252,bin253,bin254,bin255,NULL));

	H_RULE(s1,h_sequence(r0,r10,r20,r30,r40,r50,r60,r70,r80,r90,NULL));
	H_RULE(s2,h_sequence(r100,r110,r120,r130,r140,r150,r160,r170,r180,r190,NULL));
	H_RULE(s3,h_sequence(r200,r210,r220,r230,r240,r250,NULL));

	json_parser = h_sequence(s1,s2,s3,h_end_p(),NULL);

	return json_parser;
}

