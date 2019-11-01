#include "game.h"

int user_num = 0;


// user* create_new_user(char *name){
// 	user* new_one = malloc(sizeof(user));
// 	//new_one -> name = malloc(strlen(name)*sizeof(char) + 1);
// 	strcpy(new_one->name, name);
// 	new_one -> max_score = 0;
// 	new_one -> total_games = 0;
// 	new_one -> total_score = 0;
// 	user_num ++;
// 	return new_one;
// }


// user* check_user(char* name){
// 	int i = 0;
// 	user* temp = NULL;
// 	user* curr = NULL;
// 	if(strlen(name) <= 8){
// 		while(i < strlen(name)){
// 			if(((47  < name[i]) && (name[i] < 58)) ||
// 				((64  < name[i]) && (name[i] < 91)) ||
// 				((96  < name[i]) && (name[i] < 123)) ||
// 				name[i] == 45 ||
// 				name[i] == 95){
// 				i++;
// 			}else{
// 				break;
// 			}
// 		}
// 		if(i == strlen(name)){
// 			curr = head;
// 			if(curr == NULL){
// 				user* new_user=  create_new_user(name);
// 				head = new_user;
// 				puts("Welcome to the boggle game, enjoy!");
// 				new_user -> next = NULL;
// 				return new_user;
// 			}else{
// 				while(1){
// 					if(curr == NULL){
// 						user* new_user = create_new_user(name);
// 						temp -> next = new_user;
// 						puts("Welcome to the boggle game, enjoy!");
// 						new_user -> next = NULL;
// 						return new_user;
// 					}
// 					if (strcmp((curr -> name), name) == 0){
// 						puts("Back to the game! Let's go!");
// 						return curr;
// 					}
// 					else{
// 						temp = curr;//to keep the last node when finishing iterating
// 						curr = curr -> next;
// 					}
// 				}
// 			}
// 		}else{
// 			printf("%s\n", "Please try to input a valid username");
// 			return NULL;
// 		}
// 	}else{
// 		printf("%s\n", "Please try to input a valid username");
//                 return NULL;
// 	}
// }

void user_update(user* curr_user, int score_in_one_game){
	(curr_user -> total_score) += score_in_one_game;
	(curr_user -> total_games)  += 1;
	if(score_in_one_game >= (curr_user -> max_score)){
		curr_user -> max_score = score_in_one_game;
	}
}


// void score_board(){
// 	user* curr = NULL;
// 	curr = head;
// 	while(curr != NULL){
// 		printf("--------------------\n");
// 		printf("%s:                 \n", curr -> name);
// 		printf("Total game is %d    \n", curr -> total_games);
// 		printf("Total score is %d   \n", curr -> total_score);
// 		printf("Best score is %d    \n", curr -> max_score);
// 		printf("--------------------\n");
// 		curr = curr -> next;
// 	}

//}

void score_board_single(user* curr){
	//printf("Head %s has played %d, his total socre is %d and best score is %d\n", head -> name, head -> total_games, head -> total_score, head -> max_score);

	printf("--------------------\n");
	printf("%s:                 \n", curr -> name);
	printf("Total game is %d    \n", curr -> total_games);
	printf("Total score is %d   \n", curr -> total_score);
	printf("Best score is %d    \n", curr -> max_score);
	printf("--------------------\n");
}

// void free_users(){
// 	//printf("enter into the free_user function\n");
// 	user* curr = NULL;
// 	curr = head;
// 	user* temp  =NULL;
// 	while(curr != NULL){
// 		temp = curr -> next;
// 		//free(curr -> name);
// 		free(curr);
// 		curr = temp;
// 	}
// }
