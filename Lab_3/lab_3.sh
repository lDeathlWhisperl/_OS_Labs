#!/bin/bash
g++ -c lab_3.1.cpp
g++ -c lab_3.2.cpp
g++ -o lab_3.1 lab_3.1.o -lpthread
g++ -o lab_3.2 lab_3.2.o -lpthread
./lab_3.2
