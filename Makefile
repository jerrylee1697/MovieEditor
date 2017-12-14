#######################################################################
# Homework Assignment 5, for EECS 22, Fall 2017
#
# Author: Tim Schmidt
# Date: 11/09/2017
#
# Makefile: Makefile for HW 5
#
#######################################################################

# Variables
CC = gcc
DEBUG = -g -DDEBUG
CFLAGS = -Wall -ansi -std=c99 $(DEBUG) -c
LFLAGS = -Wall -ansi -std=c99 $(DEBUG)
MAIN = -DMAIN

# Default target
all: MovieLab
######################### Generate object files #######################
# Target for MovieLab.o
MovieLab.o: Constants.h FileIO.h Image.h ImageList.h Movie.h MovieLab.c 
	$(CC) $(CFLAGS) $(DEBUG) MovieLab.c -o MovieLab.o
# Target for FileIO.o
FileIO.o: Image.h FileIO.h FileIO.c 
	$(CC) $(CFLAGS)  FileIO.c -o FileIO.o
# Target for DIPs.o
DIPs.o: Image.h DIPs.h DIPs.c
	$(CC) $(CFLAGS) DIPs.c -o DIPs.o
# Target for Image.o
Image.o: Image.h Image.c 
	$(CC) $(CFLAGS) Image.c -o Image.o
# Target for ImageList.o
ImageList.o: Image.h ImageList.h ImageList.c
	$(CC) $(CFLAGS) ImageList.c -o ImageList.o
# Target for Movie.o
Movie.o: ImageList.h Movie.h Movie.c
	$(CC) $(CFLAGS) Movie.c -o Movie.o

######################### Generate the executable #####################
# Target for MovieLab
MovieLab: DIPs.o FileIO.o Image.o ImageList.o Movie.o MovieLab.o
	$(CC) -lm $(LFLAGS) $(DEBUG) DIPs.o FileIO.o Image.o ImageList.o Movie.o MovieLab.o -o MovieLab

###############################  others  ##############################

# Target for clean-up
clean:
	rm -f MovieLab
	rm -f *.o
	rm -f out.yuv

test: MovieLab
	./MovieLab
