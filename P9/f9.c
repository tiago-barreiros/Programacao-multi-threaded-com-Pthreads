#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include "semaphore.h"

volatile int bilDisp = 45522;
volatile int Vendas[10] = {0};

int semente;
int metodo;

pthread_mutex_t lock;

sem_t semafero;

void *vendaBilhetes(void *arg) {
    int b = 0;
    int i = (int ) arg;
    srand(semente + i);
    while (Vendas[i] < 1500) {
        b = 1 + rand() % 4;
        if (metodo == 1) {
            pthread_mutex_lock(&lock);
        }
        if (metodo == 2) {
            sem_wait(&semafero);
        }

        //A secção critica
        bilDisp = bilDisp - b;
        Vendas[i] = Vendas[i] + b;

        if (metodo == 1) {
            pthread_mutex_unlock(&lock);
        }
        if (metodo == 2) {
            sem_post(&semafero);
        }
        printf("A bilheteira %d conseguiu vender %d bilhetes \n", (int *) arg, b);
    }
}

int main(int argc, char *argv[]) {

    semente = atoi(argv[1]);
    metodo = atoi(argv[2]);
    pthread_t threadsList[10];
    pthread_mutex_init(&lock, NULL);
    sem_init(&semafero, 0, 1);
    printf("::: Bilheteira do Estádio de Anfield ::: \n");

    for (int i = 0; i < 10; ++i){
        pthread_create(&threadsList[i], NULL, vendaBilhetes, (void *) i);
    }
    for (int i = 0; i < 10; ++i) {

        pthread_join(threadsList[i], NULL);
    }

    int totalVendas = 0;
    for (int i = 0; i < 10; ++i) {
        totalVendas += Vendas[i];
    }

    int capacidade = bilDisp + totalVendas;
    printf("Número de total de vendas efetuadas: %d\n", totalVendas);
    printf("Capacidade Calculada %d (45522?)\n", capacidade);

    //pthread_mutex_destroy(&lock);
    //sem_destroy(&semafero);
    return 0;
}