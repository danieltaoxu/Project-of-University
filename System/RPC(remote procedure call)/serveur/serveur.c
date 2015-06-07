//
//  calculService.c
//  
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "serveur.h"
#include "../specification/format.h"
#include <pthread.h>
#include <signal.h>
/*
 * Nommer le socket .soquette dans le répertoire /tmp
 */
#define SERVER_SOCKPATH "/tmp/.soquette"
/*
 * Définir le max nombre de thread
 */
#define MAX_NB_THREAD 100
/*
 * Définir le buffer size
 */
#define BUFFER_SIZE 56

int counter_thread = 0;
pthread_t calcul_thread[MAX_NB_THREAD];
pthread_mutex_t mtx;
/*typedef struct Arg Arg;
struct Arg{
    int client;
    char buf[BUFFER_SIZE];
    int bufsize;
};*/

char bufferServeur[BUFFER_SIZE];
int byteRead = 0;

static int ackermann(int m, int n){
    if(m == 0) {
        return n + 1;
    } else if (m > 0 && n == 0) {
        return ackermann(m - 1, n);
    } else if (m > 0 && n > 0) {
        return ackermann(m - 1,ackermann(m, n - 1));
    }
    return 0;
}

void *attendreDisconnect(void *client) {
    int client_conn = (int)client;
    int nb_arg = 0;
    char *func = malloc(4 * sizeof(char));
    
    nb_arg = bufferServeur[1];
    arg_t *argv = malloc(nb_arg * sizeof(arg_t));
    if (bufferServeur[0] == FUNC_ADD) {
        strcpy(func, "add");
    } else if (bufferServeur[0] == FUNC_SUB) {
        strcpy(func, "sub");
    } else if (bufferServeur[0] == FUNC_MULT) {
        strcpy(func, "mul");
    } else if (bufferServeur[0] == FUNC_DIV) {
        strcpy(func, "div");
    } else if (bufferServeur[0] == FUNC_ACKERMAN) {
        strcpy(func, "ack");
    } else {
        strcpy(func, "inc");
    }
    
    argv = unserialize_appel_externe(bufferServeur, byteRead);
    memset(bufferServeur, 0, sizeof(bufferServeur));
    
    execution_calcul(func, argv, nb_arg, bufferServeur);
    if (write(client_conn, bufferServeur, sizeof(bufferServeur)) < 0) {
        perror("Send resultat failed!\n");
        exit(EXIT_FAILURE);
    }
    close(client_conn);
    return NULL;
}

void *reponse_externe(void *client_sk) {
    int client_conn = (int)client_sk;
    char buffer[BUFFER_SIZE];
    int byte;
    //int nb_arg = 0;
    //char *func = malloc(4 * sizeof(char));
    
    //memset(buffer, 0, sizeof(buffer));
    if ((byte = read(client_conn, bufferServeur, BUFFER_SIZE)) > 0) {
        pthread_t thread;
        byteRead = byte;
        int pthread_err = pthread_create(&thread, NULL, attendreDisconnect, (void *)client_conn);
        if (pthread_err != 0) {
            perror("Create thread failed\n");
            exit(EXIT_FAILURE);
        }
        /*nb_arg = buffer[1];
        arg_t *argv = malloc(nb_arg * sizeof(arg_t));
        if (buffer[0] == FUNC_ADD) {
            strcpy(func, "add");
        } else if (buffer[0] == FUNC_SUB) {
            strcpy(func, "sub");
        } else if (buffer[0] == FUNC_MULT) {
            strcpy(func, "mul");
        } else if (buffer[0] == FUNC_DIV) {
            strcpy(func, "div");
        } else if (buffer[0] == FUNC_ACKERMAN) {
            strcpy(func, "ack");
        } else {
            strcpy(func, "inc");
        }*/
        
        
        /*for (int j = 0; j < byte; j++) {
            printf("%#02x ", buffer[j]);
            fflush(stdout);
        }*/
        
        if ((byte = read(client_conn, buffer, BUFFER_SIZE)) > 0) {
            if (buffer[0] == FUNC_DISCONNECT) {
                printf("Client have terminated\n");
                pthread_cancel(thread);
                close(client_conn);
            }
        }
       // pthread_join(thread, NULL);
        
        /*argv = unserialize_appel_externe(buffer, byte);
        memset(buffer, 0, sizeof(buffer));*/
        //int len = execution_calcul(func, argv, nb_arg, buffer);
        /*execution_calcul(func, argv, nb_arg, buffer);
        if (write(client_conn, buffer, sizeof(buffer)) < 0) {
            perror("Send resultat failed!\n");
            exit(EXIT_FAILURE);
        }*/
        //test c'est bien afficher
        /*int i = 0;
        for (i = 0; i < len; i++) {
            printf("%#02x ", buffer[i]);
            fflush(stdout);
        }
        printf("\n");*/
    }
    
    return NULL;
}

