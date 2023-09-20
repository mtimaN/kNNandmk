#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "trie.h"
#include "utils.h"
#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

trie_node_t *trie_create_node(void)
{
	trie_node_t *trie_node = malloc(sizeof(trie_node_t));
	DIE(!trie_node, ERR);
	trie_node->freq = 0;
	for (int i = 0; i < 26; ++i)
		trie_node->children[i] = NULL;
	trie_node->n_children = 0;
	return trie_node;
}

trie_t *trie_create(int alphabet_size, char *alphabet)
{
	trie_t *trie = malloc(sizeof(trie_t));
	DIE(!trie, ERR);

	trie->root = trie_create_node();

	trie->alphabet_size = alphabet_size;
	trie->alphabet = alphabet;
	trie->no_nodes = 1;

	return trie;
}

static void __trie_insert(trie_t *trie, trie_node_t *node, char *key)
{
	if (strlen(key) == 0) {
		node->freq++;
		return;
	}
	trie_node_t *next_node = node->children[key[0] - trie->alphabet[0]];
	if (!next_node) {
		next_node = trie_create_node();
		node->children[key[0] - trie->alphabet[0]] = next_node;
		node->n_children++;
		trie->no_nodes++;
	}
	__trie_insert(trie, next_node, (key + 1));
}

void trie_insert(trie_t *trie, char *key)
{
	__trie_insert(trie, trie->root, key);
}

static trie_node_t *__trie_search(trie_t *trie, trie_node_t *node, char *key)
{
	if (key[0] == 0)
		return node;
	trie_node_t *next_node = node->children[key[0] - trie->alphabet[0]];
	if (!next_node)
		return NULL;
	return __trie_search(trie, next_node, (key + 1));
}

trie_node_t *trie_search(trie_t *trie, char *key)
{
	return __trie_search(trie, trie->root, key);
}

static char *__autocomplete_one(trie_node_t *root, char *word)
{
	if (root->freq > 0)
		return word;

	for (int i = 0; i < ALPHABET_SIZE; ++i) {
		if (root->children[i]) {
			char child = 'a' + i;
			strncat(word, &child, 1);
			return __autocomplete_one(root->children[i], word);
		}
	}
	return NULL;
}

char *autocomplete_one(trie_t *trie, char *word)
{
	trie_node_t *prefix = trie_search(trie, word);
	if (!prefix)
		return NULL;
	if (prefix->freq > 0)
		return word;
	return __autocomplete_one(prefix, word);
}

static void __autocomplete_two(trie_node_t *root, char *word, char *min_word,
							   unsigned int *min_len)
{
	if (strlen(word) < *min_len) {
		if (root->freq > 0) {
			strcpy(min_word, word);
			*min_len = strlen(word);
		}
		for (int i = 0; i < ALPHABET_SIZE; ++i) {
			if (root->children[i]) {
				char child = 'a' + i;
				strncat(word, &child, 1);
				__autocomplete_two(root->children[i], word, min_word, min_len);
				word[strlen(word) - 1] = '\0';
			}
		}
	}
}

void autocomplete_two(trie_t *trie, char *word, char *suggestion)
{
	suggestion[0] = '\0';
	trie_node_t *prefix = trie_search(trie, word);
	if (!prefix)
		return;
	if (prefix->freq > 0) {
		strcpy(suggestion, word);
		return;
	}
	unsigned int len = WORDMAX;
	__autocomplete_two(prefix, word, suggestion, &len);
}

static void __autocomplete_three(trie_node_t *root, char *word, int *freq_max,
								 char *suggestion)
{
	if (root->freq > *freq_max) {
		*freq_max = root->freq;
		strcpy(suggestion, word);
	}
	for (int i = 0; i < ALPHABET_SIZE; ++i) {
		if (root->children[i]) {
			char child = 'a' + i;
			strncat(word, &child, 1);
			__autocomplete_three(root->children[i], word, freq_max, suggestion);
			word[strlen(word) - 1] = '\0';
		}
	}
}

void autocomplete_three(trie_t *trie, char *word, char *suggestion)
{
	suggestion[0] = '\0';
	trie_node_t *prefix = trie_search(trie, word);
	if (!prefix)
		return;
	int mx = 0;
	__autocomplete_three(prefix, word, &mx, suggestion);
}

static void __autocorrect(trie_node_t *root, char *word, int k,
						  char *correction, int *ok)
{
	if (root->freq > 0 && strlen(word) == strlen(correction)) {
		int mistakes = 0;
		for (unsigned int i = 0; i < strlen(word); ++i) {
			if (word[i] != correction[i])
				mistakes++;
		}
		if (mistakes <= k) {
			printf("%s\n", correction);
			*ok = 1;
		}
	}
	if (strlen(correction) < strlen(word)) {
		for (int i = 0; i < ALPHABET_SIZE; ++i) {
			if (root->children[i]) {
				char child = 'a' + i;
				strncat(correction, &child, 1);
				__autocorrect(root->children[i], word, k, correction, ok);
				correction[strlen(correction) - 1] = '\0';
			}
		}
	}
}

void autocorrect(trie_t *trie, char *word, int k)
{
	char correction[50] = {0};
	int ok = 0;
	__autocorrect(trie->root, word, k, correction, &ok);
	if (!ok)
		printf("No words found\n");
}

static int __trie_remove(trie_t *trie, trie_node_t *node, char *key)
{
	if (strlen(key) == 0) {
		if (node->freq > 0) {
			node->freq = 0;
			return node->n_children == 0;
		} else {
			return 0;
		}
	}
	trie_node_t *next_node = node->children[key[0] - trie->alphabet[0]];
	if (next_node && __trie_remove(trie, next_node, (key + 1)) == 1) {
		free(next_node);
		node->children[key[0] - trie->alphabet[0]] = NULL;
		node->n_children--;
		trie->no_nodes--;
		if (node->n_children == 0 && node->freq == 0)
			return 1;
	}
	return 0;
}

void trie_remove(trie_t *trie, char *key)
{
	__trie_remove(trie, trie->root, key);
}

static void __trie_free(trie_node_t *root)
{
	if (!root)
		return;

	for (int i = 0; i < ALPHABET_SIZE; ++i)
		__trie_free(root->children[i]);

	free(root);
}

void trie_free(trie_t **trie)
{
	__trie_free((*trie)->root);
	free(*trie);
}
