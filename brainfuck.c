/*********************************
Brainfuck interpreter
Autor Joerg Kulbartz yoshi@yomols.de
*********************************/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <ncurses.h>

typedef struct {
  char command;
  int (*func)(char** , char** );
} command;

int stepRight(char** instructions, char** data){
  (*data)++;
  return 0;
}

int stepLeft(char** instructions, char** data){
  (*data)--;
  return 0;
}

int increment(char** instructions, char** data){
  (**data)+=1;
  return 0;
}

int decrement(char** instructions, char** data){
  (**data)-=1;
  return 0;
}

int outPut(char** instructions, char** data){
  printf("%c", **data);
  return 0;
}

int inPut(char** instructions, char** data){
  scanf("%c", *data);
  return 0;
}

int jumpRightNonZero(char** instructions, char** data){
  //!!! Leads to SEGFAULT if no matching ]
  int counter=0;
  if(**data!='0')return 0;
  while((*instructions)++){
    printf("%c\n", **instructions);
    if(**instructions==']' && counter==0){

      printf("Output of [\n");
      printf("%c\t%c\t%c\n", *(*data -1 ), *(*data), *(*data+1));
      printf("%c\t%c\t%c\n", *(*instructions -1 ), *(*instructions), *(*instructions+1));
      return 0;
    }
    if(**instructions=='[') counter++;//This counts the matching parantheses.
    if(**instructions==']') counter--;
  }
  return 1;
}

int jumpLeftNonZero(char** instructions, char** data){
  //!!! Leads to SEGFAULT if no matching [
  int counter=0;
  if(**data=='0')return 0;
  while((*instructions)--){
    if(**instructions=='[' && counter==0){
      printf("Output of ]\n");
      printf("%c\t%c\t%c\n", *(*data -1 ), *(*data), *(*data+1));
      printf("%c\t%c\t%c\n", *(*instructions -1 ), *(*instructions), *(*instructions+1));
      return 0;
    }
    if(**instructions==']') counter++;//This counts the matching parantheses.
    if(**instructions=='[') counter--;
  }
  return 1;
}
    


command* init(void){

  command* trampolin=malloc(8*sizeof(command));

  trampolin[0].command='>';
  trampolin[0].func=stepRight;

  trampolin[1].command='<';
  trampolin[1].func=stepLeft;

  trampolin[2].command='+';
  trampolin[2].func=increment;

  trampolin[3].command='-';
  trampolin[3].func=decrement;

  trampolin[4].command='.';
  trampolin[4].func=outPut;

  trampolin[5].command=',';
  trampolin[5].func=inPut;

  trampolin[6].command='[';
  trampolin[6].func=jumpRightNonZero;

  trampolin[7].command=']';
  trampolin[7].func=jumpLeftNonZero;

  return trampolin;
}

int main(int argv, char* argc[]){

  char *dataStart;
  char *instructionStart;
  char *dataEnd;
  char *instructionEnd;

  char* band=(char*)malloc(100);
  strcpy(band, "000000000000");
  char* instructions="+[+[>+[>+<-]<-]>+<-]";
  char* datPtr=band;
  char* insPtr=instructions;
  command* trampolin;
  trampolin=init();

  dataStart=band;
  dataEnd=dataStart;
  while(*dataEnd != '\0') dataEnd++;
  
  instructionStart=instructions;
  instructionEnd=instructionStart;


  printf("Program:\n%s\n", instructions);
  printf("Data:\n%s\n", band);
  //Start of interpreter
  while(*instructionEnd != '\0') instructionEnd++;

  while(*insPtr!='\0'){
    int i=0;
    while(trampolin[i].command!=*insPtr){
      i++;
      if(i > 7)break;
    }
    printf("Vor exec: %c %c\n", *insPtr, *datPtr);
    printf("%i\t%c\t%c\n", trampolin[i].func(&insPtr, &datPtr), *insPtr, *datPtr);//Implicitly executes trampolin
    insPtr++;
  }
  printf("Here\t %c\t%c\n", *insPtr, *datPtr);
  printf("%s\n", band);

  return 0;

}
