#!/bin/bash

gcc -L/usr/lib -g -lm -lopenal -O2 -Wall audiotest.c -o audiotest
