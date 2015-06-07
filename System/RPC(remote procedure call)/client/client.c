/*
 * TODO : Le client se connecte au serveur, sérialise sa requête puis attend pendant 5 secondes la réponse de ce dernier.
 * À la réception de la requète, la connexion au serveur est fermée, la réponse est désérialisée, puis affichée.
 */
 
#include <stdio.h> //printf
#include <stdlib.h> //malloc
#include <unistd.h> //perror, exit
#include <errno.h> //perror
#include <sys/un.h> //AF_UNIX
#include <sys/socket.h> //socket()...
#include <string.h> //strcmp
#include <time.h> //difftime() //TODO surement pas besoin
#include <sys/types.h>
#include "../specification/format.h"
#include "client.h"
#define SRV_SOCK_PATH "/tmp/.soquette"
#define BUFFSIZE 56
#define TIMEOUT 5

int appel_externe(arg_t* retour, const char* fonction, unsigned short argc, arg_t* argv){
	//Initialisation du buffer
	int buffsize = get_appel_externe_serialized_size(argc, argv);
	char* buf_send = malloc(buffsize * sizeof(char));
	
	//Remplissage du buffer
	serialize_appel_externe(fonction, argc, argv, buf_send, buffsize);
	
	//Initialisation du socket
	struct sockaddr_un addr;
	int srv_fd;
	if((srv_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("Erreur de socket");
		exit(EXIT_FAILURE);
	}
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SRV_SOCK_PATH, sizeof(addr.sun_path) - 1);
	
	//Connexion
	if(connect(srv_fd,(struct sockaddr*) &addr,
		sizeof(struct sockaddr_un )) < 0){
		perror("Erreur de connexion");
		exit(EXIT_FAILURE);
	}
	
	//Écriture
	if(write(srv_fd, buf_send, buffsize) != buffsize){
		perror("Erreur d'écriture");
		exit(EXIT_FAILURE);
	}
	
	//Initialisation des variables utilisées pour la réception
	char buf_receive[BUFFSIZE];
	int i = 0;
	struct timeval timeout;      
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
	
	//Si le serveur ne répond pas dans un délai imparti, on arrête la réception
	if (setsockopt (srv_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        perror("setsockopt failed\n");
	i = read(srv_fd, buf_receive, BUFFSIZE);

    	/*DBG : afficher la chaîne reçue du serveur
    	for (int j = 0; j < i; j++) {
        	printf("%#02x ", buf_receive[j]);
    	}
    	printf("\n");
    	printf("i : %d\n", i);*/
    	
	//Retour en fonction de la réponse obtenue
	if(i == 0 || i == -1){ //Timeout
		memset(buf_send, 0, BUFFSIZE);
		buf_send[0] = FUNC_DISCONNECT;
		if(write(srv_fd, buf_send, buffsize) != buffsize){
			perror("Erreur d'écriture");
			exit(EXIT_FAILURE);
	}
		//close(srv_fd);
		return PAS_DE_REPONSE;
	}
	if(buf_receive[0] != APPEL_OK) return buf_receive[0];
	else{
		struct arg_t ret = unserialize_server_answer(buf_receive);
		retour->type = ret.type;
		retour->size = ret.size;
		retour->arg = ret.arg;
		/*DBG : afficher l'argument retourné
		printf("dbg : ret.type : %#02x\n", retour.type);
		printf("dbg : ret.size : %d\n", retour.size);*/
		return buf_receive[0];
	}
}

void serialize_appel_externe(const char* fonction, unsigned short argc, arg_t* argv, char* buf, int buffsize){
	//Définition de la fonction à appeler et du nombre d'arguments
	if(strcmp(fonction, "addition") == 0){
		buf[0] = FUNC_ADD;
	}
	else if(strcmp(fonction, "soustraction") == 0){
		buf[0] = FUNC_SUB;
	}
	else if(strcmp(fonction, "multiplication") == 0){
		buf[0] = FUNC_MULT;
	}
	else if(strcmp(fonction, "division") == 0){
		buf[0] = FUNC_DIV;
	}
	else if(strcmp(fonction, "ackerman") == 0){
		buf[0] = FUNC_ACKERMAN;
	}
	else{
		buf[0] = FUNC_INCONNU;
	}
	buf[1] = argc;
	
	//Ajout des arguments
	int i = 2;
	while(i < buffsize){
		for(int j = 0; j < argc; j++){
			int buff1size = get_serialized_arg_size(argv[j]);
			char* buf1 = malloc(buff1size * sizeof (char));
			serialize_arg(argv[j], buf1);
			for(int k = 0; k < buff1size; k++){
				buf[i] = buf1[k];
				i++;
			}
		}
	}
	/*DBG : permet d'afficher l'argument sérialisé
	for(int i = 0; i < buffsize; i++){
		printf("%#02x ", buf[i]);
	}
	printf("\n");*/
}

arg_t unserialize_server_answer(char* srv_ans){
	int size = srv_ans[2] + 2;
	arg_t ret;
	char* buf = malloc(size * sizeof(char));
	for(int i = 0; i < size; i++){
		buf[i] = srv_ans[i+1];
		//DBG printf("%#02x ", buf[i]);
	}
	ret = unserialize_arg(buf);
	/*printf("\n");
	printf("unserialize_server_answer : ret.type : %#02x\n", ret.type);
	printf("unserialize_server_answer : ret.size : %d\n", ret.size);*/
	return ret;
}

int get_appel_externe_serialized_size(unsigned short argc, arg_t* argv){
	//Calcul de la taille du buffer et allocation
	//Taille mini : 2 - "opcode" fonction et argc
	int buffsize = 2;
	for(int i = 0; i < argc; i++){
		buffsize += get_serialized_arg_size(argv[i]);
	}
	return buffsize;
}

int main(int argc, char* argv[]){
	if(argc == 1){
		printf("Erreur d'invocation. Veuillez consulter l'aide.\n");
		exit(EXIT_SUCCESS);
	}
	if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
		printf("\nClient écrit dans le cadre du projet de Programmation Système de 2015, à l'Université Paris 7. Le client permet d'envoyer des instructions et des paramètres à un serveur, qui les exécute et renvoie le résultat.\n");
		printf("\nUtilisation : client [options] nom_fonction {type_parametre, nom_parametre}...\n");
		printf("nom_fonction\n");
		printf("\taddition\t(int | string)\n");
		printf("\tsoustraction\t(int)\n");
		printf("\tmultiplication\t(int)\n");
		printf("\tdivision\t(int)\n");
		printf("type_parametre\n");
		printf("\tint\tentier\n");
		printf("\tstring\tchaîne de caractères\n");
		printf("\nOptions :\n");
		printf("\t--help, -h\tAffiche l'aide\n");
		//printf("\t--test, -t\tEffectue une opération de test\n");
		exit(EXIT_SUCCESS);
	}
	//TODO: supprimer le bloc if suivant et supprimer --test comme option.
	/*if(strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--test") == 0){
		printf("Procédure de test - Connexion au socket /tmp/.soquette\n");
		printf("Appel externe : addition 1234 2345\n");
		arg_t args[2];
		args[0] = arg_from_int(1234);
		args[1] = arg_from_int(2345);
		arg_t retour;
		int ret = appel_externe(&retour, "addition", 2, args);
		printf("L'appel externe a retourné %#02x.\n", ret);
		//printf("retour.size : %d\n", retour.size);
		if(ret == APPEL_OK){
			printf("Résultat : %d\n", string_to_int(retour.arg, retour.size));
			for(int i = 0; i < retour.size; i++){
				printf("%#02x", ((char*)retour.arg)[i]);
				if(i == retour.size - 1) printf("\n");
				else printf(" ");
			}
		}
		exit(EXIT_SUCCESS);
	}*/
	
	//Initialise et remplit la liste des arguments
	int nb_args = (argc - 2) / 2;
	arg_t* args = malloc(nb_args * sizeof(arg_t));
	int i = 0;
	for(int j = 2; j < argc; j+= 2){
		if(strcmp(argv[j], "string") == 0){
			args[i] = arg_from_string(argv[j+1], strlen(argv[j+1]));
		}
		else if(strcmp(argv[j], "int") == 0){
			args[i] = arg_from_int(string_to_int(argv[j+1], strlen(argv[j+1])));
		}
		i++;
	}
	arg_t retour;
	
	//Appel externe
	int ret = appel_externe(&retour, argv[1], nb_args, args);
	
	//Affichage du résultat
	if(ret == APPEL_OK){
		printf("L'appel s'est déroulé correctement !\n");
		printf("Résultat : ");
		if(retour.type == TYPE_VOID) printf("void\n");
		else if(retour.type == TYPE_INT) printf("int %d\n", string_to_int(retour.arg, retour.size));
		else if(retour.type == TYPE_STRING) printf("string %s\n", (char*)retour.arg);
	}
	else if(ret == FONCTION_INCONNUE) printf("Le serveur ne connait pas cette fonction.\n");
	else if(ret == MAUVAIS_ARGUMENTS) printf("La fonction a été appelée avec de mauvais arguments.\n");
	else if(ret == PAS_DE_REPONSE) printf("Pas de réponse du serveur dans le délai imparti de %d secondes\n", TIMEOUT);
	exit(EXIT_SUCCESS);
}
