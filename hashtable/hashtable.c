#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 400


typedef struct Symbols Symbols;
int pc = 0x0000;
char nemos[100][10];
int no_codes = 0;
int no_rows = 0;

struct Symbols{
   int address;
   char label[10];
   Symbols *next,*first,*last;
};

Symbols* createNode(char* label, int address){
	Symbols *s = malloc(sizeof *s);
	s->address = address;
	strcpy(s->label, label);
	s->next = NULL;
	s->first = NULL;
	s->last = NULL;
	return s;

}

void init(Symbols *table[size]){
	int i;
	
	for(i = 0; i< size; i++){
		table[i] = NULL;
	}
}

void insert(Symbols *table[size], Symbols *s, unsigned int index){

	if(table[index] == NULL){
		table[index] = s;
		table[index]->first = table[index]->last = s;
		no_rows++;
	}
	else{
		s->first = table[index];
		table[index]->last->next = s;
		table[index]->last = s;
	}
	//printf("%d\n", table[index]->address);

}

unsigned int hash( char *str)
{
    unsigned int hash = 0;
    int c;
    while (c = *str++){
        hash += c;
    }
    hash = hash % size;
    return hash;
}

int get(Symbols *table[size], char *label){
	unsigned int index = hash(label);
	int retaddr = -1;

	while(table[index] != NULL){
		
		int val = strcmp(table[index]->label, label);
		retaddr = table[index]->address;
		//printf("%s %s %d %04x\n",table[index]->label, label, val, retaddr );
		table[index] = table[index]->next;	
	}
	return retaddr;
}

int main(){

	FILE *fp;
	Symbols *table[size],*optable[size];
	fp = fopen("symout.txt", "r");
	char code[100],*token;
	unsigned int index = 0;
	
	init(table);
	if(fp == NULL){
		printf("Cannot open File\n");
		return 1;
	}
	while(fgets(code, 100, fp) != NULL){
		token = strtok(code, " \n");
		int i = 0;
		index = 0;
		int addr = 0x000;
		while(token != NULL){
			i++;
			if(i == 1){
				addr = (int)strtol(token, NULL, 16);
			}
			if(i == 2)
			{
			index = hash(token);
			//printf("%s %u\n", token, index);
				Symbols *s = createNode(token, addr);

				insert(table,s, index);
				int value = get(table, token);
				if(value != -1)
				{
					printf("%04x\n", value);
				}
				else
					printf("Key Doesnot exist\n");
				//printf("%04x \n",table[index]->address);
			}
			token = strtok(NULL, " \n");
			
		}

		//printf("%s\n", code);
	}

	printf("\n");

	FILE *fpr;
	init(optable);
	fpr = fopen("nemo.txt", "r");
	if(fpr == NULL){
		printf("Cannot open File\n");
		return 1;
	}
	
	while(fgets(code, 100, fpr) != NULL){
		token = strtok(code, " \n");
		int i = 0;
		index = 0;
		int addr = 0x000;
		char label[10] = "";
		while(token != NULL){
			i++;

			if(i == 1){
				index = hash(token);
				strcpy(label, token);
				
			}
			if(i == 2)
			{
				addr = (int)strtol(token, NULL, 16);
				//printf("%d %u\n", addr, index);
				Symbols *s = createNode(label, addr);

				insert(optable, s, index);
				int value = get(optable, label);
				if(value != -1){
					printf("%04x\n", value);
				}
				else
					printf("Key Doesnot exist\n");
				//printf("%04x \n",table[index]->address);
			}
			token = strtok(NULL, " \n");
			
		}

		//printf("%s\n", code);
	}

	fclose(fpr);
	return 0;
}