GCC_FLAGS=-Wall -g 
EXEC= main myGit

all: $(EXEC)

clean:
	rm -f *.o $(EXEC) *~

.c.o:   
	gcc $(GCC_FLAGS) -c $*.c

main: main.o projet.o liste.o dir.o work.o commit.o ref.o branch.o checkout.o merge.o
	gcc ${GCC_FLAGS} -o $@ $^

myGit: myGit.o projet.o liste.o dir.o work.o commit.o ref.o branch.o checkout.o merge.o
	gcc ${GCC_FLAGS} -o $@ $^