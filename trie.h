
#define AZTODO(w)	printf("[AZTODO] %s @ %d: %s\n", __FUNCTION__, __LINE__, w)
#define AZDEBUG(w)	printf("[AZ] %s @ %d: %s\n", __FUNCTION__, __LINE__, w)

#define ALPHABET_SIZE 26

typedef struct trie_node {
	char data; char* word;
	struct trie_node* children[ALPHABET_SIZE];
} trie_node;

typedef struct {
	trie_node* root; int size;
} trie;


trie_node* trienode_create(char data);
trie* trie_create();
void trie_insert(trie* t, char* word);
int trie_contains(trie* t, char* word);
int trie_contains_prefix(trie* t, char* prefix);
void trienode_print(trie_node* node);
void trie_print(trie* t);
void trie_print_prefix(trie* t, char* prefix);
void trie_free(trie* t);

