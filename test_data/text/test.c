#include <stdio.h>

int main(){
	char *msg = "0123";
	FILE* fptr;
	int i;

	fptr = fopen("test0123.txt", "w");
	if(fptr == NULL){
		fprintf(stderr, "file open error\n");
		return -1;
	}

	for(i = 0; i < 128; ++i){
		fprintf(fptr, "%s", msg);
	}

	fclose(fptr);
	return 0;
}