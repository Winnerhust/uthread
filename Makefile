#
# @file  Makefile
# @author chenxueyou
# @version 0.1
# @brief   :A asymmetric coroutine library for C++
# History
#      1. Date: 2014-12-12 
#          Author: chenxueyou
#          Modification: this file was created 
#

all:
	g++  uthread.cpp  -g -c
	g++  main.cpp -g -o main uthread.o
clean:
	rm -f uthread.o main
