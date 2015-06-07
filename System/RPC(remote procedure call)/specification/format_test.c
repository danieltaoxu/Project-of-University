#include <stdio.h> //printf
#include <stdlib.h> //malloc
#include "format.h"

int main(){
	//Création d'un argument de type int et d'un entier quelconque:
	int valeur = 1234;
	arg_t a = arg_from_int(valeur);
	//DBG
	printf("Entier de référence : %d\n", valeur);
	printf("Argument créé : Type : %#02x - Taille : %d - Entier : %d\n"
		, a.type, a.size, string_to_int(a.arg, a.size));//*/
	
	//Sérialisation de l'argument :
	char* buf = malloc(get_serialized_arg_size(a) * sizeof(char));
	serialize_arg(a, buf);
	//DBG
	printf("Sérialisé : %#02x %d ", buf[0], buf[1]);
	for(int i = 2; i < (2 + a.size) * sizeof(char); i++)
		printf("\'%c\'", buf[i]);
	printf("\n");//*/
	
	//Désérialisation de la chaîne
	arg_t a1 = unserialize_arg(buf);
	//DBG
	printf("Désérialisé : Type : %#02x - Taille : %d - Entier : %d\n"
		, a1.type, a1.size, string_to_int(a1.arg, a1.size));//*/
	
	//Création d'un argument de type string avec une chaîne quelconque
	char s[] = "Hello World !";
	arg_t a2 = arg_from_string(s, sizeof(s));
	//DBG
	printf("Chaîne de référence : %s\n", s);
	printf("Argument créé : Type : %#02x - Taille : %d - String : %s\n"
		, a2.type, a2.size, a2.arg);//*/
	
	//Sérialisation de l'argument
	char *buf1 = malloc(get_serialized_arg_size(a2) * sizeof(char));
	serialize_arg(a2, buf1);
	//DBG
	printf("Sérialisé : %#02x %d ", buf1[0], buf1[1]);
	for(int i = 2; i < (2 + a2.size) * sizeof(char); i++){
		printf("\'%c\'", buf1[i]);
	}
	printf("\n");//*/
	
	//Désérialisation de la chaîne
	arg_t a3 = unserialize_arg(buf1);
	//DBG
	printf("Désérialisé : Type : %#02x - Taille : %d - String : %s\n"
		, a3.type, a3.size, a3.arg);//*/
	return 0;
}
