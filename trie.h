#pragma once
#include <stdlib.h>

/*
 * The structs are taken from the lab
*/

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define WORDMAX 50

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
	/* 1 if current node marks the end of a word, 0 otherwise */
	int freq;

	trie_node_t *children[26];
	int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
	trie_node_t *root;

	/* Number of keys */
	int size;

	/* Trie-Specific, alphabet properties */
	int alphabet_size;
	char *alphabet;

	/* Optional - number of nodes, useful to test correctness */
	int no_nodes;
};

/**
 * @brief creates a node for the trie
 *
 * @return trie_node_t* returns the created node
 */
trie_node_t *trie_create_node(void);

/**
 * @brief creates a trie
 *
 * @param alphabet_size size of the alphabet used
 * @param alphabet the alphabet used
 * @return trie_t* returns a pointer to the allocated trie
 */
trie_t *trie_create(int alphabet_size, char *alphabet);

/**
 * @brief inserts a key into the trie
 *
 * @param trie
 * @param key
 */
void trie_insert(trie_t *trie, char *key);
/**
 * @brief search whether a prefix exists in the trie
 *
 * @param trie
 * @param key
 * @return trie_node_t* the node of the last character of the prefix
 */
trie_node_t *trie_search(trie_t *trie, char *key);

/**
 * @brief autocompletes with the first word lexicographically
 * Done using a DFS which stops after finding the word.
 * @param trie
 * @param word
 * @return char* a pointer to a string containing the word
 */
char *autocomplete_one(trie_t *trie, char *word);

/**
 * @brief autocompletes with the shortest word possible
 * Done using a DFS which checks whether every word found is shorter than the
 * previous one.
 * @param trie
 * @param word
 * @param suggestion the autocompletion
 */
void autocomplete_two(trie_t *trie, char *word, char *suggestion);

/**
 * @brief autocompletes with the most used word
 * Done using a DFS which checks whether every word found is more used than the
 * previous best
 * @param trie
 * @param word
 * @param suggestion the autocompletion
 */
void autocomplete_three(trie_t *trie, char *word, char *suggestion);

/**
 * @brief autocorrects the given word with one that exists in the trie
 * Done with a DFS which checks every word with the same length as the one
 * given and displays every possible word that differs with at most k
 * characters
 * @param trie
 * @param word original word
 * @param k how much at most the correction should differ from the original
 */
void autocorrect(trie_t *trie, char *word, int k);

/**
 * @brief removes a word from the trie
 * It removes the word and also every letter from the word that isn't part of
 * other words
 * @param trie
 * @param key the word to be deleted
 */
void trie_remove(trie_t *trie, char *key);

/**
 * @brief frees the trie
 *
 * @param trie pointer to the address of the trie
 */
void trie_free(trie_t **trie);
