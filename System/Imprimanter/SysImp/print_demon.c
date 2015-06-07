#define _BSD_SOURCE
#include <wait.h>
#include "utils.h"

/*
 * Fonction qui connete le serveur et l'imprimante
 */

void connection(char *trImp, char *chemin){
	/*char *buf = malloc(512 * sizeof(char));*/
	char *buf = malloc(sizeof(char));
	int writefd;
	int filefd;
	char *dummy = malloc(512 * sizeof(char));
	static int compteur = 1; /* Numèro total d'impressions */

	if((writefd = open(trImp, O_WRONLY | O_NONBLOCK)) < 0)
		perror("open(): Erreur en ecriture");

	if((filefd = open(chemin, O_RDONLY)) < 0){
		perror("open(): Erreur en 'open file'");
		close(filefd);
	}else{

		/* Envoyer le chemin absolue à l'imprimante avec le numèro d'impression */
		memset(buf, '\0', strlen(buf));
		sprintf(dummy, "%d", compteur);
		strcpy(buf, "\n");
		strcat(buf, dummy);
		strcat(buf, ") Document qui va être imprimé: ");
		strcat(chemin, "\n");
		strcat(buf, chemin);
		strcat(buf, "--------------------------------\n");
		compteur++;

		if(write(writefd, buf, strlen(buf) * sizeof(char)) == -1)
			perror("write(): erreur en écriture du chemin");

		while(read(filefd, buf, sizeof(char)) != 0){
			/* il y a la possibilite que le serveur arrete. c'est pourquoi on utilise sleep; */
			usleep(1);
			/* on utilise break pour voir seulement une message d'erreur et no beaucoup de lignes */
			if(write(writefd, buf, sizeof(char)) == -1){ /*sizeof(buf)*/
				perror("write(): erreur");
				compteur--;
				break;
			}
			/*printf("%s", buf); //juste pour le test*/
		}
		strcpy(buf, "\n");
		write(writefd, buf, sizeof(char));
		/*printf("\n"); juste pour le test*/
	}
}

/*
 * Message d'affichage en cas de mauvaise utilisation
 * d'utilisateur
 */
void usage(char *program){
	printf("Usage: %s [-flags] [options]\n", program);
	printf("Flags\t\tOptions \n------\t\t-------- \n");
	printf("-c\t\tfichier_configuration\n");
	printf("-t\t\tnom de tube(TubeClient)\n");
}

/*
 * M A I N
 */
int main(int argc, char **argv){

	char *tr;
	char *tamp = malloc(1024 * sizeof(char));
	char tr1[]="TubeImp1", tr2[]="TubeImp2", tr3[]="TubeImp3";
	char *imprimante = malloc(30 * sizeof(char));
	char *chemin = malloc(100 * sizeof(char));
	int readClifd;
	int i = 0;
	int j;
	int pid;

	if(argc != 5){
		usage(argv[0]);
		exit(1);
	}

	if((strcmp(argv[1], "-t") == 0) && (strcmp(argv[3], "-c") == 0)){
		tr = argv[2];
	}else if((strcmp(argv[3], "-t") == 0) && (strcmp(argv[1], "-c") == 0)){
		tr = argv[4];
	}else{
		usage(argv[0]);
		exit(1);
	}

	mkfifo(tr, S_IRWXU);	/*Clients - Serveur*/
	mkfifo(tr1, S_IRWXU); /*Serveur - Imprimante1*/
	mkfifo(tr2, S_IRWXU); /*Serveur - Imprimante2*/
	mkfifo(tr3, S_IRWXU); /*Serveur - Imprimante3*/

	printf("Server is ON...\n");
	if((readClifd = open(tr, O_RDWR)) < 0){
		perror("open(): erreur en lecture");
		exit(1);
	}

	while(1){
		memset(tamp, '\0', strlen(tamp)); /* put '\0' to all positions of tamp */
		memset(imprimante, '\0', strlen(imprimante));

		if((read(readClifd, tamp, 512 * sizeof(char))) == -1) /*512*sizeof(tamp)*/
			perror("read() Erreur");
		else{
			/* Procesus enfant */
			if((pid=fork()) == 0){
				printf("------------------------------------\n");
				printf("Total: %s\n", tamp);

				/* insérer le nom d'imprimante */
				/* int i = 0; pedantic problémes ici */
				while(tamp[i] != '/'){
					i++;
				}
				strncpy(imprimante, tamp, i);

				/* insérer la chemin */
				/* int j; pedantic problémes ici */
				for(j=0; j<i; j++)
					tamp++;
				strcpy(chemin, tamp);

				printf("Imprimante: %s\n", imprimante);
				printf("Chemin: %s\n", chemin);

				/* IMPRIMANTE 1 */
				if(strcmp(imprimante, "imp1") == 0){
					connection(tr1, chemin);
				}
				/* IMPRIMANTE 2 */
				else if(strcmp(imprimante, "imp2") == 0){
					connection(tr2, chemin);
				}
				/* IMPRIMANTE 3 */
				else if(strcmp(imprimante, "imp3") == 0){
					connection(tr3, chemin);
				}
				else{
					printf("L'imprimante '%s' n'existe pas.\n", imprimante);
				}
			/* Processus pére, faire rien, n'attends pas l'enfant */
			}else{

			}
		}
	}

	return 0;
}
