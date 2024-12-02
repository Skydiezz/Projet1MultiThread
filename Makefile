all: 


	gcc -Wall -Wextra -o production Producer.c
	gcc -Wall -Wextra -o lecture Lect_Ecri.c
	gcc -Wall -Wextra -o philo philo.c
	chmod +x performances.sh

clean:
	rm -f philo production lecture

run: performances.sh
	./performances.sh
