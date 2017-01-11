#include "Datatypes.h"
#include "stdio.h"
#include "GA.h"

int main(int argc, char **argv)
{
	printf("Arguments size %d \n", argc);

	if(argc != 6) {
		printf("Invalid usage; ./executable population prop_mutation prop_crossover iterations path_size\n");
		return -1;
	}

	GA_Parameters parameters = { atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) };
	int p = atoi(argv[5]);
	Point points[p];
	int i;

	printf("Starting; number of points [%d], mutation = %d, crossover = %d, population %d, iterations %d", p, 
		 parameters.prop_mutation, parameters.prop_crossover, parameters.population, parameters.iterations);
	for (i = 0; i < p; ++i) {
		Point point = {i,i};
		points[i] = point;
	}
	//parameters, points, size
	GA_Data *ga = GA_init(&parameters, points, p);
	printf("Starting..\n");
	GA_start(ga);
	Path *path = GA_get_fittest(ga);
	printf("Printing out the result\n");
	GA_print_path(path);
	printf("Cleaning down..\n");
	GA_clean_up(ga);
}
