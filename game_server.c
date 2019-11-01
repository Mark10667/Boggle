#define _GNU_SOURCE
#include "game_server.h"
#include "game.h"


Player *player_list = NULL;

Client *client_list = NULL;

short port = -1;

char board[4][4];

int main(int argc, char* argv[]) {
    struct client *p;
	if (argc > 1)
		port = (short)(atoi(argv[1]));
	else
		port = PORT;

	//get listen file descriptor
    int listenfd = setup();
    player_list = read_file();
    //TODO install timer signal handler
    struct sigaction newact_al;
    newact_al.sa_handler = timer_handler;
    newact_al.sa_flags = SA_RESTART;
    sigemptyset(&newact_al.sa_mask);
    sigaction(SIGALRM, &newact_al, NULL);
	//start timer
    alarm(TIMER_TICK);

	//TODO: implement select()
    while (1) {
      struct timeval tv;
      int maxfd;
      fd_set fdlist;
      maxfd = listenfd;
      FD_ZERO(&fdlist);
      FD_SET(listenfd, &fdlist);
      if(client_list != NULL){
        for (p = client_list; p; p = p->next) {
            FD_SET(p->fd, &fdlist);
            if (p->fd > maxfd)
                maxfd = p->fd;
        }
      }
      if (select(maxfd + 1, &fdlist, NULL, NULL, &tv) < 0) {
        perror("select");
      } else {
        for (p = client_list; p; p = p->next)
            if (FD_ISSET(p->fd, &fdlist))
                break;
        if (p)
            receiveclient(p);
        if (FD_ISSET(listenfd, &fdlist))
            new_connection(listenfd);
      }
    }
      close (listenfd);
      //write_file(player_list);
      free_players(player_list);
    return 0;
}

/*
 * Sets up server socket: bind and listen
 * Returns a file descriptor for listening and accepting new connections
 */
int setup (void) {
	int on = 1, status;
	struct sockaddr_in self;
	int listenfd;
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	status = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                      (const char *) &on, sizeof(on));
	if(status == -1) {
		perror("setsockopt -- REUSEADDR");
	}

	memset(&self, '\0', sizeof(self));
	self.sin_family = AF_INET;
	self.sin_addr.s_addr = INADDR_ANY;
	self.sin_port = htons(port);
	printf("Listening on %d\n", port);

	if (bind(listenfd, (struct sockaddr *)&self, sizeof(self)) == -1) {
		perror("bind"); // probably means port is in use
		exit(1);
	}

	if (listen(listenfd, 5) == -1) {
		perror("listen");
		exit(1);
	}

	return listenfd;
}


/* timer signal handler */
void timer_handler(int sig) {
	//broadcast current game end to all connected clients
	Client *curr;
	for (curr = client_list; curr!=NULL; curr = curr->next) { // the first curr is top in the original version
            sendclient(curr, "Game over, type new_game to start the new board\r\n");
    }
	//TODO - generate new game board
  clean_board();
  board_generator();
  // char str [BUFFER_SIZE - 10];
  // sprintf(str, "---------\n");
  // sendclient(p, str);
  // for(int row = 0; row < 4; row++){
  //     for(int col = 0; col < 4; col++){
  //        sprintf(str, "|%c", board[row][col]);
  //        sendclient(p, str);
  //     }
  //     sprintf(str, "|\n");
  //     sendclient(p, str);
  //     sprintf(str, "---------\n");
  //     sendclient(p, str);
  // }
	//reset the timer so we get called again in 120 seconds
	alarm(TIMER_TICK);
}

/* accepts new connection and adds a client-specific fd */
void new_connection (int listenfd)  {
    int fd;
    struct sockaddr_in r;
    socklen_t socklen = sizeof(r);

    if ((fd = accept(listenfd, (struct sockaddr *)&r, &socklen)) < 0) {
        perror("accept");
        return;
    }

    printf("connection from %s\n", inet_ntoa(r.sin_addr));
    add_client(fd, r.sin_addr);
}

