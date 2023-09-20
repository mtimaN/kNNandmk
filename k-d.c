#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "k-d.h"
#include "utils.h"

/*
 * Remember: distance is actually euclidian distance squared for computational
 * reasons.
 */
static unsigned int distance(int *p1, int *p2, unsigned int k)
{
	unsigned int sum = 0;
	for (unsigned int i = 0; i < k; i++) {
		int diff = p1[i] - p2[i];
		sum += diff * diff;
	}
	return sum;
}

static kd_node_t *kd_node_create(int *point, size_t k)
{
	kd_node_t *kd_node = malloc(sizeof(kd_node_t));
	DIE(!kd_node, ERR);

	kd_node->left = NULL;
	kd_node->right = NULL;

	kd_node->point = malloc(sizeof(int) * k);
	DIE(!kd_node->point, ERR);
	memcpy(kd_node->point, point, k * sizeof(int));
	return kd_node;
}

kd_tree_t *kd_tree_create(int k)
{
	kd_tree_t *kd_tree;
	kd_tree = malloc(sizeof(kd_tree_t));
	DIE(!kd_tree, ERR);

	kd_tree->root = NULL;
	kd_tree->k = k;

	return kd_tree;
}

static kd_node_t *__kd_tree_insert(kd_tree_t *kd_tree, kd_node_t *root,
								   int *point, int depth)
{
	if (!root)
		return kd_node_create(point, kd_tree->k);
	int cd = depth % kd_tree->k;
	if (point[cd] < root->point[cd])
		root->left = __kd_tree_insert(kd_tree, root->left, point, depth + 1);
	else
		root->right = __kd_tree_insert(kd_tree, root->right, point, depth + 1);

	return root;
}

kd_node_t *kd_tree_insert(kd_tree_t *kd_tree, int *point)
{
	return __kd_tree_insert(kd_tree, kd_tree->root, point, 0);
}

static void __nn(kd_tree_t *kd_tree, kd_node_t *root, kd_node_t ***best,
				 int *n, unsigned int *min_dist, int *point, int depth)
{
	if (!root)
		return;
	int cd = depth % kd_tree->k;

	unsigned int dist = distance(root->point, point, kd_tree->k);
	if (dist < *min_dist) {
		*n = 1;
		free(*best);
		*best = malloc(sizeof(kd_node_t *));
		DIE(!best, ERR);
		(*best)[0] = root;
		*min_dist = dist;
	} else if (dist == *min_dist) {
		*n += 1;
		*best = realloc(*best, sizeof(kd_node_t *) * (*n));
		DIE(!best, ERR);
		(*best)[*n - 1] = root;
		*min_dist = dist;
	}

	kd_node_t *next = NULL;
	kd_node_t *opposite = NULL;
	if (point[cd] < root->point[cd]) {
		next = root->left;
		opposite = root->right;
	} else {
		next = root->right;
		opposite = root->left;
	}
	__nn(kd_tree, next, best, n, min_dist, point, depth + 1);
	if (abs(point[cd] - root->point[cd]) * abs(point[cd] - root->point[cd])
		< *min_dist)
		__nn(kd_tree, opposite, best, n, min_dist, point, depth + 1);
}

static int compar(const void *n1, const void *n2)
{
	kd_node_t *new_n1 = *(kd_node_t **)n1;
	kd_node_t *new_n2 = *(kd_node_t **)n2;
	int i = 0;
	while (new_n1->point[i] == new_n2->point[i])
		i++;
	return new_n1->point[i] - new_n2->point[i];
}

void nn(kd_tree_t *kd_tree, int *point)
{
	unsigned int min_dist = 999999999;
	kd_node_t **best = malloc(sizeof(kd_node_t *));
	DIE(!best, ERR);
	int n = 0;
	__nn(kd_tree, kd_tree->root, &best, &n, &min_dist, point, 0);
	qsort(best, n, sizeof(kd_node_t *), compar);
	for (int i = 0; i < n; ++i) {
		for (unsigned int j = 0; j < kd_tree->k; ++j)
			printf("%d ", best[i]->point[j]);
		printf("\n");
	}
	free(best);
}

static void __range_search(kd_tree_t *kd_tree, kd_node_t *root, int *min,
						   int *max, int depth)
{
	if (!root)
		return;
	int cd = depth % kd_tree->k;
	if (max[cd] < root->point[cd]) {
		__range_search(kd_tree, root->left, min, max, depth + 1);
	} else if (min[cd] > root->point[cd]) {
		__range_search(kd_tree, root->right, min, max, depth + 1);
	} else {
		__range_search(kd_tree, root->left, min, max, depth + 1);
		__range_search(kd_tree, root->right, min, max, depth + 1);
	}
	int ok = 1;
	for (unsigned int i = 0; i < kd_tree->k; ++i)
		if (min[i] > root->point[i] || max[i] < root->point[i]) {
			ok = 0;
			break;
		}
	if (ok) {
		for (unsigned int i = 0; i < kd_tree->k; ++i)
			printf("%d ", root->point[i]);

		printf("\n");
	}
}

void range_search(kd_tree_t *kd_tree, int *min, int *max)
{
	__range_search(kd_tree, kd_tree->root, min, max, 0);
}

static void __kd_tree_free(kd_node_t *kd_node)
{
	if (!kd_node)
		return;
	__kd_tree_free(kd_node->left);
	__kd_tree_free(kd_node->right);

	free(kd_node->point);
	free(kd_node);
}

void kd_tree_free(kd_tree_t *kd_tree)
{
	__kd_tree_free(kd_tree->root);
	free(kd_tree);
}
