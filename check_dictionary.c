#include "game.h"
#define WORD_MAX_LINE 100

//remeber to capitalize first!


static DNode* big_dictionary [BIG_HASH_SIZE];
static DNode* small_dictionary [SMALL_HASH_SIZE];

int check_dictionary(char* word, char* file_name) {
	FILE *input_FP = NULL;
	char line [WORD_MAX_LINE];
	DNode* result;

	if(!(input_FP = fopen ( file_name , "r" )))    {
        fprintf(stderr,"Could not open file \"%s\" for reading dictionary words\n", file_name);
        return 1;
    }

	while( fgets (line, WORD_MAX_LINE, input_FP)!=NULL ) {
		line[strcspn(line, "\r\n")] = '\0';  //trim new line characters
		insert (big_dictionary, BIG_HASH_SIZE, line);
	}
	fclose (input_FP);

	result = lookup (big_dictionary, BIG_HASH_SIZE, word);
	if (result != NULL){
		printf ("%s is in the dictionary\n", word);

    return 1;
  }
	else{
		printf ("%s is not found in the dictionary\n", word);

    return 0;
  }
	//free_dictionary(big_dictionary, BIG_HASH_SIZE);

}


int check_exist(char* word) {
    // const char vocab;
    // vocab = word;
  	DNode* result;


    //now testing small dictionary - twice - with cleanup
    char *key = word;

    result = lookup (small_dictionary, SMALL_HASH_SIZE, key);
    if (result == NULL) {
      insert (small_dictionary, SMALL_HASH_SIZE, key);
      printf ("Successfully inserted %s in this session\n", key);
      //free_dictionary(small_dictionary, SMALL_HASH_SIZE);
      return 1;
    }
    else{
      printf ("%s has been already used in this session\n", key);
      return 0;
    }
}



void free_big_dictionary () {
	int i;
	for (i=0; i<BIG_HASH_SIZE; i++) { //iterate over hash array
		if (big_dictionary [i]!=NULL) { //if there is an entry at position i
			DNode *head = big_dictionary[i]; //find the head of the linked list
			DNode *current = head;
			while (current != NULL) {
				DNode * temp = current;
				current = current->next;
                if (temp->key !=NULL)
                    free (temp->key);
				free (temp);
			}
			big_dictionary[i] = NULL;  //BUG fix
		}
	}
}

void free_small_dictionary () {
	int i;
	for (i=0; i<SMALL_HASH_SIZE; i++) { //iterate over hash array
		if (small_dictionary [i]!=NULL) { //if there is an entry at position i
			DNode *head = small_dictionary[i]; //find the head of the linked list
			DNode *current = head;
			while (current != NULL) {
				DNode * temp = current;
				current = current->next;
                if (temp->key !=NULL)
                    free (temp->key);
				free (temp);
			}
			small_dictionary[i] = NULL;  //BUG fix
		}
	}
}
