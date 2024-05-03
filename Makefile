build:
	ipcrm -a
	gcc process_generator.c -o process_generator.out
	gcc clk.c -o clk.out
	gcc scheduler.c -o scheduler.out -lm
	gcc process.c -o process.out
	gcc test_generator.c -o test_generator.out
	cc GUI.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f *.out  processes.txt

all: clean build

run:
	./a.out
