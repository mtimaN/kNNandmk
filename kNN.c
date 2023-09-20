#include <stdio.h>
#include <string.h>

#include "k-d.h"
#include "utils.h"

int main(void)
{
	unsigned int n, k;
	int *point, *min, *max;
	char command[10];
	kd_tree_t *kd_tree;
	scanf("%s", command);
	if (strcmp(command, "LOAD") != 0)
		printf("Program should start with LOAD\n");

	char fname[50];
	scanf("%s", fname);
	FILE *f = fopen(fname, "rt");
	fscanf(f, "%u %u", &n, &k);
	kd_tree = kd_tree_create(k);
	point = malloc(k * sizeof(int));
	DIE(!point, ERR);
	min = malloc(k * sizeof(int));
	DIE(!min, ERR);
	max = malloc(k * sizeof(int));
	DIE(!max, ERR);
	for (unsigned int i = 0; i < n; ++i) {
		for (unsigned int j = 0; j < k; ++j)
			fscanf(f, "%d", &point[j]);

		kd_tree->root = kd_tree_insert(kd_tree, point);
	}
	fclose(f);
	while (scanf("%s", command)) {
		if (strcmp(command, "LOAD") == 0) {
		} else if (strcmp(command, "NN") == 0) {
			for (unsigned int i = 0; i < k; ++i)
				scanf("%d", &point[i]);
			nn(kd_tree, point);
		} else if (strcmp(command, "RS") == 0) {
			for (unsigned int i = 0; i < k; ++i)
				scanf("%d %d", &min[i], &max[i]);
			range_search(kd_tree, min, max);
		} else if (strcmp(command, "EXIT") == 0) {
			break;
		}
	}
	free(min);
	free(max);
	free(point);
	kd_tree_free(kd_tree);
	return 0;
}
