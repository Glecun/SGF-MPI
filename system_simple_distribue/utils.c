#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "utils.h"

int execRegex (char* str_regex,char* str_request){
	int err, match;
	regex_t preg;
	err = regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED);
	if (err != 0){ perror("compilaion regexp failed"); }
	match = regexec (&preg, str_request, 0, NULL, 0);
	regfree (&preg);
	return match;
}

char *inputString(FILE* fp, size_t size){
	/*
		Alloue dynamiquement la taille de la chaine de charactere.
	*/
	// source : http://stackoverflow.com/questions/16870485/how-can-i-read-an-input-string-of-unknown-length
	//The size is extended by the input with the value of the provisional
	char *str;
	int ch;
	size_t len = 0;
	str = realloc(NULL, sizeof(char)*size);//size is start size
	if(!str)return str;
	while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
		str[len++]=ch;
		if(len==size){
			str = realloc(str, sizeof(char)*(size+=16));
			if(!str)return str;
		}
	}
	str[len++]='\0';

	return realloc(str, sizeof(char)*len);
}