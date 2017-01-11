#include "GA.h"

#define DEBUG_ENABLE

GA_Data *GA_init(GA_Parameters * parameters, Point * point, int size)
{
	GA_Data *ga;
	(ga) = (GA_Data *) malloc(sizeof(GA_Data));
	(*ga).parameters = parameters;
	if((*(*ga).parameters).population % 2 != 0) {
		(*(*ga).parameters).population++;
	}
	(*ga).point = point;
	(*ga).size = size;
	(*ga).population.size = ((*parameters).population);
	(*ga).population.path =
		(Path *) malloc(sizeof(Path) * ((*parameters).population));
	srand(time(NULL));
	initPopulation(ga);
	return ga;
}

void initPopulation(GA_Data * ga)
{
	int i = 0;
	const int pop_size = ((*(*ga).parameters).population);
	(*ga).stack_size = 2 * pop_size;
	(*ga).resources = (Path **) malloc(sizeof(Path **) * (*ga).stack_size);

	for (i = 0; i < (*ga).stack_size; ++i) {
		Path *path;
		GA_create_child(&path, ga);
		stack_push(&(*ga).head, path);
		(*ga).resources[i] = path;
	}

	for (i = 0; i < pop_size; ++i) {
		Path *path;
		GA_init_child(&path, ga);
		int n = 0;
		for (; n < (*ga).size; ++n) {
			(*path).points[n] = (*ga).point[n];
		}

		for (n = 0; n < (*ga).size; ++n) {
			Point p;
			int rand1 = rand() % (*ga).size;
			int rand2 = rand() % (*ga).size;
			p = (*path).points[rand2];
			(*path).points[rand2] = (*path).points[rand1];
			(*path).points[rand1] = p;
		}
		(*ga).population.path[i] = (*path);
	}
}

void GA_init_child(Path ** child, GA_Data * ga)
{
	(*child) = (Path *) stack_pop(&(*ga).head);
}

void GA_create_child(Path ** child, GA_Data * ga)
{
	(*child) = (Path *) malloc(sizeof(Path));
	(**child).size = (*ga).size;
	(**child).points = (Point *) malloc(sizeof(Point) * (*ga).size);
	(**child).state = DIRTY;
}

void GA_clean_up(GA_Data * ga)
{


	int i;
	for (i = 0; i < (*ga).stack_size; ++i) {
		Path *path = (*ga).resources[i];
		free((*path).points);
		(*path).points = NULL;
		free(path);

	}
	stack_remove(&(*ga).head);
	free((*ga).resources);
	free((*ga).population.path);
	free(ga);
}

void GA_evolve(GA_Data * ga)
{
	int n;

	for (n = 0; n < (*ga).population.size; ++n) {
		GA_update_chromosome(&(*ga).population.path[n]);
	}

	int i;
	
	const int population_size = (*ga).population.size;
	Path path[population_size];

	Path *fittest1 = GA_fittest(ga);
	Path *fittest2 = GA_fittest(ga);

	path[0] = *fittest1;
	path[1] = *fittest2;
	GA_print_chromosome_data(fittest1);

	for (int ii = 2; ii < population_size; ii += 2) {
		Path *child1;
		Path *child2;
		GA_init_child(&child1, ga);
		GA_init_child(&child2, ga);
		path[ii + 0] = *child1;
		path[ii + 1] = *child2;
	}

	#pragma omp parallel for
	for (int ii = 2; ii < population_size; ii += 2) {
		//Init childs..
		Path *child1 = &path[ii + 0];
		Path *child2 = &path[ii + 1];

		//Select fathers                
		Path *father1 = GA_tournament_selection(&(*ga).population, ga);
		Path *father2 = GA_tournament_selection(&(*ga).population, ga);

		int prob = rand() % 100;

		if (prob < (*(*ga).parameters).prop_crossover) {
			//Crossover
			GA_crossover(father1, father2, child1, child2);
		} else {
			copy(father1, child1);
			copy(father2, child2);
		}

		//Mutation
		GA_mutation(child1, ga);
		GA_mutation(child2, ga);

		//Storing childs
		(*child1).state = DIRTY;
		(*child2).state = DIRTY;
	}

	for (int ii = 0; ii < population_size; ++ii) {
		GA_free_chromosome(&(*ga).population.path[ii], ga);
		(*ga).population.path[ii] = path[ii];
	}
}

void copy(Path * source, Path * dest)
{
	(*dest).size = (*source).size;
	(*dest).distance = (*source).distance;
	(*dest).fitness = (*source).fitness;
	(*dest).state = (*source).state;
	int i;
	for (i = 0; i < (*source).size; ++i) {
		(*dest).points[i] = (*source).points[i];
	}
}

void GA_free_chromosome(Path * path, GA_Data * ga)
{
	stack_push(&(*ga).head, path);
}

void GA_update_chromosome(Path * path)
{
	if((*path).state == DIRTY) {
		int n = 1;
		Point previous = (*path).points[0];
		float dist = 0;
		for (; n < (*path).size; ++n) {
			Point actual = (*path).points[n];
			dist += distance(&previous, &actual);
			previous = actual;
		}
		(*path).distance = dist;
		(*path).fitness = 1 / dist;
		(*path).state = UPDATED;
	}
}

