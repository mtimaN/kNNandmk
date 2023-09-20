#pragma once

typedef struct kd_node_t kd_node_t;
struct kd_node_t {
	kd_node_t *left, *right;
	int *point;
};

typedef struct kd_tree_t kd_tree_t;
struct kd_tree_t {
	kd_node_t *root;

	unsigned int k;
};

/**
 * @brief Dinamically allocates a new kd_tree and returns a pointer to it
 * @param k the dimension
 * @return kd_tree_t* pointer to the tree
 */
kd_tree_t *kd_tree_create(int k);

/**
 * @brief inserts a new point into the kd_tree
 * It acts somewhat like a BST tree but with every level it uses
 * point[level % k] as the reference value for the comparisons.
 * @param kd_tree pointer to the tree.
 * @param point the new point being inserted
 * @return kd_node_t* pointer to the node where the point was inserted
 */
kd_node_t *kd_tree_insert(kd_tree_t *kd_tree, int *point);

/**
 * @brief searches the nearest neighbour of the given point
 * It searches according to the current dimension but if it is possible
 * for a closer node to be on the other branch, it is called recusrively
 * on the other branch as well. If more points are equally close to the
 * given point then an array is formed. The array is then sorted using qsort
 * and the points are printed.
 * @param kd_tree the tree where the search occurs
 * @param point the point for which it finds the neighbour
 */
void nn(kd_tree_t *kd_tree, int *point);

/**
 * @brief prints every node within given range
 * It acts similar to kNN but prints every point within range
 * @param kd_tree pointer to the tree
 * @param min minimal coordinates
 * @param max maximal coordinates
 */
void range_search(kd_tree_t *kd_tree, int *min, int *max);

/**
 * @brief frees the tree
 * It calls a helper function which recursively frees every node
 * @param kd_tree the tree to be freed
 */
void kd_tree_free(kd_tree_t *kd_tree);
