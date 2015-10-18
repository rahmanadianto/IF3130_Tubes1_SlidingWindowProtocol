/*
 * File 			 : common.h
 * Description : This file contain all configuration and data structure for sliding window protocol project 
 */

#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h>
#include <unistd.h> // close socket
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> // inet_pton
#include <pthread.h>

/* ASCII CONST */
#define SOH 1				// Start of Header Character 
#define STX 2				// Start of Text Character
#define ETX 3				// End of Text Character 
#define ACK 6				// Acknowledgement
#define NAK 21			// Negatif Acknowledgement
#define Endfile 26	// End of file Character

/* Constant */
#define MAXLEN 1024

typedef unsigned char Byte;

struct Frame {
	unsigned int soh;
	unsigned int frame_number;
	unsigned int stx;
	unsigned int etx;
	unsigned int checksum;
	Byte data;
};

struct Ack {
	unsigned int value;
	unsigned int frame_number;
	unsigned int checksum;
};

unsigned int hash(unsigned int input) {
  unsigned int hash = 5381;
  hash += ((input << 5) ^ ((input & 0xf8000000) >> 27));

  return hash;
}

#endif