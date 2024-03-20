#include <stdio.h>

#include "trie.h"

static void print_usage()
{
        printf(
        " * Usage: lookup [OPTION] [ARGUMENT]\n"
        " * Options:\n"
        " *     p <prefix> Print all words with the given prefix\n"
        " *     c <prefix> Check if the prefix is in the trie\n"
        " *     w <word> Check if the word is in the trie\n"
        " *     (no option) Print all words in the trie\n");
}

int main(int argc, char *argv[])
{
	trie* my_trie = NULL;

	/* Create a trie */
	my_trie = trie_create();
	if (my_trie == NULL) {
		printf("Failed to create trie!\n");
		return 0;
	}
	
	/* Read words (one per line) from standard input, inserting each into the trie */
	AZTODO("Read words");	

	/* Parse the command-line parameters passed to the program (see the comment in lookup.c for how to do this) */
	AZTODO("Parse the command-line parameters");	

	/* Take the appropriate action as noted above */
	AZTODO("action");	

	/* Free your trie */
	trie_free(my_trie);

	printf("Exit from lookup!\n");
	return 0;
}
