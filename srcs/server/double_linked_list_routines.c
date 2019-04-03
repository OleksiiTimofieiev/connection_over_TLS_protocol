#include "server/server.h"

void	append(t_queue **head_ref, unsigned char *new_data)
{
	t_queue *new_node = (t_queue *)malloc(sizeof(t_queue));
	t_queue *last = *head_ref; 

	memcpy(new_node->data, new_data, INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY);

	new_node->next = NULL;

	if (*head_ref == NULL)
	{
		new_node->prev = NULL;
		*head_ref = new_node;
		return;
	}

	while (last->next != NULL)
		last = last->next;

	last->next = new_node;

	new_node->prev = last;

	return;
}

void	deleteNode(t_queue **head_ref, t_queue *del)
{
	/* base case */
	if (*head_ref == NULL || del == NULL)
		return ;

	/* If node to be deleted is head node */
	if (*head_ref == del)
		*head_ref = del->next;

	/* Change next only if node to be deleted is NOT the last node */
	if (del->next != NULL)
		del->next->prev = del->prev;

	/* Change prev only if node to be deleted is NOT the first node */
	if (del->prev != NULL)
		del->prev->next = del->next;

	/* Finally, free the memory occupied by del*/
	free(del->data);
	free(del);
	
	return ;
}