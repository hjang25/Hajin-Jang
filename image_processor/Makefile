# This is a makefile for 3 source files: image_manip.c, ppm_io.c, project.c
# Typing 'make' or 'make project' will create the executable named 'project'

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

project: project.o ppm_io.o image_manip.o
	$(CC) -o project project.o ppm_io.o image_manip.o -lm

checkerboard: checkerboard.o ppm_io.o
	$(CC) -o checkerboard checkerboard.o ppm_io.o -lm

img_cmp: img_cmp.o ppm_io.o
	$(CC) -o img_cmp img_cmp.o ppm_io.o -lm

img_cmp.o: img_cmp.c ppm_io.h
	$(CC) $(CFLAGS) -c img_cmp.c -lm

checkerboard.o: checkerboard.c ppm_io.h
	$(CC) $(CFLAGS) -c checkerboard.c -lm

project.o: project.c ppm_io.h image_manip.h
	$(CC) $(CFLAGS) -c project.c -lm

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c -lm

image_manip.o: image_manip.c image_manip.h ppm_io.h
	$(CC) $(CFLAGS) -c image_manip.c -lm

clean:
	rm -f *.o project