#include<stdio.h>
#include<string.h>
 #include<stdlib.h>

// test the remove char 'ab' and c from one string
// one way is getting the src from arg, other is from input


void bytewise(char *dst, const char *src);
void with_memmove(char *dst, const char *src);

int main(int argc, char **argv){

	//	char *src = argv[1];
	char *src;
	fgets(src, 10,stdin);
	char *dst = (char*)malloc(sizeof(src)+1);

	bytewise(dst,src);
	//	with_memmove(dst,src);
	printf("%s\n",dst);	
	free(dst);
	
	return 0;
}

void with_memmove(char *dst, const char *src){

	char *position = strstr(src, "ab");
	strncpy(dst, src,strlen(src)-strlen(position));	
}



void bytewise(char *dst, const char *src){

	int i=0,j = 0;
	for(;src[i] != '\0';i++){
		if(src[i] == 'a' && src[i+1] == 'b'){
			i++;
			continue;
		}
	
		if(src[i] == 'c')
			continue;
		dst[j] = src[i];
		j++;
	}
	dst[j] = '\0';
	
}
