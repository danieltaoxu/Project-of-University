#ifndef H_CLIENT
#define H_CLIENT
#define TIMEOUT 5 //Temps d'attente de la réponse du serveur
#include "../specification/format.h"

/* Demande au serveur d'effectuer une opération
 * Patiente TIMEOUT secondes, puis retourne la réponse
 * -----
 * void* retour
 *     adresse où sera stocké le résultat de l'appel
 * const char* fonction
 *     nom de la fonction à appeler
 * unsigned short argc
 *     nombre d'arguments à passer
 * arg_t argv
 *     adresse vers le tableau contenant les arguments
 * -----
 * retour : terminaison - voir format.h
 */
static int appel_externe(arg_t* retour, const char* fonction, unsigned short argc,
	arg_t* argv);

/* Retourne la chaîne de caractères de la requête à envoyer au serveur
 * -----
 * const char* fonction
 *     fonction à appeler
 *     "addition", "soustraction", "multiplication" ou "division"
 * unsigneed short argc
 *     nombre d'arguments à passer
 * arg_t* argv
 *     adresse vers le tableau contenant les arguments
 * char* buf
 *     adresse vers le tableau stockant le résultat
 *     doit être alloué avec pour taille get_appel_externe_serialized_size()
 * int buffsize
 *     taille du buffer
 */
void serialize_appel_externe(const char* fonction, unsigned short argc, arg_t* argv, char* buf, int buffsize);

/* Retourne un argument en fonction de la réponse du serveur
 * -----
 * char* srv_ans
 *     réponse du serveur
 */
arg_t unserialize_server_answer(char* srv_ans);

/* Détermine la taille de la chaîne de l'appel externe
 * -----
 * unsigned short argc
 *     nombre d'arguments
 * arg_t* argv
 *     adresse vers le tableau contenant les arguments
 *
 */
static int get_appel_externe_serialized_size(unsigned short argc, arg_t* argv);
#endif //H_CLIENT
