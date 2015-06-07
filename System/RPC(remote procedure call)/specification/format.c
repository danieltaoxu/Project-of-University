#include <stdio.h>
#include <stdlib.h>
#include "format.h"

arg_t arg_from_int(int i){
	arg_t ret;
	ret.type = TYPE_INT;
	ret.size = get_number_digits(i);
	ret.arg = malloc(ret.size * sizeof(char));
	int_to_string(i, ret.arg);
	return ret;
}

arg_t arg_from_string(char* s, int size){
	arg_t ret;
	ret.type = TYPE_STRING;
	ret.size = size;
	ret.arg = malloc(ret.size * sizeof(char));
	for(int i = 0; i < ret.size; i++){
		((char*)ret.arg)[i] = s[i];
	}
	return ret;
}

void serialize_arg(arg_t a, char* buf){
	if(a.type == TYPE_VOID) buf[0] = a.type;
	else if(a.type == TYPE_INT || a.type == TYPE_STRING){
		buf[0] = a.type;
		buf[1] = a.size;
		for(int i = 2; i < a.size + 2; i++){
			buf[i] = ((char*) a.arg)[i - 2];
		}
	}
}

arg_t unserialize_arg(char* as){
	arg_t ret;
	ret.type = as[0];
	if(ret.type == TYPE_VOID) return ret;
	ret.size = as[1];
	ret.arg = malloc(ret.size * sizeof(char));
	for(int i = 0; i < ret.size; i++){
		((char*)ret.arg)[i] = as[i + 2];
	}
	return ret;
}

int get_number_digits(int i){
	int j = i;
	char nb_caracteres = 0;
	while(j != 0){
		j /= 10;
		nb_caracteres++;
	}
	return nb_caracteres;
}

void int_to_string(int i, char* buf){
	int size = get_number_digits(i);
	char* buf1 = malloc(sizeof((size + 1) * sizeof (char)));
	snprintf(buf1, size + 1, "%d", i);
	for(int i = 0; i < size; i++) buf[i] = buf1[i];
}

int string_to_int(char* s, int size){
	int ret = 0;
	for(int i = size; i > 0; i--){
		int puissance_dix = 1;
		for(int j = 1; j < i; j++) puissance_dix *= 10;
		ret += (s[size-i] - '0') * puissance_dix;
	}
	return ret;
}

int get_serialized_arg_size(arg_t a){
	return a.size + 2;
}
