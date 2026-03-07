SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)

main:$(OBJ)
	gcc $^ -l sqlite3 -o $@

.Phony : clean
clean:
		@rm *.o
		@echo Clean All Objects

.Phony : print
print:
		@echo SRC=$(SRC)
		@echo OBJ=$(OBJ)

