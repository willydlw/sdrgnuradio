#include <stdio.h>

int main(){
	char *msg = "abcd";
	FILE* fptr;
	int i;

	fptr = fopen("testabcd.txt", "w");
	if(fptr == NULL){
		fprintf(stderr, "file open error\n");
		return -1;
	}

	for(i = 0; i < 512; ++i){
		fprintf(fptr, "%s", msg);
	}

	fclose(fptr);
	return 0;
}
