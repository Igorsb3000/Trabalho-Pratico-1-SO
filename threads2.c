#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

pthread_t *threads;


void* multiplicando_matriz(){
	
}

int main(int argc, char *argv[]){	

	// Alocar array para minha thread
    int quantidade_threads = atoi(argv[3]);
	printf("quantidade de threads: %d\n", quantidade_threads);

	threads = malloc(quantidade_threads * sizeof(int));
	printf("threads: %ln\n", threads);
	
	for (int variavel = 0; variavel < quantidade_threads; variavel++)
	{
		threads[variavel] = 10;
		printf("for loop: threads[%d]: %ln\n", variavel, threads);
	}

	printf("threads: %ln\n", threads);
	// calculate size in bytes
	int arraySize = sizeof(threads);
	int intSize = sizeof(threads[0]);

	// length
	int length = arraySize / intSize;

	printf("Length of array = %d \n", length);
	

	sleep(10);

	int n1, m1, n2, m2, lin_m, col_m, i, j, k;
	FILE *file1, *file2, *file3;

	// Verificando se os nomes dos 2 arquivos foram passados na linha de comando
	if(argc < 3){
		printf("Informe os nomes dos 2 arquivos que contém as matrizes m1 e m2!\n");
		return 1;
	}


	/* Lendo Matriz 1 */

	// Abrindo os arquivos para leitura das matrizes
	file1 = fopen(argv[1], "r");
	if(file1 == NULL){
		printf("Arquivo %s não encontrado!\n", argv[1]);
		return 0;
	}
	
	// Obtendo as dimensoes da matriz
	fscanf(file1, "%d;%d;", &n1, &m1);
	int matriz_1[n1][m1];

	for(i=0; i<n1; i++){
		for(j=0; j<m1; j++){
			fscanf(file1, "%d;", &matriz_1[i][j]);
		}
	}
	// Fechando o arquivo
	fclose(file1); 


	printf("\nMatriz 1:\n\n");
	for(i=0; i<n1; i++){
		for(j=0; j<m1; j++){
			printf("%d;", matriz_1[i][j]);
		}
		printf("\n");
	}


	/* Lendo Matriz 2 */

	// Abrindo os arquivos para leitura das matrizes
	file2 = fopen(argv[2], "r");
	if(file2 == NULL){
		printf("Arquivo %s não encontrado!\n", argv[1]);
		return 0;
	}
	
	// Obtendo as dimensoes da matriz
	fscanf(file2, "%d;%d;", &n2, &m2);
	int matriz_2[n2][m2];

	for(i=0; i<n2; i++){
		for(j=0; j<m2; j++){
			fscanf(file2, "%d;", &matriz_2[i][j]);
		}
	}
	// Fechando o arquivo
	fclose(file2); 


	printf("\nMatriz 2:\n\n");
	for(i=0; i<n2; i++){
		for(j=0; j<m2; j++){
			printf("%d;", matriz_2[i][j]);
		}
		printf("\n");
	}

	// Verificando se a multiplicacao das matriz m1 e m2 eh possivel
	if(n1 != m2){
		printf("\nNão é possivel multiplicar as matrizes m1 e m2!\n");
		return 1;
	}


	/* Multiplicando as Matrizes - THREADS*/ 

	//Defining Threads
    int status;
    void *thread_return;

    lin_m = n1; // Quantidade de linhas da matriz m1
	col_m = m2; // Quantidade de colunas da matriz m2

	/* 

	Professor sobre o trabalho da primeira unidade, tava fazendo aqui a parte das threads e eu passo os dois arquivos na linha de comando e o valor de P.
	Só que o tamanho da minha matriz_resultado eu só consigo saber in run time, como que danado eu vou instanciar o tamanho do meu array da matriz_resultado se eu só consigo saber in run time?
	tentei fazer usando maloc, fiz alguns testes, vou deixar código abaixo, mas não deu certo...


	Para cada P elementos da matriz resultado uma thread deve ser criada. 

	2x2 = 4 elementos. -> P = 2 => 4/2 = 2 threads
	10x10 = 100 elementos. -> P = 2 => 100/2 = 50 threads

	P => quantidade de vezes que vai executar a função dentro da Thread
	*/

	int matriz_resultado[lin_m][col_m];

	printf("\nMontando matriz resultado...\n");
	clock_t begin = clock();

    for(i=0; i<lin_m; i++){
		for(j=0; j<col_m; j++){
			matriz_resultado[i][j] = 0;
			for(k=0; k<m1; k++){ // Não entendi isso aqui
				matriz_resultado[i][j] += matriz_1[i][k] * matriz_2[k][j];
			}
		}
	}
    
	clock_t end = clock();
	double time_spent = (double)(end - begin) / (CLOCKS_PER_SEC/1000);
	printf("\nTempo gasto para execução foi de %f ms\n", time_spent);

    // for(i=0; i<quantidade_threads; i++){
        
        
    // }


    
    

	return 0;
}