Path *GA_get_fittest(GA_Data * ga)
{
	int n = 0;
	float fitness = 0;
	int index = 0;
	for (; n < (*ga).population.size; ++n) {
		GA_update_chromosome(&(*ga).population.path[n]);
		if (fitness < (*ga).population.path[n].fitness) {
			index = n;
			fitness = (*ga).population.path[n].fitness;
		}
	}
	Path *fit_copy;
	GA_create_child(&fit_copy, ga);
	copy(&(*ga).population.path[index], fit_copy);
	return fit_copy;
}

Path *GA_fittest(GA_Data * ga)
{
	int n = 0;
	float fitness;
	int index = 0;
	for (; n < (*ga).population.size; ++n) {
		GA_update_chromosome(&(*ga).population.path[n]);
		if (fitness < (*ga).population.path[n].fitness) {
			index = n;
			fitness = (*ga).population.path[n].fitness;
		}
	}
	Path *fit_copy;
	GA_init_child(&fit_copy, ga);
	copy(&(*ga).population.path[index], fit_copy);
	return fit_copy;
}

void GA_crossover(Path * father1, Path * father2, Path * child1,
				  Path * child2)
{
	
	int n;
	int higher = 0;
	int position = 0;
	for(n = 1; n < (*father1).size; ++n) {
		int dist1 = distance(&(*father1).points[n], &(*father1).points[n]);
		if(dist1 > higher) {
			higher = dist1;
			position = n;
		}
	}
	int pivot = position;
	for (n = 0; n < pivot; ++n) {
		(*child1).points[n] = (*father1).points[n];
		(*child2).points[n] = (*father2).points[n];
	}
	GA_crossover_fill_chromosome(child1, father2, pivot);
	GA_crossover_fill_chromosome(child2, father1, pivot);
}

void GA_crossover_fill_chromosome(Path * child, Path * father, int pivot)
{
	//Use a queue to avoid inArray search
	int n;
	int rest = 0;
	int found = 0;
	for (n = pivot; n < (*father).size; ++n) {
		Point actual = (*father).points[n];
		const int max = n - rest;
		if (inArray((*child).points, max, &actual) == 0) {
			(*child).points[max] = actual;
		} else {
			++rest;
		}
	}

	for (n = 0; n < (*father).size && rest; ++n) {
		Point actual = (*father).points[n];
		const int max = (*father).size - rest;
		if (inArray((*child).points, max, &actual) == 0) {
			(*child).points[max] = actual;
			--rest;
		}
	}
	(*child).state = DIRTY;
}

int inArray(Point * pointArray, int length, Point * toTest)
{
	int n = 0;
	for (; n < length; ++n) {
		if (equals(toTest, &pointArray[n])) {
			return 1;
		}
	}
	return 0;
}

int equals(Point * point1, Point * point2)
{
	return (*point1).x == (*point2).x && (*point1).y == (*point2).y;
}

void GA_mutation(Path * path, GA_Data * ga)
{
	int n = 0;
	for (; n < (*path).size; ++n) {
		if ((rand() % 100) < (*(*ga).parameters).prop_mutation) {
			int index;
			index = rand() % (*path).size;
			Point copy = (*path).points[n];
			(*path).points[n] = (*path).points[index];
			(*path).points[index] = copy;
			(*path).state = DIRTY;
		}
	}
}

Path *GA_tournament_selection(Population * population, GA_Data * ga)
{
	int length = (*population).size / 10;
	if (length == 0) {
		length = 1;
	}
	int i;
	int best_index;
	float best_fitness = 0;
	for (i = 0; i < length; ++i) {
		int index = rand() % (*ga).population.size;
		Path path = (*ga).population.path[index];
		GA_update_chromosome(&path);
		if (best_fitness < path.fitness) {
			best_index = index;
		}
	}
	return &(*ga).population.path[best_index];
}

void GA_start(GA_Data * ga)
{
	int i;
	for (i = 0; i < (*(*ga).parameters).iterations; ++i) {
		printf("Iteration\t[%5d] of [%5d]\n", i,
			   (*(*ga).parameters).iterations);
		GA_evolve(ga);
	}
}

void GA_print_chromosome_data(Path * path)
{
	//if ((*path).state == DIRTY) {
	GA_update_chromosome(path);
	// }
	printf("distance : %f, fitness : %f, size %d\n", (*path).distance,
		   (*path).fitness, (*path).size);
}

void GA_print_path(Path * path)
{
	GA_print_chromosome_data(path);


	int n = 0;
	for (n = 0; n < (*path).size; ++n) {
		printf("[%f, %f]\n", (*path).points[n].x, (*path).points[n].y);
	}
	printf("\n");
}

int distance(Point * point1, Point * point2)
{
	int x_dist = (*point1).x - (*point2).x;
	int y_dist = (*point1).y - (*point2).y;
	return sqrt(x_dist * x_dist + y_dist * y_dist);
}
