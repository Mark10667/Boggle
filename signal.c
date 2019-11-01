#include "game.h"

// handle SIGALRM
// void handle_al(int code){
//   //printf("%s, Your score on the last single game is %d\n", curr_user -> name, score_in_one_game);
//   //printf("Your score on the last single game is %d\n", score_in_one_game);
//   //board_generator();
//   //printf("The board is reshuffled and next game begins\n");
//   //first_run_check = 1;
//   check_n = 1;
//   alarm(TIME_INTERVAL);
//
// }
//
// void sig_al(){
//   struct sigaction newact_al;
//   newact_al.sa_handler = handle_al;
//   newact_al.sa_flags = SA_RESTART;
//   sigemptyset(&newact_al.sa_mask);
//   sigaction(SIGALRM, &newact_al, NULL);
// }


// handle SIGINT


void handle_int(int code){
  // add the score_in_one_game to total_score
  // to decide if this interupted game has max_score
  printf("\n");
  //user_update(curr_user, score_in_one_game);
  //score_board();
  //write_file();

  free_small_dictionary();
  free_big_dictionary();

  puts("See you next time :) ");

  exit(1);
}

void sig_int(){
  struct sigaction newact_int;
  newact_int.sa_handler = handle_int;
  newact_int.sa_flags = 0;
  sigemptyset(&newact_int.sa_mask);
  sigaction(SIGINT, &newact_int, NULL);
}
