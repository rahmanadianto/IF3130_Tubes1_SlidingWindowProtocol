/**
 *
 * created by Luqman A. Siswanto 
 * 13513024
 */

#include "common.h"
#include "Frame.h"
#include "Ack.h"

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

const int BUFFER_SIZE = 2 * WINDOWSIZE;
const int N = 25;

struct arg_struct {
  // nothing, just formality :)
};

struct arg_struct args;

queue<char> q[BUFFER_SIZE];
char buffer[N];
struct sockaddr_in myaddr;
struct sockaddr_in remaddr;
socklen_t addrlen;
int sockfd;

/* paddr: print the IP address in a standard decimal dotted format */
void paddr(unsigned char *a) {
  printf("%d.%d.%d.%d", a[0], a[1], a[2], a[3]);
}

void sendAck(Frame f, bool success) {
  printf("Mengirim ");
  printf(success? "ACK\n" : "NAK\n");
  Ack ack(success? ACK : NAK, f.getFrameNumber());
  sendto(sockfd, ack.serialize(), 6, 0, (struct sockaddr*) &remaddr, (socklen_t) addrlen);
}

void *run_receive(void*) {
  Frame f;
  while(1) {
    int recvlen = recvfrom(sockfd, buffer, 9, 0, (struct sockaddr*) &remaddr, &addrlen);
    f.unserialize(buffer);
    int num = f.getFrameNumber();
    if(f.isValid()) {
      sendAck(f, true);
      printf("Frame number %d diterima dengan sukses.\n", num);
      q[num].push(f.getData());
    } else {
      sendAck(f, false);
      printf("Frame number %d diterima. Checksum error / format salah.\n", num);
    }
  }
  pthread_exit(NULL);
}

void *run_consume(void*) {
  int pt = 0;
  int wait = 100000; // in microseconds;
  while(1) {
    while(q[pt].empty()) {
      usleep(wait);
    }
    printf("Mengkonsumsi byte : '%c'\n", q[pt].front());
    q[pt].pop();
    pt++;
    usleep(DELAY);
  }
  pthread_exit(NULL);
}

int argc;
char** argv;

class Receiver {
public:
  Receiver() {
    create_socket();
    initiate_binding();
    bind_socket();

    
    if(pthread_create(&receiver_thread, NULL, &run_receive, (void*) &args) != 0) {
      puts("Threading receiver gagal");
      exit(0);
    }
    if(pthread_create(&consumer_thread, NULL, &run_consume, (void*) &args) != 0) {
      puts("Threading consumer gagal");
      exit(0);
    }
  }
  ~Receiver() {
    pthread_exit(NULL);
    close(sockfd);
  }
private:
  void create_socket() {
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("Tidak dapat membuat socket\n");
      exit(0);
    }
    printf("Socket created with descriptor %d\n", sockfd);
  }

  void initiate_binding() {
    addrlen = sizeof(remaddr);
    port = argc > 1? atoi(argv[1]) : DEFAULT_PORT;
    memset((char*) &myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET; 
    inet_pton(AF_INET, "127.0.0.1", &myaddr.sin_addr.s_addr);
    myaddr.sin_port = htons(port);

    printf("Binding pada alamat ");
    paddr((unsigned char*) &myaddr.sin_addr.s_addr);
    printf(":%d ...\n", port);
  }

  void bind_socket() {
    if(bind(sockfd, (struct sockaddr*) &myaddr, sizeof(myaddr)) < 0) {
      perror("Binding gagal.");
      exit(0);
    }
    printf("Binding berhasil\n\n");
  }

  int port;
  pthread_t receiver_thread;
  pthread_t consumer_thread;
};

int main(int _argc, char *_argv[]) {
  argc = _argc;
  for(int i = 0; i < argc; i++) {
    memcpy(argv[i], _argv[i], sizeof(_argv[i]));
  }
  Receiver rec;
  return 0;
}