#include "../../includes/server/server.h"

/* decided to push to the head in order to speed up and have O(1) */
void	push_front(t_data **head_ref, unsigned char *new_data)
{
	/* 1. allocate node */
	t_data *new_node = (t_data *)malloc(sizeof(t_data));

	/* 2. put in the data  */
	memcpy(new_node->data, new_data, INITIAL_PACKET_SIZE + DIGEST_SIZE);

	/* 3. Make next of new node as head */
	new_node->next = (*head_ref);

	/* 4. move the head to point to the new node */
	(*head_ref) = new_node;
}