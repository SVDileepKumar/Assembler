#include <string.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct Symbols Symbols;
int pc = 0x0000;
char nemos[100][10];
int no_codes = 0;

struct Symbols{
   int address;
   char label[10];
   int flag;
   Symbols *next;
}*first,*last;


int searchCode(char *code){
    int i = 0;

    for(i=0; i<no_codes; i++){
        
        if(strcmp(code, nemos[i]) == 0){
            
            return 1;
        }
    }
    return 0;
}

void insert(Symbols* s){
   if(first == NULL){
      
      first = last = s;

   }else{
      last->next = s;
      last = s;
   }  
}

void build(char *lable){
    
    if(first == NULL){
        Symbols *s = malloc(sizeof *s);
        s->address = pc; strcpy(s->label , lable); 
        s->next = NULL;s->flag = 0;
        insert(s);
    }
    else{
        int p = search(lable);

        if(p){
           Symbols *s = malloc(sizeof *s);
           s->address = pc; strcpy(s->label , lable); 
           s->next = NULL;s->flag = 0;
           insert(s);        
        }
    }  
}

void display(){
   Symbols *s = first;
   printf("%s \t%s\n","Address", "Label" );
   while (s != NULL){
      printf("%04x       \t%s\n", s->address, s->label);
      s = s->next;
   }
}

int search(char *label){
   Symbols *s = first;
   
  
   while(s != NULL){
      int l = strcmp(label, s->label);
      if( l == 0 ){
         return 0;
      }
      s = s->next;
   }
   return 1;
}

int main()
{
    FILE *ptr_file,*fp;
    first = last = NULL;
    
    char code[3][10];
    char buf[100],buf2[100];
    char *token1,*token2;
    char **ptr;
    int ret;

    
    //open file
    ptr_file =fopen("input.txt","r");  

    int i=0;
    //read input file 
    while (fgets(buf,1000, ptr_file)!=NULL){
         //printf("%s",buf);
        token1 = strtok(buf, " \t\n");
        i=0;
        //get each coloumn
         while( token1 != NULL ) 
        {   
            if(i<3){

               strcpy(code[i] , token1);
          
               //printf("%s%d\n", code[i], i);
              i++;
            }
            //printf("%s %d\n", code[i-1], i);

            token1 = strtok(NULL, " \n");
        }
            
        if(i == 3){
            if(strcmp(code[1], "START") == 0){
                int c = (int)strtol(code[2], NULL, 16); //atoi(code[2]);
                pc = pc + c;
                printf("%s %04x\n", code[1], c);
                //continue;
            }
            else{
                build(code[0]);

                if(strcmp(code[1], "WORD") == 0){
                    pc += 3;
                    //printf("%d ", pc);
                }
                else if(strcmp(code[1], "RESW") == 0){
                    pc = pc + 3;
                    //printf("%d ", pc);
                }
                else if(strcmp(code[1], "RESB") == 0){
                    int c = (int)strtol(code[2], NULL, 10);                         
                    pc = pc + c;

                }
                else if(strcmp(code[1], "BYTE") == 0){
                    if(code[2][0] == 'C' ){
                    int l = strlen(code[2]) -3;
                    //printf("%d\n", l);
                    pc += l;
                    }else{
                        pc = pc + 1;
                    }
                }
                else{
                    pc += 3;
                }
            }
        }
        else if(i == 2 || i == 1){
            
            pc += 3;
          // printf("%s %04x\n", code[i-1], pc);
           
        }
        //printf("%04x  %s\n ", pc, code[0]);
    }   
           
    display();
    fclose(ptr_file);
    
    return 0;
}