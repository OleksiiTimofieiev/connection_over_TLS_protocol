#include "server/server.h"

t_data 				*l_data = NULL;
t_queue 			*queue	= NULL;
pthread_mutex_t 	mutex;

void 	*handle_new_data(void *data)
{
	unsigned char 		aes_key[32];
	unsigned char 		decrypted_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE] = {0};
	const unsigned char iv_1[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};
	unsigned char 		iv[16];
	unsigned char		error_message[] = "error";

	/* get the data for the thread from the void *data */
	t_thread_data *input = (t_thread_data *)data;

	memcpy(iv, iv_1, 16);

	/* decryption of the aes key for the packet */
	rsa_decrypt(&input->data[INITIAL_PACKET_SIZE + DIGEST_SIZE], aes_key);

	/* decryption of the packet with aes key */
	aes_decrypt(iv, aes_key, input->data, decrypted_full_packet);

	unsigned char checksum[DIGEST_SIZE];

	/* generate checksum */
	mbedtls_sha1_ret(decrypted_full_packet, 256, checksum);

	pthread_mutex_lock(&mutex);

	/* checking the checksum and adding to the linked list if valid checksum */
	if (check_sha1_sum(checksum, &decrypted_full_packet[256]))
		push_front(&l_data, decrypted_full_packet, false);
	else
		push_front(&l_data, error_message, true);

	pthread_mutex_unlock(&mutex);

	input->current_status = FINISHED;

	return (NULL);
}

bool 	thread_create(t_thread *thread_pool, unsigned char *buffer, int threads_limit)
{
	int ret, i;

	/*
	 * Find inactive or finished thread slot
	 */

	for (i = 0; i < threads_limit; i++)
	{
		if (thread_pool[i].data.current_status == INACTIVE)
			break;

		if (thread_pool[i].data.current_status == FINISHED)
		{
			pthread_join(thread_pool[i].thread, NULL);
			break;
		}
	}

	if (i == threads_limit)
	{
		printf("%s\n", "No available thread.");
		return (false);
	}

	/*
	 * Fill thread-info for thread
	 */

	memcpy(thread_pool[i].data.data, buffer, INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY);

	thread_pool[i].data.current_status = RUNNING;

	/* create thread task */
	if ((ret = pthread_create(&thread_pool[i].thread, NULL, handle_new_data, &thread_pool[i].data)) != 0)
	{
		printf("%s\n", "thread creation error");
		exit(0);
	}

	return (true);
}