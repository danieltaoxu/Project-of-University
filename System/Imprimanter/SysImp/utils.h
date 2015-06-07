#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#define error1 "Error: Vous avez inseré une repertoire."
#define error2 "Error: La chemin n'est pas accessible." 

/*
* Méthode pour faire la concatenation entre 2 
* chaines de caractères
*/
void concatenation(char *debut, char *fin){
 	while(*debut)
		debut++;
	while(*fin){
		*debut = *fin;
		debut++;
		fin++;
	}
	*debut = '\0';
}

/*
*	Méthode qui retourne la chemin absolue
*/
char *absolue(char *reference){
	struct stat status;
	char *p, cwd[1024];
	int flag = 0;
	p = getcwd(cwd, sizeof(cwd));
	if(reference[0] != '/'){ /*chemin relatif*/
		/*concatenation(p, "/");*/
		/*concatenation(p, reference);*/
		strcat(p, "/");
		strcat(p, reference);
		if((stat(p, &status)) == 0){
			if(S_ISREG(status.st_mode))
				return p;
			else if(S_ISFIFO(status.st_mode))
				return p;
			else if(S_ISDIR(status.st_mode))
				flag = 1; /* error1*/
		}else
			flag = 2;	/* error2 */	
	}else{
		stat(reference, &status);
		if(S_ISREG(status.st_mode)){
			strcpy(p, reference);
			return p;
		}
		else if(S_ISDIR(status.st_mode))
			flag = 1;
	}
	if(flag == 1)
		return error1;/*"repertoire";*/
	else
		return error2;/*"pas accessible";*/
}
