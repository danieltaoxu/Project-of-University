//
//  serveur.h
//  
//
//

#ifndef ____serveur__
#define ____serveur__

#include "../specification/format.h"
/* Retourner le resultat pour les clients
 * -----------
 * void *client_sk
 *      socket client
 */
void *reponse_externe(void *client_sk);

/* Exécution des calcul pour les clients
 * -----------
 * const char *func
 *      nom d'opération
 * const arg_t *argv
 *      les arguments
 * const int nb_arg
 *      nombre d'argument
 * char *buf
 *      resultat désérializé stockant
 * @return la longuer de chaîne
 */
static int execution_calcul(const char *func, const arg_t *argv, const int nb_arg, char *buf);

/* Désérialiser la chaîne de caractère de la requête et faire calcul
 * ------------
 * char *buf
 *      adresse vers le requête lu
 * const int buffsize
 *      taille du buffer
 * @return la structure d'argument
 */
static arg_t *unserialize_appel_externe(char *buf, const int buffsize);

/* Attendre les clients
 *
 */
static void connect_client();

/* Accepter signal
 * -------------
 * int signo
 *     signal
 */
static void sig_rm_soquette(int signo);

#endif /* defined(____serveur__) */
