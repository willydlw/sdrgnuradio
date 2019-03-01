objects = main.o checkdata.o 

all: echeck

echeck: $(objects) 
	gcc -o echeck main.o checkdata.o

main.o: main.c checkdata.h
	gcc -Wall -Wextra -c  main.c

checkdata.o: checkdata.c checkdata.h
	gcc -Wall -Wextra -c checkdata.c


# using a Phony target prevents make from getting confused
# by an actual file named clean
.PHONY : clean
clean:
	rm -f echeck $(objects)