static int execution_calcul(const char *func, const arg_t *argv, const int nb_arg, char *buf) {
    int i = 0;
    int typeTmp = argv[i].type;
    i++;
    int k = 0;
    
    while (i < nb_arg) {
        if (typeTmp != argv[i].type) {
            buf[0] = MAUVAIS_ARGUMENTS;
            return k++;
        }
        i++;
    }
    
    if(strcmp(func, "inc") == 0) {
        buf[k] = FONCTION_INCONNUE;
        return k++;
    }
    
    arg_t argRetour;
    //Pour savoir quel type de calcul
    if (typeTmp == TYPE_INT) {
        int value[nb_arg];
        for (int m = 0; m < nb_arg; m++) {
            value[m] = string_to_int(argv[m].arg, argv[m].size);
        }

        int sum = 0;
        if (strcmp(func, "add") == 0) {
            for (int m = 0; m < nb_arg; m++) {
                sum += value[m];
            }
            buf[k] = APPEL_OK;
        } else if (strcmp(func, "sub") == 0) {
            int m = 0;
            sum = value[m];
            for (m = 1; m < nb_arg; m++) {
                sum -= value[m];
            }
            buf[k] = APPEL_OK;
        } else if (strcmp(func, "mul") == 0) {
            int m = 0;
            sum = value[m];
            for (m = 1; m < nb_arg; m++) {
                sum *= value[m];
            }
            buf[k] = APPEL_OK;
        } else if (strcmp(func, "div") == 0) {
            int m = 0;
            sum = value[m];
            for (m = 1; m < nb_arg; m++) {
                sum /= value[m];
            }
            buf[k] = APPEL_OK;
        } else if (strcmp(func, "ack") == 0) {
            sum = ackermann(value[0], value[1]);
            buf[k] = APPEL_OK;
        }
        
        //printf("%#02x ", buf[k]);
        //Serializer le resultat argument(entier)
        if (buf[k] == APPEL_OK) {
            k++;
            argRetour = arg_from_int(sum);
            int argSize = get_serialized_arg_size(argRetour);
            char *bufferArg = malloc(argSize * sizeof(char));
            serialize_arg(argRetour, bufferArg);
            int n = 0;
            while (n < argSize) {
                buf[k] = bufferArg[n];
                k++;
                n++;
                //printf("%#02x ", buf[k]);
            }
        }
    } else if (typeTmp == TYPE_STRING) {
        int lenStr = 0;
        for (int m = 0; m < nb_arg; m++) {
            lenStr += argv[m].size;
        }
        //printf("%d\n", lenStr);
        char *str = malloc(lenStr * sizeof(char));
        k = 0;
        if (strcmp(func, "add") == 0) {
            for (int m = 0; m < nb_arg; m++) {
                strcat(str, argv[m].arg);
            }
            buf[k] = APPEL_OK;
            //Serializer le resultat argument(entier)
            k++;
            argRetour = arg_from_string(str, strlen(str));
            int argSize = get_serialized_arg_size(argRetour);
            char *bufferArg = malloc(argSize * sizeof(char));
            serialize_arg(argRetour, bufferArg);
            int n = 0;
            while (n < argSize) {
                buf[k] = bufferArg[n];
                k++;
                n++;
            }
        }
    } /*else {
        buf[k] = APPEL_OK;
        k++;
        buf[k] = TYPE_VOID;
        k++;
    }*/
    return k;
}

