#include "game.h"
#include "game_server.h"



char* to_upper(char* word){
	int length  = strlen(word);
	char* vocab = calloc(length + 1, sizeof(char));
	int i = 0;
	while(word[i] != '\0'){
		if(word[i] >= 'a' && word[i] <= 'z'){
			vocab[i] = word[i] - 32;
		}else if(word[i] >= 'A' && word[i] <= 'Z'){
			vocab[i] = word[i];
		}else{
			continue;
		}
		i++;
	}
	vocab[i] = '\0';
	return vocab;
}

int** checker(){
        int testrow, testcol;
        int **testboard = calloc(4, sizeof(int*));
        for(int i = 0; i < 4; i ++){
                testboard[i] = calloc(4, sizeof(int));
        }
        for ( testrow = 0; testrow < 4; testrow++ ) {

                 for ( testcol = 0; testcol < 4; testcol++ ) {
                      testboard[testrow][testcol] = 0;
                    }

        }
	return testboard;

}

void free_testboard(int ** testboard){
        for(int i = 0; i < 4; i ++){
                free(testboard[i]);
        }
        free(testboard);
}

int find_word(char* input,int stri,int** testboard,int i, int j, int found){

        int length = strlen(input);

        if(stri == length){
//		printf("%d\n",stri);
                return length;
        }


	for(int row=i-1; row<=i+1 && row < 4; row++ ){
		for(int col=j-1; col<=j+1 && col < 4; col++){
       			if(row>=0 && col>=0 && input[stri] == board[row][col] && !testboard[row][col] && !found){
        	        		testboard[row][col] = 1;
//					printf("%d:%d,%d|%d\n",stri,row,col,length);
        	        		found = find_word(input, stri+1, testboard, row, col, found);
					if (!found)
						testboard[row][col]=0;


        	        }
		}
	}
	return found;
}

int check_board(char* input){
//	printf("%s\n",input);
	int len = strlen(input);
	if(len <= 16){
		int** testboard = NULL;
		int found=0;
		testboard = checker();
		int stri = 0;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				if((board[i][j] == input[stri]) && (testboard[i][j] == 0)&& !found){
	//				printf("%d\n",stri);
					testboard[i][j] = 1;
					found = find_word(input, stri+1, testboard, i, j, found);
					if (!found)
	          testboard[i][j]=0;
				}
			}
		}
		free_testboard(testboard);
		return found;
	}	else{
		return 0;
	}
}


int final_check(char* input){
	int result;
	result = check_board(input);
	int length = strlen(input);
  char* sameinput = calloc(length, sizeof(char));
	if(result == 0){
//		sameinput[0] = input[0];
//		int newindex = 1;
//		for(int i = 1; i < length; i++){
//			if(!((sameinput[newindex-1] == 'Q')&&(input[i] == 'U'))){
//				sameinput[newindex] = input[i];
//				newindex++;
//			}
//		}
		for(int i = 0; i < (length - 1); i++){
			if((input[i] == 'Q') && (input[i+1] == 'U')){
				for(int j = 0; j < i+1; j++){
					sameinput[j] = input[j];
				}
				for(int k = i+1; k < (length-1); k++){
					sameinput[k] = input[k+1];
				}
			}
		}
		sameinput[length-1] = '\0';
//		printf("%s:%d\n",sameinput,(int)strlen(sameinput));
		result = check_board(sameinput);
	}
	free(sameinput);
	return result;
}
