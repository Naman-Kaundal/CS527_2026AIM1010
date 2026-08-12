CC = gcc
CFLAGS = -Wall -std=c99

all: simulator

simulator: main.c compiler.c processor.c memory.c compiler.h processor.h memory.h
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c -o simulator

clean:
	rm -f simulator program.byte    


# To run data_1sum.byte:
# Copy-Item program_1sum.txt program.txt -Force
# Copy-Item data_1sum.byte data.byte -Force
# .\simulator.exe
# Copy-Item data.byte data_1sum.byte -Force


# To run data_2complex.byte:
# Copy-Item program_2complex.txt program.txt -Force
# Copy-Item data_2complex.byte data.byte -Force
# .\simulator.exe
# Copy-Item data.byte data_2complex.byte -Force


#To run data_3determinant.byte:
# Copy-Item program_3determinant.txt program.txt -Force
# Copy-Item data_3determinant.byte data.byte -Force
# .\simulator.exe
# Copy-Item data.byte data_3determinant.byte -Force