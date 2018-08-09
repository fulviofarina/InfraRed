#pragma once

#define MAX_ALPHABET 8 //ARRAY SIZES

struct  AlphaBeta
{
	String auxMsgRcvd = "abcdefgh"; //auxiliar

	char Mean[MAX_ALPHABET] =
	{ 'a', 'b', 'c','d', 'e','f', 'g' ,'h' };//, 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r'
											 //{ 'a', 'b', 'c','d', 'e','f', 'g' ,'h','i','j','k','l','m','n', 'o', 'p','q','r'}; // YOUR OWN ALPHABET
	uint8_t  Freq[MAX_ALPHABET] =    //ARRAY OF FREQUENCY MULTIPLIERS
									 //	{ 2, 3, 4, 5, 6, 7, 9, 12 };  //original

									 //	 { 2, 3, 4, 5, 6, 7, 10, 12 }; //current

	{ 3, 8, 4, 10, 7, 12, 2, 5 };  //test

	const char split = Mean[MAX_ALPHABET - 1];  //k letter
};

static AlphaBeta abc;