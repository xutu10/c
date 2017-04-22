#include <stdio.h>
#include <stdlib.h>

int ways,sets;
int checkCache(int[][8], int , int);

int main(int argc, char *argv[])
{
	printf("Cache-Simulator ...\n");
	char* src;
	int size,lines=0;
	char usetyp;
	src = argv[1];
	size = atoi(argv[2]);
	ways = atoi(argv[3]);
	usetyp = *argv[4];
		
	lines = size/16;
	sets = lines/ways;
	// output basic info
	printf("cachelinesize :16 \n");
	printf("cachelines : %d \n",lines);
	printf("ways :%d\n",ways);
	printf("sets :%d \n",sets);

	//info for cache table
	int index, tag=0;
	// info which read from source
	int typ, addr = 0;
	// info for counting
	int isHit=0,hit=0,num=0,itemcount = 0;
	// with double array simulate the cachetable
	int cachetable[2048][8] = {{0}};
	//read the info from source file
	FILE* file;
	file = fopen(src,"r");	
	if(feof(file))
		printf("File not found\n");
	while(!feof(file)){
		// set the flag as 0, check the typ, if this item is needed
		int ret,tmpaddr=0;
		ret = fscanf(file,"%d",&typ);
		// if at end of the file, finish the loop
		if(ret == -1)
			break;
		fscanf(file,"%x",&addr); 
		// if meet the condition, go to check the cachetable
		if(usetyp == 'U'||(usetyp =='I'&&typ == 2)||(usetyp =='D'&&typ!=2)){			
			tmpaddr = addr/16;
			index = tmpaddr%sets;
			tag = tmpaddr/sets;		
			// call the checkCache to check, if this address already in the cache
			isHit = checkCache(cachetable,index,tag);
			if(isHit)
				hit++;
			// number of availabe items by the usetyp
			itemcount++;
		}
		// all of items in the source
		num++;
	}
	fclose(file);
	printf("Anzahl der Gesamtzugriffe: %d \n",num);
	switch(usetyp){
		case 'U': printf("Anzahl der Datenzugriffe: %d\n", num); break;
		case 'D': printf("Anzahl der Datenzugriffe: %d \n",itemcount); break;
		case 'I': printf("Anzahl der Instrcutionzugriffe:%d  \n",itemcount); break;
	}
	double rate = (double)hit/itemcount; 
	printf("number of hit:%d, number of miss: %d, Hit-Rate: %f \n",hit, itemcount-hit,rate);

	return 0;
}


int checkCache(int cachetable[][8],int index, int tag ){  //????
	int j,m,flag=0;
	// check, if tag in ways
	for(j=0;j<ways;j++){
		// if already in the cache
		if(cachetable[index][j]== tag){
			flag =1;
			// set ways by order according to LRU, first element is always the recentest one, last is least recent
			// everytime update the ways, no matter hit or not
			for(m=j;m>0;m--)
				cachetable[index][m] = cachetable[index][m-1];				
			// update first element, which always recentest be used
			cachetable[index][0] = tag;
			break;
			}
		}
		// if not found
		if(!flag){
			int j;
			// update all of ways
			for(j = ways;j>0;j-- )
				cachetable[index][j] = cachetable[index][j-1];	
			// update the recentest element
			cachetable[index][0] = tag;			
		}		
		return flag;
}