/* creates a new client struct and
TODO - adds it to the list of clients */
void add_client(int fd, struct in_addr addr){
    struct client *p = malloc(sizeof(struct client));
    if (!p) {
        perror("malloc failure");
        exit(1);
    }
    printf("Adding client %s\n", inet_ntoa(addr));
    fflush(stdout);
    p->fd = fd;
    p->state = NAME; //needs yet to identify new client by name
    p->inbuf = 0;
    //TODO - add it to the list of clients
    if(client_list == NULL){
      client_list = p;
      client_list -> next = NULL;
      p -> previous = NULL;
    }else{
      Client *curr = NULL;
      Client *temp = NULL;
      curr = client_list;
      while(curr != NULL){
        temp = curr;
        curr = curr -> next;
      }
      temp -> next = p;
      p -> previous = temp;
    }

	sendclient(p, "What is your player name?\n");
}


void receiveclient(struct client *p) {
    char *after = p->buf + p->inbuf;
    int room = BUFFER_SIZE - p->inbuf;
    int nbytes;
    if ((nbytes = read(p->fd, after, room)) > 0) {
        p->inbuf += nbytes;
        int where = find_network_newline(p->buf, p->inbuf);
        if (where >= 0) {
            p->buf[where] = '\0';
            p->buf[where+1] = '\0';
            printf("%s", p->buf);
            interpret_message(p);
            where+=2;  // skip over \r\n
            p->inbuf -= where;
            memmove(p->buf, p->buf + where, p->inbuf);
        }
        room = sizeof(p->buf) - p->inbuf;
        after = p->buf + p->inbuf;
    } else {
        remove_client(p->fd);
    }
}

/*
 * Acts according to the client state
 */
void interpret_message(Client *p) {
    char *buf;
    if (p->state == NAME) {
        strcpy(p->name, p->buf);
        switch (create_player(p->name, &player_list)) {
            case 1:
                sendclient(p, "Welcome back.\r\n");
                break;
            case 2:
                asprintf(&buf, "Username too long, truncated to %d chars.\r\n", (MAX_NAME-1));
                sendclient(p, buf);
                break;
            case 0:
                sendclient(p, "Welcome, new player\r\n");
                break;
        }
        printf("Added %s to the client list\n",p->name);
        p->state = COMMAND;
        sendclient(p, "Go ahead and enter player commands>\r\n");
    } else if (!strcmp(p->buf, "q")) {
        sendclient(p, "see you next time :)");
        remove_client(p->fd);
    } else {
        parse_line(p);
    }
}

int parse_line(Client *p) {
	char * input = p->buf;

	// cmd_argv will held arguments to individual commands passed to sub-procedure
    char *cmd_argv[INPUT_ARG_MAX_NUM];
    int cmd_argc;

    // tokenize arguments
    char *next_token = strtok(input, DELIM);
    cmd_argc = 0;
    while (next_token != NULL) {
        cmd_argv[cmd_argc] = next_token;
        cmd_argc++;
        next_token = strtok(NULL, DELIM);
    }

    return (cmd_argc > 0 && do_command(p, cmd_argc, cmd_argv));
}

/*
 * Process player commands
 * Return:  -1 for quit command
 *          0 otherwise
 */
