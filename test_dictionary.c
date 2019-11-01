#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
#define MAX_LINE 100

//capitalizes all letters in string s in place
void capitalize(char* s){
   int i = 0;
   while(s[i]) {
	  unsigned char c = (unsigned char ) s[i];
      s[i] = toupper(c);
      i++;
   }
}

int main (int argc, char ** argv) {
	int i;
	FILE *input_FP;
	char line [MAX_LINE];
	char * file_name;
	DNode* result;
	static DNode* big_dictionary [BIG_HASH_SIZE];
	static DNode* small_dictionary [SMALL_HASH_SIZE];
	

	if (argc < 2) {
		fprintf (stderr, "test_dictionary <dictionary file name>\n");
		return 1;
	}
	file_name = argv [1];

	if(!(input_FP = fopen ( file_name , "r" )))    {
        fprintf(stderr,"Could not open file \"%s\" for reading dictionary words\n", file_name);
        return 1;
    }

	while( fgets (line, MAX_LINE, input_FP)!=NULL ) {
		line[strcspn(line, "\r\n")] = '\0';  //trim new line characters
		insert (big_dictionary, BIG_HASH_SIZE, line);
	}	
	fclose (input_FP);

	result = lookup (big_dictionary, BIG_HASH_SIZE, "APE");
	if (result != NULL) 
		printf ("<APE> is in the dictionary\n");
	else
		printf ("<APE> not found\n");
	
	result = lookup (big_dictionary, BIG_HASH_SIZE, "MACAC");
	if (result != NULL) 
		printf ("<MACAC> is in the dictionary\n");
	else
		printf ("<MACAC> not found\n");

	free_dictionary(big_dictionary, BIG_HASH_SIZE);

	printf ("\n new game session: 1\n");
	//now testing small dictionary - twice - with cleanup
	char key[] = "arc";
	capitalize(key);
	for (i =0; i< 5; i++) {
		
		result = lookup (small_dictionary, SMALL_HASH_SIZE, key);
		if (result == NULL) {
			insert (small_dictionary, SMALL_HASH_SIZE, key);
			printf ("Successfully inserted %s in session 1\n", key);
		}
		else
			printf ("%s has been already used in session 1\n", key);
	}
		
	free_dictionary(small_dictionary, SMALL_HASH_SIZE);

	printf ("\n new game session: 2\n");	
	for (i =0; i< 5; i++) {
		result = lookup (small_dictionary, SMALL_HASH_SIZE, key);
		if (result == NULL){
			insert (small_dictionary, SMALL_HASH_SIZE, key);
			printf ("Successfully inserted %s in session 2\n", key);
		}
		else
			printf ("%s has been already used in session 2\n", key);
	}
		
	free_dictionary(small_dictionary, SMALL_HASH_SIZE);

	return 0;
}
