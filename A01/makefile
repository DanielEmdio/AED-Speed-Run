#
# Tomás Oliveira e Silva, AED, September 2022
#
# makefile to compile the A.01 assignment (speed run)
#

clean:
	rm -rf a.out *.pdf speed_run speed_run_with_zlib solution_speed_run solution_speed_run_with_zlib

solution_speed_run:		solution_speed_run.c make_custom_pdf.c
	cc -Wall -Wextra -O3 -D_use_zlib_=0 solution_speed_run.c -o solution_speed_run -lm -lz

solution_speed_run_with_zlib:	solution_speed_run.c make_custom_pdf.c
	cc -Wall -Wextra -O3 -D_use_zlib_=1 solution_speed_run.c -o solution_speed_run_with_zlib -lm -lz

speed_run:		speed_run.c make_custom_pdf.c
	cc -Wall -Wextra -O3 -D_use_zlib_=0 speed_run.c -o speed_run -lm -lz

speed_run_with_zlib:	speed_run.c make_custom_pdf.c
	cc -Wall -Wextra -O3 -D_use_zlib_=1 speed_run.c -o speed_run_with_zlib -lm -lz