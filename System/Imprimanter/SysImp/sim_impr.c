#include "utils.h"

/*
 * Message d'usage an cas de mauvaise utilisation
 * d'utilisateur
 */
void usage(char *program){
	printf("Usage: %s [-flags] [options]\n", program);
	printf("Flags\t\tOptions\n......\t\t........ \n");
	printf("-t\t\tchemin absolue ou relatif \n\t\tde tube(TubeImp1, TubeImp2, TubeImp3)\n");
	printf("-n\t\tnom d'imprimante(imp1, imp2, imp3)\n");
}

/*
 * M A I N
 */
int main(int argc, char **argv){
	
	/*char *tamp = malloc(512 *sizeof(char));*/
	char *tamp = malloc(sizeof(char));
	/* read file descriptor */
	int readfd;
	/* Nom de tube */
	char *tubeImp;
	/* nom d'imprimante */
	char *nomImp;	

	if(argc != 5){
		usage(argv[0]);
		exit(1);
	}	

	if(strcmp(argv[1], "-t") == 0 && strcmp(argv[3], "-n") == 0){
		tubeImp = argv[2];
		/*printf("---%s\n", tubeImp); pas util cette message */
		nomImp = argv[4];
	}
	else if(strcmp(argv[3], "-t") == 0 && strcmp(argv[1], "-n") == 0){
		tubeImp  = argv[4];
		nomImp = argv[2];
	}else{
		usage(argv[0]);
		exit(1);
	}	
	
	if( (strcmp(tubeImp, "TubeImp1") == 0) && (strcmp(nomImp, "imp1") == 0) ){
		printf("Imprimante-1 est connecté...\n");
	}
	else if( (strcmp(tubeImp, "TubeImp2") == 0) && (strcmp(nomImp, "imp2") == 0) ){
		printf("Imprimante-2 est connecté...\n");
	}
	else if( (strcmp(tubeImp, "TubeImp3") == 0) && (strcmp(nomImp, "imp3") == 0) ){
		printf("Imprimante-3 est connecté...\n");
	}else{
		printf("Attention: L'imprimante est connecté avec le tube \n");
		printf("------------------------------------------------- \n");
		printf("Combinaisons possibles: \n[imp1 - TubeImp1] [imp2 - TubeImp2] [imp3 -TubeImp3]\n");
		printf("Vous avez inséré: \n------------------ \n[Nom Imprimante: %s] \n[Nom Tube: %s] \n", nomImp, tubeImp);
		exit(1);	
	}

	if((readfd = open(absolue(tubeImp), O_RDWR)) < 0)   /*O_RDONLY*/
		perror("open(): erreur en lecture");
	else{
		while(1){
			if((read(readfd, tamp, sizeof(char))) == -1) /*sizeof(tamp)*/
				perror("read() erreur");
			printf("%s", tamp);	
		}	
	}
	return 0;
}
