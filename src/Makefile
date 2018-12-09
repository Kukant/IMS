CC=c++
FLAGS = -std=gnu++11
LIBS= -lsimlib

all: farma

run: stats_script

stats: farma stats_script

test: farma test_script

farma:
		$(CC) $(FLAGS) main.cpp -o farma $(LIBS)
		chmod +x farma

stats_script:
		python3 get_statistics.py

test_script:
		./farma .99
		./farma .98
		./farma .97
		./farma .96
		./farma .95
		./farma .94
		./farma .93
		./farma .92
		./farma .8
		./farma .7
		./farma .6
		./farma .5
		./farma .4
		./farma .3
		./farma .2
		./farma .1
