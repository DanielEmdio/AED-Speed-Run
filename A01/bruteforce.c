#define _max_road_size_  800  // the maximum problem size
#define _min_road_speed_   2  // must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_   9  // must not be larger than 9 (only because of the PDF figure)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "make_custom_pdf.c"
#include "../P02/elapsed_time.h"

// change macros to change the solving method
#define USE_SOLVE_1 0
#define USE_SOLVE_2 0
#define USE_SOLVE_3 1

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_
static int solve_to_use = -1;

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

static solution_t solution, solution_best;
static double solution_elapsed_time; // time it took to solve the problem
static unsigned long solution_count; // effort dispended solving the problem

// TESTS

static int DP_test[1 + _max_road_size_]; // array used as a buffer for a test solution

static void solution_4(int move_number, int position, int speed, int final_position) {
  int i, new_speed;

  // record move
  solution_count++;
  solution.positions[move_number] = position;

  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_best.n_moves)
    {
      solution_best = solution;
      solution_best.n_moves = move_number;
    }

    return;
  }

  // no, try all legal speeds
  for(new_speed = speed + 1; new_speed >= speed - 1; new_speed--)
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++);

      if ((!DP_test[position] || DP_test[position] > move_number) && i > new_speed)
      {
        DP_test[position] = move_number + 1; // update the best path move number
        solution_4(move_number + 1, position + new_speed, new_speed, final_position);
      }
    }
}

static void solve_4(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_3: bad final_position\n");
    exit(1);
  }

  solution_elapsed_time = cpu_time();
  solution_count = 0ul;
  solution_best.n_moves = final_position + 100;

  // initialize array for dynamic programming solution
  for (int i = 0; i < 1 + final_position; i++)
    DP_test[i] = 0;

  solution_4(0, 0, 0, final_position);

  /*printf("\n\n");
  for (int i = 0; i < 1 + final_position; i++) {
    printf("%d ", DP_test[i]);
  }
  printf("\n\n");*/

  solution_elapsed_time = cpu_time() - solution_elapsed_time;
}

// DYNAMIC PROGRAMMING

static char DP[1 + _max_road_size_][9]; // array used as a buffer for dynamic programming

static void solution_3_DP(int move_number, int position, int speed, int final_position) {
  int i, new_speed;

  // record move
  solution_count++;
  solution.positions[move_number] = position;

  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_best.n_moves)
    {
      solution_best = solution;
      solution_best.n_moves = move_number;
    }

    return;
  }

  // no, try all legal speeds
  for(new_speed = speed + 1; new_speed >= speed - 1; new_speed--)
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++);

      if (!DP[position][new_speed] && i > new_speed) { // make sure we do not redo work
        DP[position][new_speed] = 1; // set the path as done
        solution_3_DP(move_number + 1, position + new_speed, new_speed, final_position);
      }
    }
}

static void solve_3(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_3: bad final_position\n");
    exit(1);
  }

  solution_elapsed_time = cpu_time();
  solution_count = 0ul;
  solution_best.n_moves = final_position + 100;

  // initialize array for dynamic programming solution
  for (int i = 0; i < 1 + final_position; i++)
    for (int j = 0; j < 9; j++)
      DP[i][j] = 0;

  solution_3_DP(0, 0, 0, final_position);

  /*for (int i = 0; i < 1 + final_position; i++) {
    for (int j = 0; j < 9; j++){
      printf("%d ", DP[i][j]);
    }
    printf("\n");
  }
  printf("\n");*/

  solution_elapsed_time = cpu_time() - solution_elapsed_time;
}

// CLEVER BRUTEFORCE

