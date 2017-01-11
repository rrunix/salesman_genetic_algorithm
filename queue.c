#include "queue.h"

void stack_push(stack ** head, void *data)
{
	stack *new_node = malloc(sizeof(*new_node));
	if (NULL == new_node) {
		fputs("Couldn't allocate memoryn", stderr);
		abort();
	}
	new_node->data = data;
	new_node->next = *head;
	*head = new_node;
}

void stack_remove(stack ** head)
{
	while (stack_pop(head) != 0);
}

void *stack_pop(stack ** head)
{
	stack *top;
	void *value;
	if (NULL == head || NULL == *head) {
		printf("Returning 0\n");
		return 0;
	}
	top = *head;
	value = top->data;
	*head = top->next;
	free(top);
	return value;
}
