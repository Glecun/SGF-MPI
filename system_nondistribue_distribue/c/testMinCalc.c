#include <stdio.h>
#include <stdlib.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })

int main(){

	int size = 5000;

	while(size >0){
		if(size > 1024){
			printf("1024");
			size -=1024;
		} else {
			printf("%d\n", size);
			size -=size;
		}	
	}


	return 0;
}
