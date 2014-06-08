#include <stdio.h>
#include <stdlib.h>
struct node {
	unsigned short int f; /*frequency*/
	unsigned short int v; /*value*/
	struct node *l; /*left node*/
	struct node *r; /*right node*/
	struct node *next; /*to build an ordered list*/
	
};

int bits_path;
unsigned short int found;

unsigned int *get_chars_frequency 	(FILE *);
struct node *get_huffman_tree 		(struct node **);
struct node *get_sorted_list 		(unsigned int *);
void find_value 			(struct node *, unsigned short int);
void print_ascii_statistics 		(unsigned int *);

int main (int argc, char **argv) {

	FILE *fp;
	unsigned int *ascii;
	unsigned short int i;
	struct node *huffman_tree;
	struct node *sorted_list;

	if (argc != 2) {
		printf ("Faltou um arquivo de texto para ser comprimido!\n");
		return -1;
	}

	printf ("Arquivo de entrada: %s\n", argv[1]);
	printf ("Arquivo de saida: %s.fck\n", argv[1]);

	fp = fopen (argv[1], "r");

	ascii = get_chars_frequency (fp);

	sorted_list = get_sorted_list (ascii);

	huffman_tree = get_huffman_tree (&sorted_list);

	for (i = 0; i < 128; i++)
		if(ascii[i] != 0) {
			bits_path = 0;
			found = 0;
			printf("procurando: %c\n", i);
			find_value (huffman_tree, i);
		}

	print_ascii_statistics (ascii);
	
	printf ("values:\n");
	for(i = 0; i < huffman_tree->vi; i++) {
		printf ("%c", huffman_tree->vs[i]);
	}

	fclose (fp);

	return 0;
}

void find_value (struct node *huffman_tree, unsigned short int v) 
{
	if(huffman_tree != NULL) {
		if (huffman_tree->v == v) {
			printf ("Achou!\n");
			found = 1;
			printf ("path: %d\n", bits_path);
			return;
		} else {
			if (huffman_tree->l != NULL){
				if (found == 0) {
					printf ("e");
					bits_path = bits_path<<1;
					bits_path++;
				}
				find_value (huffman_tree->l, v);
			}
			if (huffman_tree->r != NULL){
				if (found == 0) {
					printf ("d");
					bits_path = bits_path>>1;
					bits_path = bits_path<<1;
				}
				find_value (huffman_tree->r, v);
			}
		}
	}
}

unsigned int *get_chars_frequency (FILE *fp)
{
	unsigned int *ascii = malloc (128*sizeof(int));
	char c;

	while ( fread (&c, 1, 1, fp) )
		ascii[ (int) c]++;

	return ascii;
}

void insert (struct node *, struct node **);
struct node *get_sorted_list (unsigned int *ascii) 
{
	struct node *sorted_list = NULL;
	unsigned short int i;

	for (i = 0; i < 128; i++)
		if (ascii[i] != 0) {
			struct node *new = malloc (sizeof(struct node));
			new->f = ascii[i];
			new->v = i;
			new->l = NULL;
			new->r = NULL;
			new->next = NULL;

			insert (new, &sorted_list);
		}

	return sorted_list;
}
void insert (struct node *new, struct node **list) 
{
	if ( *list == NULL) {
		*list = new;
	} else {
		struct node *aux, *prev;
		unsigned short int found = 0;
		
		for (aux = *list, prev = aux; aux != NULL && found == 0; prev = aux, aux = aux->next) {
			if (new->f <= aux->f) {
				if (aux == *list) {
					new->next = aux;
					*list = new;
					found = 1;
				} else {
					new->next = prev->next;
					prev->next = new;
					found = 1;
				}
			}
		}
		if (found == 0) {
			prev->next = new;
		}
	}
	struct node *aux;
	for (aux = *list; aux != NULL; aux = aux->next) 
		printf ("%d ", aux->f);
	printf("\n");
}

struct node *get_huffman_tree (struct node **list)
{
	struct node *huffman_tree = *list;
	struct node *new;
	int i;
	
	while (huffman_tree->next != NULL) {
		new = malloc (sizeof(struct node));
		new->f = huffman_tree->f + huffman_tree->next->f;
		new->v = 129;
		new->l = huffman_tree;
		new->r = huffman_tree->next;
		new->next = NULL;
		huffman_tree = huffman_tree->next->next;
		insert (new, &huffman_tree);
	}
	return huffman_tree;
}

void print_ascii_statistics (unsigned int *ascii) 
{
	unsigned short int i;
	for (i = 0; i < 128; i++) 
		if (ascii[i] != 0) 
			printf ("(%i) %c: %d\n", i, i, ascii[i]);
}
