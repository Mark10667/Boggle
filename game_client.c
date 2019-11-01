#include "game.h"
#include "game_server.h"

char board[4][4];
int score_in_one_game = 0;
int first_run_check = 1;
int check_n = 0;
user *head = NULL;
user* curr_user = NULL;
int roundnum = 0;

short cport = -1;
// static DNode* big_dictionary [BIG_HASH_SIZE];
// static DNode* small_dictionary [SMALL_HASH_SIZE];



int score(char* input){
        int length = strlen(input);
        if(length < 3){
                return 0;
        }else if((length == 3) || (length == 4)){
                return 1;
        }else if (length == 5){
                return 2;
        }else if(length == 6){
                return 3;
        }else if(length == 7){
                return 5;
        }else{
                return 11;
        }

}

int setup_client(){
	int fd;
	 struct sockaddr_in r;
	//char buf[MAX_LINE+1];

	 /* this is the same as before, except it says "INET" instead of "UNIX" */
	 if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			 perror("socket");
			 return(1);
	 }

	 /* This is slightly different: specify IP address and port number. */
	 memset(&r, '\0', sizeof(r));
	 r.sin_family = AF_INET;
	 //r.sin_addr.s_addr = htonl((127 << 24) | 1);
   r.sin_addr.s_addr = inet_addr("127.0.0.1");
	 r.sin_port = htons(cport);
	 /*
		* That address is 127.0.0.1 -- take the 127 and shift it to the left 24
		* bits so as to put it in the upper octet.  More commonly we would look
		* up a hostname with gethostbyname().
		*/

	 /* this is the same as before */
	 if (connect(fd, (struct sockaddr *)&r, sizeof(r)) < 0) {
			 perror("connect");
			 return(1);
	 }

	 return fd;
}

int main(int argc, char* argv[]){

	if (argc > 1)
		cport = (short)(atoi(argv[1]));
	else
		cport = PORT;

	int test_num_board;
	int test_num_dictionary;
	int test_num_exist;
//	int score_in_one_game = 0; //this is the score in one game, the user could have several games in one session.
	int current_score;//score for a certain input word
	int first_run_check = 1;//to escape the score part first and let the user input something first.
	//int first_run_user = 1;//to choose a certain user first
	char word[30] = "1";
	//char name[30];
	char *file_name = NULL;


	//word = (char*)malloc(30*sizeof(char));

	file_name = "wordlist.txt";
	int fd = setup_client();
  int len;
	char buf[MAX_LINE+1];
  memset(buf, '0', sizeof(buf));
	//sig_int();



  while(1){
    // if ((len = read(fd, buf, MAX_LINE)) < 0) {
    //     perror("read");
    // }
    // buf[len-1] = '\0';
    //
    // printf("%s\n", buf);
		if(fgets (buf, MAX_LINE, stdin)!=NULL){
      char *sen = strchr(buf, '\n');
      strcpy(&buf[sen-buf], "\r\n");
      len = write(fd, buf, strlen((buf)+1));
      if (len != strlen(buf)+1) {
  			printf("write %d:%ld", len, strlen(buf)+1);
      }else{
        printf("Client:Message being sent: %s\n",buf);
        // num = recv(fd, buf, sizeof(buf),0);
        // if ( num <= 0 )
        // {
        //         printf("Either Connection Closed or Error\n");
        //         //Break from the While
        //         break;
        // }
        //
        // buff[num] = '\0';
        // printf("Client:Message Received From Server -  %s\n",buffer);

        if(strcmp(buf, "new_game") == 0){
      		// if ((len = read(fd, buf, MAX_LINE)) < 0) {
      		// 	error("read");
      		// }else{
    		  // buf[MAX_LINE] = '\0';
    			if (!first_run_check){
    				test_num_board = final_check(word);
    				if (test_num_board){
    					//puts("test_num_board runs");
    					test_num_dictionary = check_dictionary(word, file_name);
    					//puts("check_dictionary runs");
    					if(test_num_dictionary){
    						test_num_exist = check_exist(word);
    						if(test_num_exist){
    							printf ("%s is a valid word\n", word);
    			        current_score = score(word);
    			       	score_in_one_game += current_score;
    			       	printf("Your score in this single game right now is %d\n", score_in_one_game);
    						}
    					}
    				}else{
    					test_num_dictionary = check_dictionary(word, file_name);
    					if(test_num_dictionary){
    						printf("Invalid word. It's in the dictionary, but not on this board\n");
    					}else{
    						printf("Invalid word. It's neither on dictionary nor on this board\n");
    					}
    				}
    			}

    	  	first_run_check = 0;
    	  	printf("Your input word is:\n('u' for log out, 'q' for quit game)\n");
    			//scanf("%s", word);
    			strcpy(word, buf);
    			char* vocab = to_upper(word);
    			strcpy(word, vocab);
    			free(vocab);
    			system("clear");
    			}
		   }
     }
   }

   close(fd);
	free_small_dictionary();
	free_big_dictionary();



	return 0;
}
