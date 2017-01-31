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

