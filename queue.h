#include <stdlib.h>
#include <stdio.h>

#ifndef TSP_QUEUE_H
#define TSP_QUEUE_H

typedef struct stack_s {
	void *data; /* Can hold any type of pointer */
	struct stack_s *next; /* The stack is built as a linked list */
} stack;

void  stack_push(stack **head, void *data);
void* stack_pop(stack **head);
void  stack_remove(stack **head);
#endif
