#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <ev.h>
#include "ev.h"

#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <unistd.h>

#include "../../includes/server/server.h"

#define DEFAULT_PORT    3333
#define BUF_SIZE        1024

#define INITIAL_PACKET_SIZE         256
#define DIGEST_SIZE                 16
#define LEN_OF_ENCPYPTED_AES_KEY    256



// gcc -Wall -Wextra -Werror -o server server.c -I/Users/otimofie/.brew/Cellar/libev/4.24/include -L/Users/otimofie/.brew/Cellar/libev/4.24/lib -lev
// Lots of globals, what's the best way to get rid of these?
int sd; // socket descriptor
struct sockaddr_in addr;
int addr_len = sizeof(addr);
unsigned char buffer[INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY];
unsigned char aes_key[32];
unsigned char decrypted_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE] = {0};
const unsigned char iv_1[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};

// This callback is called when data is readable on the UDP socket.
void 	udp_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
// static void udp_cb(EV_P_ ev_io *w, int revents) {

	unsigned char iv[16];

	memcpy(iv, iv_1, 16);

    if(EV_ERROR & revents)
    {
      perror("got invalid event");
      return;
    }
    // puts("udp socket has become readable");
    int bytes = recvfrom(watcher->fd, buffer, sizeof(buffer), 0, (struct sockaddr*) &addr, (socklen_t *) &addr_len);

    // printf("bytes received -> %d\n", bytes);

			// printf("\n");
			// for (int i = 0; i < INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY; i++)
			// printf("%x ", buffer[i]);

			rsa_decrypt(&buffer[INITIAL_PACKET_SIZE + DIGEST_SIZE], aes_key);

			printf("\n");
			// for (int i = 0; i < 32; i++)
			// 	printf("%x ", aes_key[i]);

			// add a null to terminate the input, as we're going to use it as a string
			// buffer[bytes] = '\0';

			aes_decrypt(iv, aes_key, buffer, decrypted_full_packet);
			// for (int i = 0; i < 32; i++)
			// 	printf("%x ", aes_key[i]);
			// printf("\n");

			// printf("iv ->");
			// for (int i = 0; i < 16; i++)
			// 	printf("%x ", iv[i]);

			int i = 0;

			while(i < INITIAL_PACKET_SIZE + DIGEST_SIZE)
			{
				printf("%x ", decrypted_full_packet[i++]);
			}

			unsigned char checksum[DIGEST_SIZE];

			mbedtls_sha1_ret(decrypted_full_packet, 256, checksum);

			 i = 0;
			int j = 256;

			// bool function;
			while(i < 16)
			{
				if (checksum[i] != decrypted_full_packet[j])
					printf("wtf ?\n");
				else
				{
					printf("%d", i + 1);
				}
				
				i++;
				j++;
			}
			

				// printf("udp client said: %s\n", buffer);

				if (bytes == 0)
			{
				ev_io_stop(loop, watcher);
				perror("peer might closing");
     }
}

int main(void) {
    int port = DEFAULT_PORT;
    puts("udp_echo server started...");

    // Setup a udp listening socket.
    sd = socket(PF_INET, SOCK_DGRAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
        perror("bind");

    // Do the libev stuff.
    struct ev_loop *loop = ev_default_loop(0);
    ev_io udp_watcher;
    ev_io_init(&udp_watcher, udp_cb, sd, EV_READ);
    ev_io_start(loop, &udp_watcher);
    ev_loop(loop, 0);

    // This point is never reached.
    close(sd);
    return EXIT_SUCCESS;
}
