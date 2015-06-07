#include "utils.h"

/*
 * Message d'usage en cas de mauvaise utilisation
 * d'utilisateur
 */
void usage(char *program){
	printf("Usage: %s [-P] [Imprimante] [Reference]\n", program);
	printf("Imprimantes possibles: [imp1] [imp2] [imp3]\n");
}

/*
 * M A I N
 */
int main(int argc, char**argv){
	/*unsigned int *lng = 0;
	/uid_t uid;
	/gid_t gid;
	/char type = 'i';*/
	char *total = malloc(512 * sizeof(char));
	char *chemin;
	char *imprimante;
	int	writefd;
	int longueur;

	if(argc != 4){
		usage(argv[0]);
		exit(1);
	}

	if((strcmp(argv[1], "-P")) == 0){
		imprimante = argv[2];
		chemin = absolue(argv[3]);
	}
	else if((strcmp(argv[2], "-P")) == 0){
		imprimante = argv[3];
		chemin = absolue(argv[1]);
	}else{
		usage(argv[0]);
		exit(1);
	}

	if(strcmp(chemin, error1) == 0){
		printf("%s\n", error1);
		return -1;
	}else if(strcmp(chemin, error2) == 0){
		printf("%s\n", error2);
		return -1;
	}else{
		/*lng = strlen(imprimante) + strlen(chemin);*/
		total = imprimante;
		strcat(total, "\0");
		strcat(total, chemin);
		longueur = strlen(total);
		printf("%s\n", total);
		/*lng = sizeof(char) + sizeof(uid_t) + sizeof(gid_t) + strlen(argv[2]) + strlen(chemin);*/

		if((writefd = open("TubeClient", O_WRONLY)) < 0){
			perror("open(): erreur pour l'écriture");
			close(writefd);
		}else{
			printf("%d\n", longueur);
			printf("writefd: %d\n", writefd);
			if(write(writefd, total, sizeof(char) * longueur) == -1){
				perror("write() erreur");
				close(writefd);
			}
		}
		close(writefd);
	}
	return 0;
}







