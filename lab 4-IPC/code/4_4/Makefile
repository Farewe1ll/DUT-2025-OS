hdrs = ipc.h
c_src = smoker1.c smoker2.c smoker3.c supplier.c
c_obj = smoker1.o smoker2.o smoker3.o supplier.o

all: smoker1 smoker2 smoker3 supplier

smoker1: smoker1.o ipc.o
	gcc smoker1.o ipc.o -o smoker1

smoker2: smoker2.o ipc.o
	gcc smoker2.o ipc.o -o smoker2

smoker3: smoker3.o ipc.o
	gcc smoker3.o ipc.o -o smoker3

supplier: supplier.o ipc.o
	gcc supplier.o ipc.o -o supplier

clean:
	rm -f *.o smoker1 smoker2 smoker3 supplier