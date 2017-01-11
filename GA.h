#include "Datatypes.h"
#include <stdlib.h>
#include <math.h> 
#include <stdio.h>
#include "queue.h"
#include <time.h>
#include <limits.h>
#include <omp.h>

#ifndef GA_H

#define GA_H

/**
 *
 *
 */
GA_Data* GA_init(GA_Parameters * parameters, Point * point, int size);

/**
 *
 *
 */
void initPopulation(GA_Data *ga);


/**
 *
 *
 */
void GA_init_child(Path ** child, GA_Data *ga);

/**
 *
 *
 */
void GA_create_child(Path **child, GA_Data *ga);

/**
 *
 *
 */
void GA_clean_up(GA_Data *ga);
/**
 *
 *
 */
void GA_evolve(GA_Data *ga);

/**
 *
 *
 */
void GA_crossover(Path * father1, Path * father2, Path * child1,
				  Path * child2);

/**
 *
 *
 */
void GA_mutation(Path * path,GA_Data *ga);

/**
 *
 *
 */
void GA_update_chromosome(Path * path);

/**
 *
 *
 */
Path *GA_fittest(GA_Data *ga);

/**
 *
 *
 */
Path *GA_get_fittest(GA_Data *ga);

/**
 *
 *
 */
void GA_crossover_fill_chromosome(Path * child, Path * father, int pivot);

/**
 *
 *
 */
void GA_print_path(Path * path);

/**
 *
 *
 */
void copy(Path * source, Path * dest);

/**
 *
 *
 *
 */
void GA_free_chromosome(Path * path,GA_Data *ga);

/**
 *
 *
 */
Path *GA_tournament_selection(Population * population,GA_Data *ga);

/**
 *
 *
 */
int distance(Point * point1, Point * point2);

/**
 *
 *
 */
void GA_print_chromosome_data(Path * path);
/**
 *
 *
 */

int inArray(Point * pointArray, int length, Point * toTest);

/**
 *
 *
 */
int equals(Point * point1, Point * point2);

/**
 *
 *
 */
void GA_start(GA_Data *ga);

#endif
