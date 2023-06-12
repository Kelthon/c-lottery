# c-lottery

## How to Build and Run

You can build and run using an IDE like Code::Blocks or using a compiler like gcc in the terminal. To compile using gcc first make sure gcc is already installed on your machine. Then copy and paste the commands below

```bash
# Compile code
gcc -c bet.c -o .\obj\Debug\bet.o;
gcc -c strnum.c -o .\obj\Debug\strnum.o
gcc -fopenmp -c metrics.c -o .\obj\Debug\metrics.o
gcc -fopenmp -c compare.c -o .\obj\Debug\compare.o
gcc -fopenmp main.c -o .\bin\Debug\c-lottery.exe .\obj\Debug\bet.o .\obj\Debug\metrics.o .\obj\Debug\strnum.o .\obj\Debug\compare.o

# Run code 
.\bin\Debug\c-lottery.exe;
