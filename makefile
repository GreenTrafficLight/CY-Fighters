OBJS	= battle.o computer.o fighters_database.o fighter.o file_manager.o game.o main.o menu.o player.o skill.o team_interface.o team.o
SOURCE	= battle.c computer.c fighters_database.c fighter.c file_manager.c game.c main.c menu.c player.c skill.c team_interface.c team.c
HEADER	= battle.h computer.h fighters_database.h fighter.h file_manager.h game.h menu.h player.h skill.h team_interface.h team.h
OUT	= main
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lncursesw

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

battle.o: battle.c
	$(CC) $(FLAGS) battle.c 

computer.o: computer.c
	$(CC) $(FLAGS) computer.c 

fighters_database.o: fighters_database.c
	$(CC) $(FLAGS) fighters_database.c 

fighter.o: fighter.c
	$(CC) $(FLAGS) fighter.c 

file_manager.o: file_manager.c
	$(CC) $(FLAGS) file_manager.c 

game.o: game.c
	$(CC) $(FLAGS) game.c 

main.o: main.c
	$(CC) $(FLAGS) main.c 

menu.o: menu.c
	$(CC) $(FLAGS) menu.c 

player.o: player.c
	$(CC) $(FLAGS) player.c 

skill.o: skill.c
	$(CC) $(FLAGS) skill.c 

team_interface.o: team_interface.c
	$(CC) $(FLAGS) team_interface.c 

team.o: team.c
	$(CC) $(FLAGS) team.c 


clean:
	rm -f $(OBJS) $(OUT)