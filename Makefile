all: a b c 

a: kmeans_parallel_a.c
	gcc -fopenmp -g -Wall -o kmeans_parallel_a kmeans_parallel_a.c -lm

b: kmeans_parallel_b.c
	gcc -fopenmp -g -Wall -o kmeans_parallel_b kmeans_parallel_b.c -lm

c: kmeans_parallel_c.c
	gcc -fopenmp -g -Wall -o kmeans_parallel_c kmeans_parallel_c.c -lm

clean:
	$(RM) kmeans_parallel_a kmeans_parallel_b kmeans_parallel_c