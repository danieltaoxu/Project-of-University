#ifndef H_FORMAT
#define H_FORMAT

//Constantes relatives aux opérations reconnues par le client et le serveur
enum{
	FUNC_ADD = 0x00,
	FUNC_CONCAT = 0x00,
	FUNC_SUB = 0x01,
	FUNC_MULT = 0x02,
	FUNC_DIV = 0x03,
	FUNC_ACKERMAN = 0x04,
	FUNC_SPLIT = 0x05,
	FUNC_ECHO = 0x06,
	FUNC_DISCONNECT = 0x07,
	FUNC_INCONNU = 0xff
};

//Constantes relatives aux messages de terminaison de l'appel
enum{
	APPEL_OK = 0x00,
	FONCTION_INCONNUE = 0x01,
	MAUVAIS_ARGUMENTS = 0x02,
	PAS_DE_REPONSE = 0x03
};

//Constantes relatives aux types compatibles avec le client et le serveur
enum{
	TYPE_VOID = 0x00,
	TYPE_INT = 0x01,
	TYPE_STRING = 0x03
};

//Structure permettant de définir un argument (ou une valeur de retour)
typedef struct arg_t arg_t;
struct arg_t{
	int type;
	int size; //Si *arg pointe sur un tableau il est préférable de connaître sa taille
	void *arg;
};

/* Crée un argument à partir d'un entier
 * -----
 * int i
 *     entier à convertir
 */
arg_t arg_from_int(int i);

/* Crée un argument à partir d'une chaîne de caractères
 * -----
 * char* s
 *     chaîne à convertir
 * int size
 *     taille de la chaîne
 */
arg_t arg_from_string(char* s, int size);

/* Sérialise un argument tant que son type est supporté.
 * Stocke le résultat dans buf.
 * -----
 * arg_t a
 *     argument à sérialiser
 * char* buf
 *     adresse d'un tableau alloué de taille get_serialized_arg_size(arg_t a)
 */
void serialize_arg(arg_t a, char* buf);

/* Désérialise un argument tant que son type est supporté.
 * -----
 * char* as
 *     chaîne de caractères contenant l'argument sérialisé
 */
arg_t unserialize_arg(char* as);

/* Retourne le nombre de chiffres nécessaires pour écrire un nombre
 * -----
 * int i
 *     entier pour lequel on veut connaître le nombre de chiffres
 */
int get_number_digits(int i);

/* Convertit un entier en chaîne de caractères.
 * Stocke le résultat dans buf.
 * -----
 * int i
 *     entier à convertir
 * char* buf
 *     adresse d'un tableau alloué de taille get_number_digits(i)
 * -----
 * Cette fonction utilise snprintf.
 */
void int_to_string(int i, char* buf);

/* Convertit une chaîne de caractères en entier
 * -----
 * char* s
 *     chaîne contenant la représentation du nombre
 * int size
 *     taille de la chaîne
 * -----
 * C'est l'inverse de int_to_string
 */
int string_to_int(char* s, int size);

/* Donne la taille nécessaire pour stocker l'argument une fois sérialisé.
 * -----
 * arg_t a
 *     argument dont on veut prédire la taille une fois sérialisé
 * retour
 *     taille de l'élément sérialisé, en nombre de char (octets)
 *
 */
int get_serialized_arg_size(arg_t a);

#endif //H_FORMAT
