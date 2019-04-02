#include "../../includes/server/server.h"

/* decided to push to the head in order to speed up and have O(1) */
void	push_front(t_data **head_ref, unsigned char *new_data)
{
	t_data *new_node;

	if (!(new_node = (t_data *)malloc(sizeof(t_data))))
	{
		printf("%s\n", "No memory left");
		exit(0);
	}

	memcpy(new_node->data, new_data, INITIAL_PACKET_SIZE + DIGEST_SIZE);

	new_node->next = (*head_ref);

	(*head_ref) = new_node;
}

void	deleteList(t_data **head_ref)
{

	t_data *current = *head_ref;
	t_data *next;

	while (current != NULL)
	{
		next = current->next;
		free(current->data);
		current = next;
	}

	*head_ref = NULL;
}