int do_command(struct client * p, int cmd_argc, char **cmd_argv) {

    if (cmd_argc <= 0) {
        return 0;
    } else if (strcmp(cmd_argv[0], "q") == 0 && cmd_argc == 1) {
        write_file(player_list);
        return -1;
    } else if (strcmp(cmd_argv[0], "all_players") == 0 && cmd_argc == 1) {
		//TODO produce list of all players and their stats in nice text format and sendclient
      Player* curr = NULL;
      curr = player_list;
      while(curr != NULL){
        char str [BUFFER_SIZE - 10];
        //printf("--------------------\n");
        sprintf(str, "--------------------\n ");
        sendclient(p, str);
        //printf("%s:                 \n", curr -> name);
        sprintf(str, "%s:                 \n", curr -> name);
        sendclient(p, str);
        //printf("Total game is %d    \n", curr -> total_games);
        //printf("Total score is %d   \n", curr -> total_score);
        sprintf(str, "Total score is %d   \n", curr -> total_score);
        sendclient(p, str);
        //printf("Best score is %d    \n", curr -> max_score);
        sprintf(str, "Best score is %d    \n", curr -> max_score);
        sendclient(p, str);
        //printf("--------------------\n");
        sprintf(str, "--------------------\n");
        sendclient(p, str);
        curr = curr -> next;
      }
    } else if (strcmp(cmd_argv[0], "top_3") == 0 && cmd_argc == 1) {
		//TODO produce list of top 3 players with their total score and sentclient as a text
        Player* players = malloc(sizeof(Player)*player_num);
        Player * curr = NULL;
        curr = player_list;
        int i = 0 ;
        while(curr!=NULL){
          players[i] = *curr;
          i++;
          curr = curr -> next;
        }

        qsort(players, player_num, sizeof(Player), compare_players);
        char str [BUFFER_SIZE - 10];

        for(int i = 0 ; i < 3; i++){
          if(i < player_num){
            sprintf(str, "--------------------\n ");
            sendclient(p, str);
            sprintf(str, "%s:                 \n", players[i].name);
            sendclient(p, str);
            sprintf(str, "Total score is %d   \n", players[i].total_score);
            sendclient(p, str);
            sprintf(str, "--------------------\n");
            sendclient(p, str);
          }
        }
        free(players);
    } else if (strcmp(cmd_argv[0], "add_score") == 0 && cmd_argc == 2) {
        char str [BUFFER_SIZE - 10];
        int score =atoi(cmd_argv[1]);
        Player *player = find_player(p->name, player_list);

        player->total_score+=score;
        if (score > player->max_score)
            player->max_score = score;
        sprintf (str, "added score %d for player %s\r\n", score, p->name);
        sendclient(p, str);
    } else if (strcmp(cmd_argv[0], "new_game") == 0 && cmd_argc == 1) {
        //TODO -- transmit current board to be presented as a 2D array of chars
        board_generator();
        char str [BUFFER_SIZE - 10];
        sprintf(str, "---------\n");
        sendclient(p, str);
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
               sprintf(str, "|%c", board[row][col]);
               sendclient(p, str);
            }
            sprintf(str, "|\n");
            sendclient(p, str);
            sprintf(str, "---------\n");
            sendclient(p, str);
        }
    }else {
        sendclient(p, "Incorrect syntax\r\n");
    }
    return 0;
}










void sendclient(struct client *p, char *msg) {
    write(p->fd, msg, strlen(msg));
}


/*
 * Search the first inbuf characters of buf for a network newline.
 * Return the location of the '\r' if the network newline is found, or -1 otherwise.
 */
int find_network_newline(char *buf, int inbuf) {
	int i;
	for (i = 0; i < inbuf - 1; i++)
		if ((buf[i] == '\r') && (buf[i + 1] == '\n'))
			return i;
	return -1;
}

void remove_client(int clinetfd){
	Client *p;
	for (p = client_list; p; p = p->next){
		if(p->fd == clinetfd){
			Client *next = p->next;
			Client *prev = p->previous;
			if (prev != NULL){
				prev->next = next;
			} else {
				client_list = next;
			}
			if (next != NULL){
				next->previous = prev;
			}
			free(p);
			p=NULL;
			printf("Removed client fd %d\n", clinetfd);
			return;
		}
	}
}

int compare_players(const void*a, const void*b){
  Player *p1 = (Player*)a;
  Player *p2 = (Player*)b;
  return p2->total_score - p1->total_score;
}

void board_generator(){
	int test_all[17];
	for(int j = 0; j< 16; j++){
                test_all[j] = 0;
        }
	srand(time(0));

	char* all[16] = {"RIFOBX","IFEHEY","DENOWS","UTOKND","HMSRAO","LUPETS","ACITOA","YLGKUE","QBMJOA","EHISPN","VETIGN","BALIYT","EZAVND","RALESC","UWILRG","PACEMD"};
  int row, col;  // loop-control-variables for accessing rows and columns in A

  for ( row = 0; row < 4; row++ ) {
   for ( col = 0; col < 4; col++ ) {
	  	while(board[row][col] == 0){
      	int n = rand() % 16;
      	int a = rand() % 6;
				if(test_all[n] == 0){
          board[row][col] = all[n][a];
          test_all[n] = 1;
    		}
			}
		}
	}

	for(int j = 0; j< 16; j++){
		test_all[j] = 0;
	}

}

void clean_board(){
	for (int row = 0; row < 4; row++ ) {
		 for (int col = 0; col < 4; col++ )
					board[row][col] = 0;

	}
}

void print_board(){
	//printf("round # %d\n", roundnum);
	printf("---------\n");
	for(int row = 0; row < 4; row++){
			for(int col = 0; col < 4; col++){
				 printf("|%c", board[row][col]);
			}
			printf("|\n");
			printf("---------\n");
	}
}
