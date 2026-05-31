build: main.c
	gcc -Wall -Wextra -Werror main.c lib/* -o fcsh

run: main.c
	gcc -Wall -Wextra -Werror main.c lib/* -o fcsh
	./fcsh

clean:
	rm fcsh
