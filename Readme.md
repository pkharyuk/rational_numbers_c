# C implementation of Rational Numbers.

## About 
Custom C implementation of rational numbers and basic operations with them. Demo program computes mean of an array of rational numbers. The program can handle input from a formatted text file or sample numbers randomly.

## Project structure

### Source code
- demo.c : main demo module
- build/ : directory for storing object files
- src/ : source code directory
    - custom_random.{h,c} : sampling random numbers
    - handle_clargs.{h,c} : handling command-line arguments
    - handle_input.{h,c} : handling input from a text file
    - process_string.{h,c} : some string operations
    - rational_number.{h,c} : implementation of rational numbers
- tests/ : directory for testing units
    - test_rational_number.c : unit tests for rational_numbers.{h,c}

### Sample input file
- sample_input.txt : example of an input text file; for storing $n$ numbers must contain ($n+1$) lines, where the first line is a single positive number ($n$), and the following $n$ lines are space/tab separated pairs of numbers ($\text{nominator}_i$, $\text{denominator}_i$), $i=\overline{1, n}$.

### Makefile

Contains rules and shortcuts for several operations:

- make compile : compile basic modules and the demo program
- make unit_tests : compile testing unit(s) and run it (them)
- make memcheck : check for possible memory leakages with valgrind
- make clean : remove all compilation results

### Dockerfile

Basic docker file for creating suitable image

### ```Scripts.sh```

Bash shortcuts to handle docker commands and make rules. Usage: ```bash ./scripts.sh <command>``` (e.g., help).
To use it without prefixing bash, add executing rights to your user with ```chmod +x ./scripts.sh```.