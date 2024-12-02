all: 


	gcc -Wall -Wextra -o production Producer.c
	gcc -Wall -Wextra -o lecture Lect_Ecri.c
	gcc -Wall -Wextra -o philo philo.c

clean:
	rm -f philo production lecture

run: production
	./production 4 4
