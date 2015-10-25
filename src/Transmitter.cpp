/*
 * Author : Rahman Adianto - 13513006
 */

#include "Frame.h"
#include "Ack.h"
#include <chrono>

const long long TIMEOUT = 3000; // millisecond

int socketfd; // socket handler
struct sockaddr_in transmitterAddr;
struct sockaddr_in receiverAddr; 

typedef struct {
	Frame frame;
	chrono::system_clock::time_point lastSent;
	Byte ack;
} rQElement;

// buffer for retransmission, set chrono::system_clock::now() every transmit
deque<rQElement> retransmissionQueue;

// receive acknowledgement
void* receiveAck(void* threadId) {
	Ack ack;
	char* buffer;
	socklen_t addrlen = sizeof(receiverAddr);
	while(true) {
		int recvlen = recvfrom(socketfd, buffer, 6, 0, (struct sockaddr*) &receiverAddr, &addrlen);
    ack.unserialize(buffer);
    int frameNum = ack.getFrameNumber();
    if(ack.isValid()) {
    	cout << retransmissionQueue.front().frame.getFrameNumber() << ' ' << frameNum << endl;
    	if (retransmissionQueue.front().frame.getFrameNumber() == frameNum) {
    		while(retransmissionQueue.front().ack == ACK) {
    			retransmissionQueue.pop_front();
    			printf("sliding window\n");
    		}
    	}
    	else {
    		for (auto &it : retransmissionQueue) {
    			if (it.frame.getFrameNumber() == frameNum) {
    				it.ack = ACK;
    			}
    		}
    	}
    }
	}

	pthread_exit(NULL);
}

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
		}

		void send() {
			/* Create thread for receive ACK signal from receiver */
			pthread_t thread;
			pthread_create(&thread, NULL, receiveAck, NULL);

			socklen_t addrlen = sizeof(receiverAddr);
			for (auto it : frames) {
				if (retransmissionQueue.size() < WINDOWSIZE) {
					if (!sendto(socketfd, it.serialize(), 9, 0, (struct sockaddr *)&receiverAddr, addrlen)) {
						perror("sendto failed");
					} 
					else {
						printf("Paket %d dikirim\n", it.getFrameNumber());
					}

					rQElement rqe;
					rqe.frame = it;
					rqe.lastSent = chrono::system_clock::now();
					rqe.ack = NAK;
					retransmissionQueue.push_back(rqe);
				}

				while (retransmissionQueue.size() == WINDOWSIZE) {
					if (!retransmissionQueue.empty()) {
						if (timeDiv(chrono::system_clock::now(), retransmissionQueue.front().lastSent) > TIMEOUT) {
							rQElement rqe;
							rqe.frame = retransmissionQueue.front().frame;
							rqe.lastSent = chrono::system_clock::now();
							rqe.ack = retransmissionQueue.front().ack;

							// retransmission if timeout and NAK
							if (rqe.ack == NAK) {
								if (!sendto(socketfd, rqe.frame.serialize(), 9, 0, (struct sockaddr *)&receiverAddr, addrlen)) {
									perror("sendto failed");
								} 
								else {
									printf("Paket %d dikirim ulang\n", rqe.frame.getFrameNumber());
								}
							}

							retransmissionQueue.pop_front();
							retransmissionQueue.push_back(rqe);
						}
					}
				} 
			}
		}

		~Transmitter() {
			close(socketfd);
		}

	private:
		// array of frame
		std::vector<Frame> frames;
		void messageParsing(char* filename) {
			std::ifstream infile;
			infile.open(filename);

			char currentChar;
			int counter = 0;
			int checksum;
			while (infile.get(currentChar)) {
				Frame frame(counter % (2 * WINDOWSIZE), currentChar);
				frames.push_back(frame);
				counter++; 
			}
		}

		// div antar dua waktu
		long long timeDiv(chrono::system_clock::time_point t1, chrono::system_clock::time_point t2) {
			return chrono::duration_cast<chrono::milliseconds>(t1 - t2).count();
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
