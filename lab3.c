#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_THREADS     11
#define ROWS 9
#define COLUMNS 9
#define GRID_SIZE 3
#define ROW_THREAD 0
#define COLUMN_THREAD 1
#define SUB_GRID_THREADS 9

struct row_column_pair
{
  int row;
  int column;
};

//Functions used by pthread_create
void * check_row(void *row);
void * check_column(void *column);
void * check_sub_grid(void *sub_grid);

/*
The functions that check the rows/columns/subgrids for validity
All use an array of bools, set to the row/column size + 1.
Since the rows and columns of the grid range from 1-9, and since we skip over 0's*,
the values in the grid can be used to index into the array of bools. 
We simply loop through the grid and set the indexes to true the first time that value is encountered.
If the bool at that value is already true, it means it has already been encountered and thus,
the solution is invalid.

*As per instructions, 0's are ignored
*/
void checkrow(int row);
void checkColumn(int column);
void checksubgrid(int r, int c);

int grid[COLUMNS][ROWS];

int main(int argc, char *argv[])
{
  pthread_t threads[NUM_THREADS];
  bool valid=true;
  FILE *file;
  file=fopen("puzzle.txt", "r");

  int row = 0;
  int column = 0;
  int number;
  //Read the file
  while(fscanf(file, "%d", &number) > 0)
  {
    grid[column][row] = number;
    row ++;
    if(row >= ROWS)
    {
      row = 0;
      column ++;
    }
  }

  //Create the threads to check if the rows and columns are valid
  pthread_create(&threads[ROW_THREAD], NULL, check_row, NULL);
  pthread_create(&threads[COLUMN_THREAD], NULL, check_column, NULL);

  //The index of the thread that checks the validity of a subgrid
  int a = COLUMN_THREAD + 1; 
  //Nested loop to create the subgrid threads
  //As the grids are GRID_SIZE X GRID_SIZE, move the row and column over by GRID_SIZE each iteration
  for(int r = 0; r < ROWS; r +=GRID_SIZE)
  {
    for(int c = 0; c < COLUMNS; c +=GRID_SIZE)
    {
      struct row_column_pair s;
      s.column = c;
      s.row = r;
      struct row_column_pair *f = &s; //More confident in this than malloc/calloc
      pthread_create(&threads[a], NULL, check_sub_grid, (void *)f);
      a++;
    }
  }
  for(int i = 0; i < NUM_THREADS; i ++)
  {
    void *validity;
    pthread_join( threads[i], &validity);  
    valid = (bool)validity;
    //If valid is false it means one of the threads found that
    //the area it was checking was invalid. In that case break from the loop
    if(!valid)
    {
      break;
    }
  }
  if(valid)
  {
    printf("This is a valid grid\n");
  } 
  else 
  {
    printf("This is an invalid grid\n");
  }

  return 0;
}

void checkrow(int row)
{
  bool found_numbers[ROWS + 1]  = { 0 };
  for(int i = 0; i < ROWS; i ++)
  {
    int number = grid[row][i];
    if(number > 0)
    {
      if(found_numbers[number])
      {
        pthread_exit((void*)false);
      }
      else
      {
        found_numbers[number] = true;
      }
    }
  }
}

void checkcolumn(int column)
{
  bool found_numbers[COLUMNS + 1]  = { 0 };
  for(int i = 0; i < COLUMNS; i ++)
  {
    int number = grid[i][column];
    if(number > 0)
    {
      if(found_numbers[number])
      {
        pthread_exit((void*)false);
      }
      else
      {
        found_numbers[number] = true;
      }
    } 
  }
}

void checksubgrid(int row, int column)
{
  bool found_numbers[GRID_SIZE * GRID_SIZE]  = { 0 };
  for(int r = 0; r < GRID_SIZE; r ++)
  {
    for(int c = 0; c < GRID_SIZE; c ++)
    {
      int number = grid[column + c][row + r];
      if(number > 0)
      {
        if(found_numbers[number])
        {
          pthread_exit((void*)false);
        }
        else
        {
          found_numbers[number] = true;
        }
      }
    }
  }
}

void * check_row(void *r)
{
  for(int i = 0; i < ROWS; i ++)
  {
    checkrow(i);
  }
  return (void*)true;   
}

void * check_column(void *c)
{
  for(int i = 0; i < COLUMNS; i ++)
  {
    checkcolumn(i);
  }
  return (void*)true; 
}

void * check_sub_grid(void *sub_grid)
{
  struct row_column_pair *s = (struct row_column_pair*)sub_grid;
  checksubgrid(s->column, s->row);

  return (void*)true;
}