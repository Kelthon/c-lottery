# c-lottery

## How to Build and Run

You can build and run using an IDE like Code::Blocks or using a compiler like gcc in the terminal. To compile using gcc first make sure gcc is already installed on your machine. Then copy and paste the commands below

### On Windows

```bash
gcc -c bet.c -o ./obj/bet.o
gcc -c strnum.c -o ./obj/strnum.o
gcc -fopenmp -c metrics.c -o ./obj/metrics.o
gcc -fopenmp -c compare.c -o ./obj/compare.o
gcc -fopenmp main.c -o ./bin/c-lottery.exe ./obj/bet.o ./obj/metrics.o ./obj/strnum.o ./obj/compare.o
./bin/c-lottery.exe
```

### On Linux

```bash
mkdir -p bin obj
gcc -c bet.c -o ./obj/bet.o
gcc -c strnum.c -o ./obj/strnum.o
gcc -fopenmp -c metrics.c -o ./obj/metrics.o
gcc -fopenmp -c compare.c -o ./obj/compare.o
gcc -fopenmp main.c -o ./bin/c-lottery ./obj/bet.o ./obj/metrics.o ./obj/strnum.o ./obj/compare.o -lm
./bin/c-lottery
```
