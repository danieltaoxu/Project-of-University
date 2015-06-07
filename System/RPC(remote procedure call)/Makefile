CC = gcc
CFLAGS = -Wall -std=gnu99
#gnu99 pour la déclaration des compteurs dans une boucle for

FORMATDIR = ./specification
CLIENTDIR = ./client
SERVERDIR = ./serveur
SERVERLIBS = -lpthread
OUTPUTDIR = ./bin

all: client server

format: 
	$(CC) -c $(CFLAGS) $(FORMATDIR)/format.c
	
formattest: prebuild format
	$(CC) $(CFLAGS) format.o $(FORMATDIR)/format_test.c -o $(OUTPUTDIR)/format_test

client: prebuild format
	$(CC) $(CFLAGS) format.o $(CLIENTDIR)/client.c -o $(OUTPUTDIR)/client

server: prebuild format
	$(CC) $(CFLAGS) format.o $(SERVERDIR)/serveur.c -o $(OUTPUTDIR)/serveur $(SERVERLIBS)

prebuild:
	mkdir -p $(OUTPUTDIR)

clean:
	rm -f *.o
