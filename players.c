#define _GNU_SOURCE
#include "game_server.h"

int player_num = 0;

//TODO - implement this
/*
 * Create a new player with the given name.  Insert it at the tail of the list
 * of players whose head is pointed to by *player_ptr_add.
 */
 Player* create_new_player(const char *name){
 	Player* new_one = malloc(sizeof(Player));
 	//new_one -> name = malloc(strlen(name)*sizeof(char) + 1);
 	strcpy(new_one->name, name);
 	new_one -> max_score = 0;
 	//new_one -> total_games = 0;
 	new_one -> total_score = 0;
 	player_num ++;
 	return new_one;
 }

int create_player(const char *name, Player **player_ptr_add) {

  	int i = 0;
  	Player* temp = NULL;
  	Player* curr = NULL;
  	if(strlen(name) <= 8){
  		while(i < strlen(name)){
  			if(((47  < name[i]) && (name[i] < 58)) ||
  				((64  < name[i]) && (name[i] < 91)) ||
  				((96  < name[i]) && (name[i] < 123)) ||
  				name[i] == 45 ||
  				name[i] == 95){
  				i++;
  			}else{
  				break;
  			}
  		}
  		if(i == strlen(name)){
  			curr = *player_ptr_add;
  			if(curr == NULL){
  				Player* new_user=  create_new_player(name);
  				*player_ptr_add = new_user;
  				new_user -> next = NULL;
  				return 0; // new player
  			}else{
  				while(1){
  					if(curr == NULL){
  						Player* new_user = create_new_player(name);
  						temp -> next = new_user;
  						new_user -> next = NULL;
  						return 0; //new player
  					}
  					if (strcmp((curr -> name), name) == 0){
              //back to game
  						return 1;
  					}
  					else{
  						temp = curr;//to keep the last node when finishing iterating
  						curr = curr -> next;
  					}
  				}
  			}
  		}else{
  			return 2;
  		}
  	}else{
                  return 2;
  	}


   //return 0;
}

//TODO - implement this
/*
 * Return a pointer to the player with this name in
 * the list starting with head. Return NULL if no such player exists.
 */
Player *find_player(const char *name, Player *head) {
    Player * curr = NULL;
    curr = head;
    while(curr != NULL){
      if(strcmp(curr -> name, name) == 0){
        return curr;
      }else{
        curr = curr -> next;
      }
    }
    return NULL;
}

void free_players(Player *player_list){
	//printf("enter into the free_user function\n");
	Player* curr = NULL;
	curr = player_list;
	Player* temp  =NULL;
	while(curr != NULL){
		temp = curr -> next;
		//free(curr -> name);
		free(curr);
		curr = temp;
	}
}

int write_file(Player* player_list){
	FILE *fp = NULL;
	int result;
	fp = fopen("player.bin", "wb");
	if(fp == NULL){
			printf("Cannot open file for writing\n");
			return 1;
	}

	//user User[user_num];
	Player* players = NULL;
	players = malloc(sizeof(Player)*player_num);
	Player* curr = NULL;
	curr = player_list;
	int i = 0;
	while(curr != NULL){
		players[i] = *curr;
		i++;
		curr = curr -> next;
	}

	// for(int i = 0; i < user_num; i++){
	// 	printf("User %s has played %d, his total socre is %d and best score is %d\n", (&User[i]) -> name, (&User[i]) -> total_games, (&User[i]) -> total_score, (&User[i]) -> max_score);
	// }
	// printf("user_num is %d\n", user_num);
	fwrite(&player_num, sizeof(int), 1, fp);
	result = fwrite(players, sizeof(Player), player_num, fp);
	if (result != player_num) {
				printf ("Not all players were written: wanted to write %d, but fwrite returned %d\n", player_num, result);
				return 1;
	}
	fclose(fp);
	//function in user.c
	free(players);
	return 0;
}

Player* read_file(){
	FILE *rp = NULL;
	int result_num, result_array_num;
	if((rp = fopen("player.bin", "rb"))){
		if((!rp)){
				printf("Cannot open file for reading\n");
				//return 1;
		}

		result_num = fread(&player_num, sizeof(int), 1, rp);
		if(result_num != 1){
				puts("fail to read player_num");
		}
		//user User[user_num];
		Player* player_array = NULL;
		player_array = malloc(sizeof(Player)*player_num);

		result_array_num = fread(player_array, sizeof(Player), player_num, rp);
		if (result_array_num != player_num) {
					printf ("Not all players were read: wanted to read %d, but fread returned %d\n", player_num, result_array_num);
					//return 1;
		}
		fclose(rp);
		int j = 1;
		Player* a1 = malloc(sizeof(player_array[0]));
		*a1 = player_array[0];

		//printf("Head %s has played %d, his total socre is %d and best score is %d\n", head -> name, head -> total_games, head -> total_score, head -> max_score);
		Player* curr = NULL;
		curr = a1;
		printf("how many players? %d\n", player_num);
		for(int i = 0; i < player_num; i++){
			printf("Player %s has played %d, his total socre is %d and best score is %d\n", (&player_array[i]) -> name, (&player_array[i]) -> total_games, (&player_array[i]) -> total_score, (&player_array[i]) -> max_score);
		}
		while(j< player_num){
			Player* a = malloc(sizeof(Player));
			*a = player_array[j];
			curr -> next = a;
			curr = curr -> next;
			j++;
		}
		curr -> next = NULL;
		//printf("Head %s has played %d, his total socre is %d and best score is %d\n", head -> name, head -> total_games, head -> total_score, head -> max_score);
		free(player_array);
		return a1;
		}
		return 0;
}