static arg_t *unserialize_appel_externe(char *buf, const int buffsize) {
    int argc = buf[1];
    arg_t *argv = malloc(argc * sizeof(arg_t));
    int num = 0;
    int i = 2;
    int arg_size = buf[i + 1];
    int arg_bufsize;
    int debut = 2;
    int fin;
    
    //Lire la chaîne et désérializer les arguments
    while (num < argc) {
        arg_bufsize = 2 + arg_size;
        char *arg_buf = malloc(arg_bufsize * sizeof(char));
        fin = debut + arg_bufsize;
        int j = 0;
        for (i = debut; i < fin; i++) {
            arg_buf[j] = buf[i];
            j++;
        }
        argv[num] = unserialize_arg(arg_buf);
        num++;
        arg_size = buf[fin + 1];
        debut = fin;
    }
    return argv;
}

static void sig_rm_soquette(int signo) {
    if (signo == SIGINT) {
        if(unlink(SERVER_SOCKPATH) < 0) {
            perror("Remove socket failed!\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
}

static void connect_client() {
    int server_sk, client_sk;
    struct sockaddr_un addr;
    //char *mesNegative = "Serveur est occupe, essyez a tout a l'heure\n";
    /*
     * Céer un serveur socket
     */
    if ((server_sk = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("Create server socket failed!\n");
        exit(EXIT_FAILURE);
    }
    
    memset(&addr, 0, sizeof(struct sockaddr_un));
    /*
     * Mettre le type de socket
     */
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SERVER_SOCKPATH, sizeof(addr.sun_path) - 1);
    
    /*
     * Attacher l'adresse de serveur socket
     */
    if (bind(server_sk, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0) {
        perror("Bind failed!\n");
        exit(EXIT_FAILURE);
    }
    
    /*
     * Commencer à écouter le socket
     * Nombre max client
     */
    if (listen(server_sk, MAX_NB_THREAD)) {
        perror("Listen failed!\n");
        exit(EXIT_FAILURE);
    }
    
    /*
     * Attendre le client qui veut connecter
     */
    while (1) {
        if (signal(SIGINT, sig_rm_soquette) == SIG_ERR) {
            perror("Can't catch SIGINT!\n");
            exit(EXIT_FAILURE);
        }
        
        if ((client_sk = accept(server_sk, NULL, NULL)) < 0) {
            perror("Accept failde!\n");
            exit(EXIT_FAILURE);
        }
        /*
         * Créer un thread pour un client etle serveur ne bloque pas d'attendre qu'un client
         */
        //if (counter_thread < MAX_NB_THREAD) {
            int pthread_err = pthread_create(&calcul_thread[counter_thread], NULL,reponse_externe, (void *)client_sk);
            if (pthread_err != 0) {
                perror("Create thread failed\n");
                exit(EXIT_FAILURE);
            }
            //counter_thread++;
        //printf("%d\n", counter_thread);
        /*} else {
            if (write(client_sk, mesNegative, sizeof(mesNegative)) < 0) {
                perror("Send message negative failed!\n");
                exit(EXIT_FAILURE);
            }
        }*/
        /*int m = 0;
        for (m = counter_thread; m >= 0; m--) {
            printf("%d\n", counter_thread);
            pthread_join(calcul_thread[m], NULL);
            counter_thread--;
        }*/
        
    }
}

int main() {
    printf("Serveur - Créer le socket /tmp/.soquette\n");
    connect_client();
    exit(EXIT_SUCCESS);
}
