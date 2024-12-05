all: 


	gcc -Wall -Wextra -o production Producer.c
	gcc -Wall -Wextra -o lecture Lect_Ecri.c
	gcc -Wall -Wextra -o philo philo.c
	gcc -Wall -Wextra -o testi testi.c
	chmod +x experiments.sh

clean:
	rm -f philo production lecture testi

run: experiments.sh
	./experiments.sh
