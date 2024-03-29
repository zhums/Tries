#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

/*
 * Purpose
 * 	Creates a new trie node
 * Parameters
 *	data: The character to be stored in the data field of the node
 * What to Do
 *	Allocate memory for a new trie_node
 *	Initialize all children pointers to NULL
 *	Set its data field to the character passed as a parameter
 *	Set the word field to NULL
 * Returns
 *	A pointer to the newly-created trie_node
 */
trie_node* trienode_create(char data)
{
        trie_node *new_node;

	new_node = calloc(1, sizeof(*new_node));
        if (new_node)
		new_node->data = data;

	return new_node;
}

/*
 * Purpose
 *	Creates a new trie
 * Parameters
 *	None
 * What to Do
 *	Allocate memory for a new trie
 *	Initialize the root field to a new trie node that stores the special character $ as the data (used to denote the root node)
 *	Set the size of the trie to zero
 * Returns
 *	A pointer to the newly-created trie
 */
trie* trie_create()
{
	trie *new_trie;

	new_trie = (trie *)calloc(1, sizeof(*new_trie));
	if (new_trie) {
		trie_node *node;
		
		node = trienode_create('$');
		if (node == NULL) {
			free(new_trie);
			new_trie = NULL;
			goto out;
		}

		new_trie->root = node;
	}
out:
	return new_trie;
}

static trie_node * find_or_insert_node(trie_node *cur_node, char *word_ptr)
{
        trie_node *next_node = NULL;

        int children_idx = word_ptr[0] - 'a';

        // printf("find_or_insert_node %c children_idx %d\n", word_ptr[0], children_idx);
        next_node = cur_node->children[children_idx];

        if (next_node == NULL) {
                next_node = trienode_create(word_ptr[0]);
                if (next_node == NULL) {
                        return NULL;
		}
                cur_node->children[children_idx] = next_node;
        }

        ++word_ptr;
        if (word_ptr[0])
                cur_node = find_or_insert_node(next_node, word_ptr);
        else
                cur_node = next_node;

//        if (cur_node == NULL);
        return cur_node;
}

/*
 * Purpose
 *	Inserts a word into the trie, if it doesn't already exist
 * Parameters
 *	t: A pointer to the trie into which the word is to be inserted
 *	word: The word to be inserted into the trie
 * What to Do
 *	This is likely the "hardest" function, but we'll go through the logic step by step
 *	Create a pointer variable to a trie_node
 *		o This will represent the current node in the trie
 *		o Initialize it to point at the root node of the trie
 *	Create a pointer variable to a char
 *		o Initialize it to point at the first character of word
 *	For each character in word
 *		o Get the next character (dereference your pointer)
 *		o Compute its index in the children array of the current node
 *			 If the character is a = the array index is 0
 *			 If the character is b = the array index is 1, etc.
 *		o Check if the current node has a child at this index
 *			 If not, create a new trie node at this index for the given character using trienode_create
 *		o Descend in the trie to this child node
 *			 Hint: update your current node pointer
 *		o Move to the next character in the word
 *			 Hint: update your character pointer
 *	After all characters are processed
 *		o We are now at the child node that represents the end of the word. We wish to store the word in this node
 *		o Duplicate the word using strdup and store it in the node
 *			 This function handles dynamic allocation and copying of a string in one operation, but don't forget to free its memory later in your trie_free function
 *		o Increment the size field of the trie
 *	If the word already exists in the trie, this function should make no changes
 * Returns
 *	Nothing
 * Note
 *	This operation can be performed iteratively. If you prefer, you may also choose to create a helper function and perform it recursively
 */
void trie_insert(trie* t, char* word)
{
	trie_node *target_node;

//	printf("Insert %s\n", word);
	target_node = find_or_insert_node(t->root, word);

	if (target_node && target_node->word == NULL) {
		target_node->word = strdup(word);
		t->size++;
	}
}

static int trienode_contains(trie_node* node, char* word)
{
	int ret = 0;
	int children_idx;

        if (node == NULL)
                return 0;

        if (node->word && !strcmp(node->word, word))
                return 1;

        for (children_idx = 0; !ret && children_idx < ALPHABET_SIZE; children_idx++)
                ret = trienode_contains(node->children[children_idx], word);

        return ret;
}

/*
 * Purpose
 *	Checks if a word is piresent in the trie
 * Parameters
 *	t: A pointer to the trie in which to search for the word
 *	word: The word to search for in the trie
 * What to Do
 *	The logic of this function is like that of trie_insert, except we're not inserting a new node
 *	Instead, we iterate over each character in the input word, traversing the trie from the root node
 *	If a child node for the current character is not found, it means the word is not present in the trie
 *	If we reach the end of the word and the current node's word member is
 *		o Not NULL: the word is present in the trie
 *		o NULL: the word is not present in the trie (the path represents a prefix of another word but not a complete word itself)
 * Returns
 *	1 (true) if the word is found in the trie
 *	0 (false) otherwise
 * Example
 *	If the word cat exists in our trie, then trie_contains would return:
 *		o 1 for the word cat
 *		o 0 for the prefix ca
 */
