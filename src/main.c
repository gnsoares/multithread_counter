#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ENTRIES 1000
#define MAX_THREADS 4


// variavel da saida
int result = 0;

// lock das threads
pthread_mutex_t result_lock;


/**
 * Funcao verificadora de numeros primos
 * @param n numero a ser verificado
 * @return 0 se nao for primo, 1 caso contrario
 */
int is_prime(int64_t n) {

    // o menor primo e 2
    if (n < 2) return 0;

    // caso tenha divisor != 1 e != n: nao e primo
    for (unsigned long i = 2; i <= sqrtl(n); i++)
        if (n % i == 0)
            return 0;

    // caso contrario: e primo
    return 1;
}


/**
 * Funcao concorrente de verificacao de numeros primos
 * @param args endereco de um inteiro a ser verificado
 */
void* co_is_prime(void* args) {

    // computacao do resultado para essa entrada
    int buffer = is_prime(*((int*) args));

    // aumento do contador da saída
    pthread_mutex_lock(&result_lock);
    result += buffer;
    pthread_mutex_unlock(&result_lock);

    return NULL;
}


int main() {

    // variaveis de controle
    int n_entries = 0;
    int n_threads = 0;
    int counter = 0;

    // buffer da entrada
    unsigned long entry;
    unsigned long entries[MAX_ENTRIES];

    // threads
    pthread_t threads[MAX_THREADS];


    // leitura da entrada
    while (scanf("%lu", &entry) > 0)
        entries[n_entries++] = entry;


    // processamento da entrada
    while (counter < n_entries) {

        // numero de threads = min(n_entries - counter, MAX_THREADS)
        n_threads = n_entries - counter > MAX_THREADS ? MAX_THREADS : n_entries - counter;

        // cada thread deve processar uma entrada
        for (int i = 0; i < n_threads; i++)
            pthread_create(threads + i, NULL, co_is_prime, entries + counter++);

        // espera das threads para continuar
        for (int i = 0; i < n_threads; i++)
            pthread_join(threads[i], NULL);

    }


    // impressao da saida
    printf("%d\n", result);

    return 0;
}
