#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "trie.h"

#define MAX_INPUT_SIZE	1 << 20

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

static void read_words(trie* my_trie)
{
	char buf[MAX_INPUT_SIZE];
	char* token;
	char* rest;

	memset(buf, 0, sizeof(buf));
	read(0, buf, MAX_INPUT_SIZE);
	rest = buf;
	while ((token = strtok_r(rest, "\n\t ", &rest))) {
		trie_insert(my_trie, token);
	}
}

typedef struct {
	char cmd;
	char *cmd_value;

} command_pair;

command_pair lookup;

static void parse_command_opt(int argc, char *argv[])
{
	if (argc == 1) {
		lookup.cmd = 'n';
	} else if (argc == 3) {
		if (!strcmp(argv[1], "p"))
			lookup.cmd = 'p';
		else if (!strcmp(argv[1], "c"))
                        lookup.cmd = 'c';
		else if (!strcmp(argv[1], "w"))
                        lookup.cmd = 'w';
                lookup.cmd_value = argv[2];
	}
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
	read_words(my_trie);

	/* Parse the command-line parameters passed to the program (see the comment in lookup.c for how to do this) */
	parse_command_opt(argc, argv);

	/* Take the appropriate action as noted above */
	switch (lookup.cmd) {
	case 'p':
		trie_print_prefix(my_trie, lookup.cmd_value);
		break;
	case 'c':
		printf("Prefix %s: %d\n", lookup.cmd_value, trie_contains_prefix(my_trie, lookup.cmd_value)? 1 : 0);
                break;
	case 'w':
		printf("Word %s: %d\n", lookup.cmd_value, trie_contains(my_trie, lookup.cmd_value)? 1 : 0);
                break;
	case 'n':
		trie_print(my_trie);
                break;
	default:
		print_usage();
	}

	/* Free your trie */
	trie_free(my_trie);

	return 0;
}
