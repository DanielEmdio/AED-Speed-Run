#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "make_custom_pdf.c"
#include "../P02/elapsed_time.h"

#define _max_road_size_  800  // the maximum problem size
#define _min_road_speed_   2  // must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_   9  // must not be larger than 9 (only because of the PDF figure)

// change macros to change the solving method
#define USE_SOLVE_1 0
#define USE_SOLVE_2 0
#define USE_SOLVE_3 1

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_

static void init_road_speeds(void)
{
  double speed;
  int i;

  for(i = 0; i <= _max_road_size_; i++)
  {
    speed = (double)_max_road_speed_ * (0.55 + 0.30 * sin(0.11 * (double)i) + 0.10 * sin(0.17 * (double)i + 1.0) + 0.15 * sin(0.19 * (double)i));
    max_road_speed[i] = (int)floor(0.5 + speed) + (int)((unsigned int)random() % 3u) - 1;

    if(max_road_speed[i] < _min_road_speed_)
      max_road_speed[i] = _min_road_speed_;
    if(max_road_speed[i] > _max_road_speed_)
      max_road_speed[i] = _max_road_speed_;
  }
}

typedef struct
{
  int n_moves;                         // the number of moves (the number of positions is one more than the number of moves)
  int positions[1 + _max_road_size_];  // the positions (the first one must be zero)
} solution_t;

// DYNAMIC PROGRAMMING

static solution_t solution_3, solution_3_best;
static double solution_3_elapsed_time; // time it took to solve the problem
static unsigned long solution_3_count; // effort dispended solving the problem

static int solution_3_DP(int move_number, int position, int speed, int final_position) {

}

static void solve_3(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_3: bad final_position\n");
    exit(1);
  }

  solution_3_elapsed_time = cpu_time();
  solution_3_count = 0ul;
  solution_3_best.n_moves = final_position + 100;
  solution_3_DP(0, 0 ,0 , final_position);
  solution_3_elapsed_time = cpu_time() - solution_3_elapsed_time;
}

// CLEVER BRUTEFORCE

static solution_t solution_2, solution_2_best;
static double solution_2_elapsed_time; // time it took to solve the problem
static unsigned long solution_2_count; // effort dispended solving the problem

static int solution_2_clever_bruteforce(int move_number, int position, int speed, int final_position) {
  int i, new_speed, used_pos;

  // record move
  used_pos = 0;
  solution_2_count++;
  solution_2.positions[move_number] = position;

  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_2_best.n_moves)
    {
      solution_2_best = solution_2;
      solution_2_best.n_moves = move_number;
    }

    return 0;
  }

  // no, try all legal speeds starting with increasing
  for(new_speed = speed + 1; new_speed >= speed - 1; new_speed--){
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++); // check next cases

      if(i > new_speed) {
        used_pos++;
        int used_total = 0;
        for (int j = 1; j <= used_pos; j++) used_total += j;

        if (used_pos + used_total <= final_position)
          if(!solution_2_clever_bruteforce(move_number + 1, position + new_speed, new_speed, final_position))
            return 0;
      }
    }
  }

  return 1;
}

static void solve_2(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_2: bad final_position\n");
    exit(1);
  }

  solution_2_elapsed_time = cpu_time();
  solution_2_count = 0ul;
  solution_2_best.n_moves = final_position + 100;
  solution_2_clever_bruteforce(0, 0 ,0 , final_position);
  solution_2_elapsed_time = cpu_time() - solution_2_elapsed_time;
}

// PLAIN RECURSION

static solution_t solution_1, solution_1_best;
static double solution_1_elapsed_time; // time it took to solve the problem
static unsigned long solution_1_count; // effort dispended solving the problem

static void solution_1_recursion(int move_number, int position, int speed, int final_position)
{
  int i, new_speed;

  // record move
  solution_1_count++;
  solution_1.positions[move_number] = position;

  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_1_best.n_moves)
    {
      solution_1_best = solution_1;
      solution_1_best.n_moves = move_number;
    }

    return;
  }

  // no, try all legal speeds
  for(new_speed = speed - 1; new_speed <= speed + 1; new_speed++)
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++);

      if(i > new_speed)
        solution_1_recursion(move_number + 1, position + new_speed, new_speed, final_position);
    }
}

