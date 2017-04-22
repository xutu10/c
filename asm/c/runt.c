#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/resource.h>


#define HEAP_SIZE 20*1048576  
#define MAX_SIZE 14*1048576  
#define STACK_SIZE 2*1048576 

typedef enum {Ref,Prim} RefPrimFlag;

typedef struct {
  RefPrimFlag refprimflag;
  int refprimval;
} RefPrim;

void printRefPrim(RefPrim* rp){
  switch (rp->refprimflag ){
    case Ref:  printf("(-> %d)",rp->refprimval);break;
    case Prim: printf(" %d",rp->refprimval);break;
  }
}

void printStack();
void printHeap();

int callingArg;


typedef struct {
  int constrNr;
  int arity;
  RefPrim* args;
} HeapNode;

void printHeapNode(HeapNode* h){
  printf("Constr<%d %d>(",h->constrNr,h->arity);
  int i;
  for (i=0;i<h->arity;i++){
    if (i>0) printf(",");
      printRefPrim(h->args+i);
  }
  printf(")");
}


HeapNode** heap;
HeapNode** gcheap;

unsigned int hp=0;
unsigned int heapSize = 909;
unsigned int lastUsedheapSize = 909;;

int nr;
int arity;

HeapNode* newCell(unsigned int arity,int nr){
  HeapNode* newNode = (HeapNode*)malloc(sizeof(HeapNode));
  newNode->constrNr = nr;
  newNode->arity = arity;
  return newNode;
}

void gc();

HeapNode* newHeapCell(){
  if (hp>=heapSize-1){
    gc();
  }

  RefPrim* args = (RefPrim*)malloc(sizeof(RefPrim)*arity);
  HeapNode* newNode = (HeapNode*)malloc(sizeof(HeapNode));

  newNode->constrNr = nr;
  newNode->arity = arity;
  newNode->args = args;

  heap[hp] = newNode;
  hp++;

  return newNode;
}


RefPrim* stack;
RefPrim* baseptr;
unsigned int sp = 0;
unsigned int sb = 0;

void unpack(){
  int heapRef = stack[--sp].refprimval;
  int nr = heap[heapRef]->constrNr;
  int i;

  if (sp+heap[heapRef]->arity+1>=STACK_SIZE){
    printf("stack full\n");
    exit(-1);
  }

  for (i=heap[heapRef]->arity-1;i>=0;i--){
    stack[sp].refprimflag = heap[heapRef]->args[i].refprimflag;
    stack[sp].refprimval = heap[heapRef]->args[i].refprimval;
    sp++;
  }
  stack[sp].refprimflag = Prim;
  stack[sp].refprimval = heap[heapRef]->arity;
  sp++;
  stack[sp].refprimflag = Prim;
  stack[sp].refprimval = nr;
  sp++;
}


int heapRef;
void project(){
  heapRef = stack[--sp].refprimval;
  stack[sp].refprimflag = heap[heapRef]->args[callingArg].refprimflag;
  stack[sp].refprimval = heap[heapRef]->args[callingArg].refprimval;
  sp++;
}


void pushPrim(){
  if (sp+1>STACK_SIZE){
    printf("stack full\n");
    exit(-1);
  }
  stack[sp].refprimflag=Prim;
  stack[sp].refprimval=callingArg;
  sp++;
}

void pushRef(){
  if (sp+1>STACK_SIZE){
    printf("stack full\n");
    exit(-1);
  }
  stack[sp].refprimflag=Ref;
  stack[sp].refprimval=callingArg;
  sp++;
}

void pop(){
  sp--;
}

void popTops(){
  sp = sp - stack[--sp].refprimval-1;
}

void push(){
  if (sp+1>STACK_SIZE){
    printf("stack full\n");
    exit(-1);
  }
  stack[sp].refprimflag = stack[sb-callingArg].refprimflag;
  stack[sp].refprimval  = stack[sb-callingArg].refprimval;
  sp++;
}


void pushtop(){
  if (sp+1>STACK_SIZE){
    printf("stack full\n");
    exit(-1);
  }
  stack[sp].refprimflag = stack[sp-callingArg-1].refprimflag;
  stack[sp].refprimval  = stack[sp-callingArg-1].refprimval;
  sp++;
}

HeapNode** allocHeap(unsigned int size) {
  HeapNode** result = (HeapNode**) malloc(size*sizeof(HeapNode*));
  if (!result) {
    printf("could not allocate heap\n");
    exit(-1);
  }
  return result;
}

unsigned int copy(unsigned int heapIndex){
  HeapNode* oldcell = heap[heapIndex];

  if (oldcell->constrNr == -1){
    return oldcell->arity;
  }else{
    HeapNode* newcell = newCell(oldcell->arity,oldcell->constrNr);
    newcell->args=oldcell->args;
    oldcell->constrNr = -1;
    oldcell->arity = lastUsedheapSize;
    unsigned int result = lastUsedheapSize;
    gcheap[lastUsedheapSize] = newcell;
    lastUsedheapSize++;

    int ar=newcell->arity;
    int ai;
    for (ai=0;ai< ar;ai++){
      newcell->args[ai].refprimflag = oldcell->args[ai].refprimflag;
      if (oldcell->args[ai].refprimflag == Ref){
        newcell->args[ai].refprimval = copy(oldcell->args[ai].refprimval);
      }else{
        newcell->args[ai].refprimval = oldcell->args[ai].refprimval;
      }
    }

    return result;
  }
}

void gc(){
  printf("Starting gc. old heap: %d",hp-1);
  int luh3  = lastUsedheapSize*3;
  int newSize = luh3 > heapSize ? luh3 : heapSize;
  if (newSize>MAX_SIZE) newSize = MAX_SIZE;
  printf(" new size: %d\n",newSize);
  gcheap = allocHeap(newSize);
  lastUsedheapSize = 0;

  int i;
  for (i=0;i<sp;i++ ){
    if ((stack+i)->refprimflag==Ref){
      (stack+i)->refprimval = copy((stack+i)->refprimval);
    }
  }

  //alten heap löschen
  //int i;
  for (i=0;i<hp;i++){
    if (heap[i]->constrNr!=-1)free(heap[i]->args);
    free(*(heap+i));
  }
  free(heap);

  //neuen heap setzen
  heap = gcheap;
  heapSize = newSize;
  hp = lastUsedheapSize;
  printf("  Ending gc. new heap: %d\n",hp);
}

void printStack() {
  int i;
  for (i = 0; i < sp; i++) {
    printRefPrim(&stack[i]);
    printf("\n");
  }
}

void printHeap(){
  printf("\nHEAP\n");
  int i;
  for (i=0;i<hp;i++){
    printf("%d:  ",i);
    printHeapNode(heap[i]);
    printf("\n");
  }
}

void ret(){
  RefPrim result = stack[--sp];
  sb = stack[--sp].refprimval;
  sp -= callingArg;
  stack[sp++] = result;
}


int main (int argc, char **argv)
{
  const rlim_t kStackSize = 125 * 1024 * 1024;
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0){
    if (rl.rlim_cur < kStackSize){
      rl.rlim_cur = kStackSize;
      result = setrlimit(RLIMIT_STACK, &rl);
      if (result != 0){
	fprintf(stderr, "setrlimit returned result = %d\n", result);
      }
    }
  }
  stack=(RefPrim*)malloc(STACK_SIZE*sizeof(RefPrim));
  baseptr=stack;

  heap = allocHeap(heapSize * sizeof(HeapNode*));
  // go();
  gc();
  printf("fertig\n");
  printf("\nSTACK\n");
  printStack();
  printHeap();
  return 0;
}

