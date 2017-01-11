#include "queue.h"

#ifndef TSP_DATATYPES_H
#define TSP_DATATYPES_H

typedef enum {
	DIRTY, UPDATED

} State;

typedef struct  {
	float x;
	float y;
} Point;

typedef struct {
	Point *points;
	int size;
	float distance;
	float fitness;
	State state;
} Path;

typedef struct {
	Path *path;
	int size;
} Population;

typedef struct {
	

} GA_evolving_data;

typedef struct {
	int population;
	int prop_mutation;
	int prop_crossover;
	int iterations;
	GA_evolving_data evolving_data;
} GA_Parameters;



typedef struct {
	GA_Parameters *parameters;
	Population population;
	Point *point;
	int size;
	stack *head;
	int stack_size;
	Path **resources;
} GA_Data;
#endif


