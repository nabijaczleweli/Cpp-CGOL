# This file is licensed under the MIT License. For full text of the license see file named "LICENSE" in project's root directory.
# Designed for GNU Make 3.82.90 - works flawlessly there. For crashes / make-tion faults on other builds of GNU Make please contact the author.
# Copyright (c) 2014 nabijaczleweli
# Contributors: nabijaczleweli(nabijaczleweli@gmail.com)

ifeq ($(OS),Windows_NT)
  MV = move
  EXE = .exe
else
  MV = mv
  EXE =
endif

CPP = c++
CPPPAR = -s -Os -std=c++11 -Wall -Wextra -pipe


all : TGOL.o
	$(CPP) $(CPPPAR) TGOL.o -o../TGOL

clean :
	rm -f TGOL.o
	rm -f ../TGOL$(EXE)


TGOL.o : ../lib/armadillo/armadillo source/TGOL.cpp
	$(CPP) $(CPPPAR) -I../lib/armadillo source/TGOL.cpp -c -oTGOL.o