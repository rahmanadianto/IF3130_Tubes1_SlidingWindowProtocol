<<<<<<< HEAD
#include	"common.h"
#include <fstream>
#include <vector>

class Transmitter {
	public:
		Transmitter(char* host, int port, char* filename) {
			/* Construct the transmiter sockaddr structure */
			memset(&transmitterAddr, 0, sizeof(transmitterAddr));
			transmitterAddr.sin_family = AF_INET;
			transmitterAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			transmitterAddr.sin_port = htons(0);

			/* Create the socket */
			if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
				perror("Failed to create socket");
			}

			if (bind(socketfd, (struct sockaddr *) &transmitterAddr, sizeof(transmitterAddr)) < 0) {
				perror("Failed to bind socket");
			}

			/* Construct the receiver sockaddr structure */
			memset(&receiverAddr, 0, sizeof(receiverAddr));
			receiverAddr.sin_family = AF_INET;
			inet_pton(AF_INET, host, &receiverAddr.sin_addr.s_addr);
			receiverAddr.sin_port = htons(port);

			messageParsing(filename);
			iterator = 0;
		}

		void send() {
			// sending data
		}

		~Transmitter() {
			close(socketfd);
		}

	private:
		int socketfd; // socket handler
		int iterator;
		struct sockaddr_in transmitterAddr;
		struct sockaddr_in receiverAddr;

		// array of frame
		std::vector<struct Frame> frames;

		void messageParsing(char* filename) {
			std::ifstream infile;
			Frame frame;
			infile.open(filename);

			char currentChar;
			int counter = 0;
			int checksum;
			while (infile.get(currentChar)) {
				frame.soh = SOH;
				checksum = hash(SOH);
				frame.frame_number = counter;
				checksum += hash(counter);
				frame.stx = STX;
				checksum += hash(STX);
				frame.data = (Byte) currentChar;
				checksum += hash((unsigned int) frame.data);
				frame.etx = ETX;
				checksum += hash(ETX);
				frame.checksum = checksum;

				frames.push_back(frame);

				counter++; 
			}
		}
};

int main(int argc, char* argv[]) {
	if (argc > 3) {
		Transmitter transmitter(argv[1], atoi(argv[2]), argv[3]);
		transmitter.send();
	}
	else {
		printf("Usage : transmitter [receiver IP] [receiver port] [message.txt]\n");
	}
}
=======
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
>>>>>>> 8ad339bf37ea2fe8ea0030343713518d78fc07f0
