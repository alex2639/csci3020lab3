#include <pthread.h>
#include <stdlib.h>

#include <stdio.h>
#define NUM_THREADS     11

int main(int argc, char *argv[]){
  pthread_t threads[NUM_THREADS];
  bool valid=true;
  FILE *file;
  file=fopen("puzzle.txt");

  //read from file and get numbers

  //use tokenizer to get each value

  //initialize row array to store numbers on each row [][]

  //initialize column array to store numbers on each column [][]

  //initialize box array to store numbers on each 3x3 box

  //use threads to validate columns, rows, and each 3x3 box
  //make sure that each column, row, and 3x3 box have unique
  //numbers

  //valid is initially set to true. Once a violation is found,
  //set valid to false and break

  if(valid){
    printf("This is a valid grid\n");
  } else {
    printf("This is an invalid grid\n");
  }

  return 0;
}
