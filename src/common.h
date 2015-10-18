/*
 * File 			 : common.h
 * Description : This file contain all configuration and data structure for sliding window protocol project 
 */


#ifndef COMMON_H
#define COMMON_H

/* ASCII CONST */
#define SOH 1				// Start of Header Character 
#define STX 2				// Start of Text Character
#define ETX 3				// End of Text Character 
#define ACK 6				// Acknowledgement
#define NAK 21			// Negatif Acknowledgement
#define Endfile 26	// End of file Character

typedef unsigned char Byte;

struct Frame {
	unsigned int soh;
	unsigned int frame_number;
	unsigned int stx;
	unsigned int etx;
	Byte checksum;
	Byte* data;
};

struct ACK {
	unsigned int ack;
	unsigned int frame_number;
	Byte checksum;
};

#endif