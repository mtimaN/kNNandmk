#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "trie.h"

void parse_commands(trie_t *trie)
{
	char command[15];
	char word[WORDMAX], fname[WORDMAX];
	while (scanf("%s", command)) {
		if (strcmp(command, "INSERT") == 0) {
			scanf("%s", word);
			trie_insert(trie, word);
		} else if (strcmp(command, "LOAD") == 0) {
			scanf("%s", fname);
			FILE *f = fopen(fname, "rt");
			if (!f) {
				printf("Couldn't load file!\n");
			} else {
				while (!feof(f)) {
					fscanf(f, "%s", word);
					trie_insert(trie, word);
				}
			}
			fclose(f);
		} else if (strcmp(command, "REMOVE") == 0) {
			scanf("%s", word);
			trie_remove(trie, word);
		} else if (strcmp(command, "AUTOCOMPLETE") == 0) {
			scanf("%s", word);
			int type;
			scanf("%d", &type);
			if (type == 1) {
				char *suggestion = autocomplete_one(trie, word);
				if (!suggestion)
					printf("No words found\n");
				else
					printf("%s\n", suggestion);
			} else if (type == 2) {
				char suggestion[WORDMAX];
				autocomplete_two(trie, word, suggestion);
				if (!suggestion[0])
					printf("No words found\n");
				else
					printf("%s\n", suggestion);
			} else if (type == 3) {
				char suggestion[WORDMAX];
				autocomplete_three(trie, word, suggestion);
				if (!suggestion[0])
					printf("No words found\n");
				else
					printf("%s\n", suggestion);
			} else if (type == 0) {
				char dummy[WORDMAX];
				strcpy(dummy, word);
				char *suggestion = autocomplete_one(trie, dummy);
				if (!suggestion)
					printf("No words found\n");
				else
					printf("%s\n", suggestion);
				strcpy(dummy, word);
				char suggestion2[WORDMAX];
				autocomplete_two(trie, dummy, suggestion2);
				if (!suggestion2[0])
					printf("No words found\n");
				else
					printf("%s\n", suggestion2);
				strcpy(dummy, word);
				autocomplete_three(trie, dummy, suggestion2);
				if (!suggestion2[0])
					printf("No words found\n");
				else
					printf("%s\n", suggestion2);
			}
		} else if (strcmp(command, "AUTOCORRECT") == 0) {
			int k;
			scanf("%s %d", word, &k);
			autocorrect(trie, word, k);
		} else if (strcmp(command, "EXIT") != 0) {
			printf("Invalid command\n");
		} else {
			return;
		}
	}
}

int main(void)
{
	char alphabet[] = ALPHABET;

	trie_t *trie = trie_create(ALPHABET_SIZE, alphabet);
	parse_commands(trie);
	trie_free(&trie);

	return 0;
}