static int solution_2_clever_bruteforce(int move_number, int position, int speed, int final_position) {
  int i, new_speed, used_pos;

  // record move
  used_pos = 0;
  solution_count++;
  solution.positions[move_number] = position;

  // is it a solution?
  if(position == final_position && speed == 1)
  {
    solution_best = solution;
    solution_best.n_moves = move_number;

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

  solution_elapsed_time = cpu_time();
  solution_count = 0ul;
  solution_best.n_moves = final_position + 100;
  solution_2_clever_bruteforce(0, 0 ,0 , final_position);
  solution_elapsed_time = cpu_time() - solution_elapsed_time;
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

  if (solve_to_use == 1 || solve_to_use == -1)
  {
    // plain recursion
    solve_1(final_position);
    make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
    printf("mad road speeds:");
    for(i = 0;i <= final_position;i++)
      printf(" %d",max_road_speed[i]);
    printf("\n");
    printf("positions:");
    for(i = 0;i <= solution_1_best.n_moves;i++)
      printf(" %d",solution_1_best.positions[i]);
  } else {
    if (solve_to_use == 2)
    {
      // clever bruteforcing
      solve_2(final_position);
      make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_best.n_moves,&solution_best.positions[0],solution_elapsed_time,solution_count,"Clever Bruteforcing");
    } else if (solve_to_use == 3)
    {
      // dynamic programming
      solve_3(final_position);
      make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_best.n_moves,&solution_best.positions[0],solution_elapsed_time,solution_count,"DP");
    } else if (solve_to_use == 4)
    {
      // dynamic programming
      solve_4(final_position);
      make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_best.n_moves,&solution_best.positions[0],solution_elapsed_time,solution_count,"DP_test");
    }

    printf("mad road speeds:");
    for(i = 0;i <= final_position;i++)
      printf(" %d",max_road_speed[i]);
    printf("\n");
    printf("positions:");
    for(i = 0;i <= solution_best.n_moves;i++)
      printf(" %d",solution_best.positions[i]);
  }

  printf("\n");
}

int main(int argc,char *argv[argc + 1])
{
# define _time_limit_  3600.0
  int n_mec,final_position,print_this_one;
  char file_name[64];
  bool do_example = false;

  // basic argument check
  for (int i = 1; i < argc; i++)
   {
    // generate the example data
    if(argv[i][0] == '-' && argv[i][1] == 'e' && argv[i][2] == 'x')
    {
      do_example = true;
    } else if (argv[i][0] == '-' && argv[i][1] == 's') // check if user specified the solve methid to use
    {
      if (argv[i][2] == '1')
      {
        solve_to_use = 1;
      } else if (argv[i][2] == '2')
      {
        solve_to_use = 2;
      } else if (argv[i][2] == '3')
      {
        solve_to_use = 3;
      } else if (argv[i][2] == '4')
      {
        solve_to_use = 4;
      }
    }
  }

  // generate the example data
  if(do_example)
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

    if (solve_to_use == 1 || solve_to_use == -1) // use the plain recursion solve method as the default one
    {
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
    } else if (solve_to_use == 2) {
      // clever bruteforcing
      if(solution_elapsed_time < _time_limit_)
      {
        solve_2(final_position);
        if(print_this_one != 0)
        {
          sprintf(file_name,"%03d_2.pdf",final_position);
          make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_best.n_moves,&solution_best.positions[0],solution_elapsed_time,solution_count,"Clever bruteforcing");
        }
        printf(" %3d %16lu %9.3e |",solution_best.n_moves,solution_count,solution_elapsed_time);
      }
    } else if (solve_to_use == 3) {
      // dynamic programming
      if(solution_elapsed_time < _time_limit_)
      {
        solve_3(final_position);
        if(print_this_one != 0)
        {
          sprintf(file_name,"%03d_3.pdf",final_position);
          make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_best.n_moves,&solution_best.positions[0],solution_elapsed_time,solution_count,"DP");
        }
        printf(" %3d %16lu %9.3e |",solution_best.n_moves,solution_count,solution_elapsed_time);
      }
    } else if (solve_to_use == 4) {
      // dynamic programming
      if(solution_elapsed_time < _time_limit_)
      {
        solve_4(final_position);
        if(print_this_one != 0)
        {
          sprintf(file_name,"%03d_4.pdf",final_position);
          make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_best.n_moves,&solution_best.positions[0],solution_elapsed_time,solution_count,"DP_test");
        }
        printf(" %3d %16lu %9.3e |",solution_best.n_moves,solution_count,solution_elapsed_time);
      }
    }

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