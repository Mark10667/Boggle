#ifndef GAME_H
#define GAME_H

//include
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
//dictionary define
#define BIG_HASH_SIZE 20000
#define SMALL_HASH_SIZE 100

//signal define
#define TIME_INTERVAL 60

//user.c
#define NAME_LENGTH 9

//game_client.C
#define MAX_LINE 80
//dictionary struct
typedef struct d_node {
	char* key;
    struct d_node *next;
}DNode;

//user struct
typedef struct user{
	char name[NAME_LENGTH];
	int max_score;
	int total_games;
	int total_score;
	struct user  *next;
}user;

//dictionary variable
char * copystr (const char *);
unsigned hash(const char *s);

//in boggle.case
//void board_generator();
int score(char* input);
// void clean_board();
// void print_board();
int setup_client();

//in check_board.c
char* to_upper(char*);
int** checker();
int final_check(char* );
int check_board(char* );
int find_word(char* ,int ,int** ,int , int , int);



//in user.c
user* create_new_user(char*);
//void append(user **, user *);
user* check_user(char*);
void user_update(user*, int);
//int write_file();
// user* read_file();
void score_board();
void score_board_single(user*);
void free_users();

//in dictionary.c
unsigned hash(const char *);
DNode * lookup (DNode ** dictionary, int hash_size, const char *key);
DNode * insert (DNode ** dictionary, int hash_size, const char * key);
//void free_dictionary (DNode ** dictionary, int hash_size);
char *copystr(const char *) ;

//in check_dictionary.c
int check_dictionary(char*, char*);
int check_exist(char*);
void free_small_dictionary();
void free_big_dictionary();

//in signal.case
void handle_al(int);
void sig_al();
void handle_int(int);
void sig_int();


extern user *head;
extern int score_in_one_game;
extern int first_run_check;
extern int check_n;
extern int user_num;
extern user* curr_user;
extern int roundnum;
//extern char* wordlist;

#endif