int trie_contains(trie* t, char* word)
{
        return trienode_contains(t->root, word);
}

static int trienode_contains_prefix(trie_node* node, char* prefix)
{
        int ret = 0;
        int children_idx;

        if (node == NULL)
                return 0;

        if (node->word && !strncmp(node->word, prefix, strlen(prefix) - 1))
                return 1;

        for (children_idx = 0; !ret && children_idx < ALPHABET_SIZE; children_idx++)
                ret = trienode_contains_prefix(node->children[children_idx], prefix);

        return ret;
}

/*
 * Purpose
 *	Checks if a given prefix is present in the trie
 * Parameters
 *	t: A pointer to the trie in which to search for the prefix
 *	prefix: The prefix to search for in the trie
 * What to Do
 *	The logic of this function is similar to trie_contains, with the main difference being that we do not check if the current node represents the end of a word when we reach the end of the prefix.
 *	If we can successfully navigate through the trie, following the child nodes corresponding to each letter of the prefix, then the prefix exists in the trie, and we return 1
 * Returns
 *	1 (true) if the prefix is found in the trie
 *	0 (false) otherwise
 * Example
 *	If the word cat exists in our trie, then trie_contains_prefix would return
 *		o 1 for the prefix ca
 *		o 1 for the prefix cat (since a word is a prefix of itself)
 * Hint
 *	If you implement a helper function to find and return a pointer to the trie_node corresponding to a given string, then trie_contains and trie_contains_prefix literally become functions that are each 1-2 lines of code
 */
int trie_contains_prefix(trie* t, char* prefix)
{
        return trienode_contains_prefix(t->root, prefix);
}

/*
 * Purpose
 *	Recursiively prints (in ascending alphabetical order) all words in a trie starting from the given node
 * Parameters
 *	node: The current node being visited
 * What to Do
 *	If the given node represents the end of the word, print the word
 *	DO NOT PRINT ANYTHING ELSE -- just the word and a newline
 *	Recursively call the function on each of the node's non-NULL children
 * Returns
 *	Nothing
 */
void trienode_print(trie_node* node)
{
	int children_idx;

	if (node == NULL)
		return;

	if (node->word)
		printf("%s\n", node->word);

	for (children_idx = 0; children_idx < ALPHABET_SIZE; children_idx++)
		trienode_print(node->children[children_idx]);
}

/*
 * Purpose
 *	Recursively prints (in ascending alphabetical order) all words in a trie starting from the root node
 * Parameters
 *	t: The trie to print
 * What to Do
 *	Recursively prints the trie starting from its root node
 * Returns
 *	Nothing
 * Hint
 *	You have a helper function for this
 */
void trie_print(trie* t)
{
	trienode_print(t->root);
}

static void trienode_print_prefix(trie_node* node, char* prefix)
{
        int children_idx;

        if (node == NULL)
                return;

        if (node->word && !strncmp(node->word, prefix, strlen(prefix) - 1))
		printf("%s\n", node->word);

        for (children_idx = 0; children_idx < ALPHABET_SIZE; children_idx++)
                trienode_print_prefix(node->children[children_idx], prefix);
}

/*
 * Purpose
 *	Recursively prints (in ascending alphabetical order) all words in the given trie that begin with the given prefix
 * Parameters
 *	t: The trie from which to print words
 *	prefix: Prefix of words to print
 * What to Do
 *	Recursively prints all words in the trie with the given prefix
 *	If you implemented a helper function earlier to find the node corresponding to the end of a given word/prefix earlier, this function will be very short and easy
 *	Simply find the node corresponding to the given prefix, then print the trie starting from that node
 * Returns
 *	Nothing
 * Hint
 *	You have a helper function for this
 */
void trie_print_prefix(trie* t, char* prefix)
{
	trienode_print_prefix(t->root, prefix);
}

static void trienode_free(trie_node* node)
{
        int children_idx;

        if (node == NULL)
                return;

        if (node->word)
		free(node->word);

        for (children_idx = 0; children_idx < ALPHABET_SIZE; children_idx++)
                trienode_free(node->children[children_idx]);

	free(node);
}
/*
 * Purpose
 *	Frees all memory allocated for the given trie
 * Parameters
 *	t: A pointer to the trie to be freed
 * What to Do
 *	Frees the memory allocated for all nodes in the trie (don't forget the memory allocated for the words as well)
 *	Frees the memory allocated to the trie itself
 * Hint: you'll likely want a recursive helper function for this
 * Returns
 *	Nothing
 */
void trie_free(trie* t)
{
	if (t == NULL)
		return;

	trienode_free(t->root);

	free(t);
}






