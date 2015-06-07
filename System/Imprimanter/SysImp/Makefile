CC=gcc
CFLAGS= -c -Wall -g -std=c99 -pedantic -D _POSIX_C_SOURCE=200112L 
#INCLUDES = -I /home/taoxu/Projetsystem/SysImp
#SRCS = print_demon.c mpr.c sim_impr.c
#OBJS = $(SRCS: .c=.o)
#MAIN = project

#objects = print_demon.o mpr.o sim_impr.o

poject:	print_demon mpr sim_impr

#all: project

print_demon.o: print_demon.c utils.h
		$(CC) $(CFLAGS) print_demon.c

mpr.o: mpr.c utils.h
		$(CC) $(CFLAGS) mpr.c

sim_impr.o: sim_impr.c utils.h
		$(CC) $(CFLAGS) sim_impr.c

clean:
		rm -f *.o *~
cleanall:
		rm -f *.o *~ 
		rm -f print_demon mpr sim_impr
		rm -f TubeClient TubeImp1 TubeImp2 TubeImp3


