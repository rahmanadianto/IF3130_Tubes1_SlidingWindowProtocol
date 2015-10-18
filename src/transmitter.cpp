#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//#include <thread>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fstream>

#define WindowSize 5

using namespace std;

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in transaddr;
	Frame buffer[100];

	// Membuat socket
	printf("Membuat socket untuk koneksi ke %s:%s ...\n", argv[1], argv[2]);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
  	perror("Can't create socket");
  	exit(-1);
  }
  bzero((char *)&transaddr, sizeof(transaddr));
  inet_pton(AF_INET, argv[1], &(transaddr.sin_addr)); // Alamat IP
  transaddr.sin_port = htons(atoi(argv[2])); // Port Number

  // Read character from file & save to buffer					
  ifstream infile;
	infile.open(argv[3]);
  int i=0;
  char c;
	while (infile.get(c))
	{	
		buffer[i].soh = SOH;
		buffer[i].frame_number = i % WindowSize;
		buffer[i].stx = STX;
		buffer[i].etx = ETX;
		//buffer[i].checksum = ???
		Byte data = (Byte)c;
		buffer[i].data = (Byte)c;
		i++;
	}
	infile.close();
}
