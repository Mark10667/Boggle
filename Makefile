CC = gcc
CFLAGS = -Wall -std=gnu99
PORT=8889
CFLAGS+= -DPORT=\$(PORT)

# all: game_client.o players.o game_server.o user.o check_board.o dictionary.o check_dictionary.o signal.o game
all: server client

game_client.o: game_client.c game.h game_server.h
	$(CC) $(CFLAGS) -c game_client.c

user.o: user.c game.h
	$(CC) $(CFLAGS) -c user.c

check_board.o: check_board.c game.h game_server.h
	$(CC) $(CFLAGS) -c check_board.c

dictionary.o: dictionary.c game.h
	$(CC) $(CFLAGS) -c dictionary.c

check_dictionary.o: check_dictionary.c game.h
	$(CC) $(CFLAGS) -c check_dictionary.c

signal.o: signal.c game.h
	$(CC) $(CFLAGS) -c signal.c

players.o: players.c game.h game_server.h
	$(CC) $(CFLAGS) -c players.c

game_server.o: game_server.c game.h game_server.h
	$(CC) $(CFLAGS) -c game_server.c

server: players.o game_server.o user.o check_board.o dictionary.o check_dictionary.o signal.o
	$(CC) $(CFLAGS) -g -o server players.o game_server.o user.o check_board.o dictionary.o check_dictionary.o signal.o

client: players.o game_client.o user.o check_board.o dictionary.o check_dictionary.o signal.o
	$(CC) $(CFLAGS) -g -o client game_client.o players.o user.o check_board.o dictionary.o check_dictionary.o signal.o

clean:
	rm game_client.o players.o game_server.o user.o check_board.o dictionary.o check_dictionary.o signal.o


# CC = gcc
# CFLAGS = -Wall -std=gnu99 -Werror
# all: game_client.o players.o game_server.o game
#
# PORT=8889
# CFLAGS+= -DPORT=\$(PORT)
#
# game_client.o: game_client.c game.h
# 	$(CC) $(CFLAGS) -c game_client.c
#
# players.o: players.c game.h game_server.h
# 	$(CC) $(CFLAGS) -c players.c
#
# game_server.o: game_server.c game.h game_server.h
# 	$(CC) $(CFLAGS) -c game_server.c
#
# game: game_client.o players.o game_server.o
# 	$(CC) $(CFLAGS) -g -o game game_client.o players.o game_server.o
# clean:
# 	rm game_client.o players.o game_server.o game
