all: 


	gcc -Wall -Wextra -o production Producer.c
	gcc -Wall -Wextra -o lecture Lect_Ecri.c
	gcc -Wall -Wextra -o philo philo.c
	gcc -Wall -Wextra -o test-and-set test-and-set.c
	gcc -Wall -Wextra -o test-and-test-and-set test-and-test-and-set.c
	gcc -Wall -Wextra -o philoV2 philoV2.c semaphore_struct.c -pthread
	gcc -Wall -Wextra -o productionV2 ProducerV2.c semaphore_struct.c -pthread
	gcc -Wall -Wextra -o lectureV2 Lect_EcriV2.c semaphore_struct.c -pthread
	chmod +x experiments.sh

clean:
	rm -f philo production lecture test-and-set test-and-test-and-set philoV2 productionV2 lectureV2

test: experiments.sh
	./experiments.sh
