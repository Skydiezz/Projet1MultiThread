all: 


	gcc -Wall -Wextra -o production Producer.c
	gcc -Wall -Wextra -o lecture Lect_Ecri.c
	gcc -Wall -Wextra -o philo philo.c
	gcc -Wall -Wextra -o test-and-set test-and-set.c
	gcc -Wall -Wextra -o test-and-test-and-set test-and-test-and-set.c
	chmod +x experiments.sh

clean:
	rm -f philo production lecture test-and-set test-and-test-and-set

run: experiments.sh
	./experiments.sh