static void solve_1(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_1: bad final_position\n");
    exit(1);
  }

  solution_1_elapsed_time = cpu_time();
  solution_1_count = 0ul;
  solution_1_best.n_moves = final_position + 100;
  solution_1_recursion(0, 0 ,0 , final_position);
  solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}

// EXAMPLE PROBLEM

static void example(void)
{
  int i,final_position;

  srandom(0xAED2022);
  init_road_speeds();
  final_position = 30;

#if USE_SOLVE_1
  solve_1(final_position);
  printf("boas\n");
#endif

  
  make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
  printf("mad road speeds:");
  for(i = 0;i <= final_position;i++)
    printf(" %d",max_road_speed[i]);
  printf("\n");
  printf("positions:");
  for(i = 0;i <= solution_1_best.n_moves;i++)
    printf(" %d",solution_1_best.positions[i]);
  printf("\n");
}

/*static void example_with2(void)
{
  int i,final_position;

  srandom(0xAED2022);
  init_road_speeds();
  final_position = 30;
  solve_2(final_position);
  make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_2_best.n_moves,&solution_2_best.positions[0],solution_2_elapsed_time,solution_2_count,"Plain recursion");
  printf("mad road speeds:");
  for(i = 0;i <= final_position;i++)
    printf(" %d",max_road_speed[i]);
  printf("\n");
  printf("positions:");
  for(i = 0;i <= solution_2_best.n_moves;i++)
    printf(" %d",solution_2_best.positions[i]);
  printf("\n");
}*/

int main(int argc,char *argv[argc + 1])
{
# define _time_limit_  3600.0
  int n_mec,final_position,print_this_one;
  char file_name[64];

  // generate the example data
  if(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'e' && argv[1][2] == 'x')
  {
    example();
    return 0;
  }

  // initialization
  n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
  srandom((unsigned int)n_mec);
  init_road_speeds();

  // run all solution methods for all interesting sizes of the problem
  final_position = 1;
  solution_1_elapsed_time = 0.0;
  printf("    + --- ---------------- --------- +\n");
  printf("    |                plain recursion |\n");
  printf("--- + --- ---------------- --------- +\n");
  printf("  n | sol            count  cpu time |\n");
  printf("--- + --- ---------------- --------- +\n");

  while(final_position <= _max_road_size_/* && final_position <= 34*/)
  {
    print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf("%3d |",final_position);

#if 0
    // first solution method (very bad)
    if(solution_1_elapsed_time < _time_limit_)
    {
      solve_1(final_position);
      if(print_this_one != 0)
      {
        sprintf(file_name,"%03d_1.pdf",final_position);
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
      }
      printf(" %3d %16lu %9.3e |",solution_1_best.n_moves,solution_1_count,solution_1_elapsed_time);
    }
#elif 0
    // first solution method (very bad)
    if(solution_2_elapsed_time < _time_limit_)
    {
      solve_2(final_position);
      if(print_this_one != 0)
      {
        sprintf(file_name,"%03d_2.pdf",final_position);
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_2_best.n_moves,&solution_2_best.positions[0],solution_2_elapsed_time,solution_2_count,"Clever bruteforcing");
      }
      printf(" %3d %16lu %9.3e |",solution_2_best.n_moves,solution_2_count,solution_2_elapsed_time);
    }
#else
    // first solution method (very bad)
    if(solution_3_elapsed_time < _time_limit_)
    {
      solve_3(final_position);
      if(print_this_one != 0)
      {
        sprintf(file_name,"%03d_3.pdf",final_position);
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_3_best.n_moves,&solution_3_best.positions[0],solution_3_elapsed_time,solution_3_count,"DP");
      }
      printf(" %3d %16lu %9.3e |",solution_3_best.n_moves,solution_3_count,solution_3_elapsed_time);
    }
#endif

    // done
    printf("\n");
    fflush(stdout);

    // new final_position
    if(final_position < 50)
      final_position += 1;
    else if(final_position < 100)
      final_position += 5;
    else if(final_position < 200)
      final_position += 10;
    else
      final_position += 20;
  }

  printf("--- + --- ---------------- --------- +\n");
  return 0;
# undef _time_limit_
}