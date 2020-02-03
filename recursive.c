#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define LINHA 0

float determinante(int ordem, float** matriz);
bool inversivel(int ordem, float** matriz);
float** matriz_cofatores(int ordem, float** matriz);
float** matriz_transposta(int ordem, float** matriz);
float** matriz_inversa(int ordem, float** matriz);
float** multiplica_escalar(int ordem, float** matriz, float escalar);
float* multiplica_coluna(int ordem, float** matriz, float* coluna);
float* solucionar_sistema(int ordem, float** matriz, float* coluna);

float** alocar_matriz(int ordem);
void desalocar_matriz(int ordem, float** matriz);
void imprimir_matriz(int ordem, float** matriz);

int main(){
    int i, j, num;
    float  *independente = NULL, *solucao = NULL;
    float **matriz = NULL, **cofatores = NULL, **transposta = NULL, **inversa = NULL;

    printf("Insira o tamanho da matriz quadrada: ");
    scanf("%d", &num);

    matriz = alocar_matriz(num);

    printf("\nInsira os elementos da matriz\n");

    for (i = 0; i < num; i++)
        for (j = 0; j < num; j++)
            scanf("%f", &matriz[i][j]);
    
    printf("\nMatriz transposta:\n");
    transposta = matriz_transposta(num, matriz);
    imprimir_matriz(num, transposta);

    //printf("\nTomando sempre a %d linha\npara calcular o cofator.\n", LINHA+1);
    printf("O determinante é %.2f\n", determinante(num, matriz));
    printf("Portanto, a matriz ");

    if (!inversivel(num, matriz))
        printf("NAO ");
    printf("eh inversivel\n");
    
    cofatores = matriz_cofatores(num, matriz);
    printf("\nMatriz dos cofatores:\n");
    imprimir_matriz(num, cofatores);

    inversa = matriz_inversa(num, matriz);
    printf("\nMatriz inversa:\n");
    imprimir_matriz(num, inversa);


    independente = (float*) calloc(num, sizeof(float)); //alocacao do vetor coluna
    printf("Ler os termos independentes:\n");
    for (i = 0; i < num; i++)
        scanf("%f", &independente[i]);
    
    solucao = solucionar_sistema(num, matriz, independente); //multiplica_coluna(num, inversa, independente);
    printf("\nSolucao:\n");
    
    for (i = 0; i < num; i++)
        printf("%8.2f\n", solucao[i]);

    if (num != 1)
        desalocar_matriz(num, cofatores);

    desalocar_matriz(num, transposta);
    desalocar_matriz(num, matriz);
    desalocar_matriz(num, inversa);
    if (independente != NULL)
        free(independente);
    
    if (solucao != NULL)
        free(solucao);

    return 0;
}

float** alocar_matriz(int ordem){
    int i;
    float **nova = NULL;

    nova = (float**) calloc(ordem, sizeof(float*));
    for (i = 0; i < ordem; i++)
        nova[i] = (float*) malloc(ordem*sizeof(float));

    return nova;
}

void desalocar_matriz(int ordem, float** matriz){
    int i;

    if (matriz == NULL)
        return;

    for (i = 0; i < ordem; i++)
        free(matriz[i]);
    free(matriz);
}

float determinante(int ordem, float** matriz){
    int i, j, k, a, b;
    float ***cofatores = NULL, resultado;

    if (ordem == 1) //Se só tem um elemento retorna eles
        return matriz[0][0];

    if (ordem == 2) //Só pra aumentar a velocidade -> não-obrigatório
        return matriz[0][0]*matriz[1][1] - matriz[0][1]*matriz[1][0];

    if (ordem == 3){ //Só pra aumentar a velocidade -> não-obrigatório
        return
         matriz[0][0]*matriz[1][1]*matriz[2][2]
        +matriz[0][1]*matriz[1][2]*matriz[2][0]
        +matriz[0][2]*matriz[1][0]*matriz[2][1]
        -matriz[0][2]*matriz[1][1]*matriz[2][0]
        -matriz[0][0]*matriz[1][2]*matriz[2][1]
        -matriz[0][1]*matriz[1][0]*matriz[2][2];
    }


    /* Alocacao dos cofatores*/
    cofatores = (float***) malloc(ordem*sizeof(float**));
    for (i = 0; i < ordem; i++)
        cofatores[i] = alocar_matriz(ordem-1);
    /* Alocacao dos cofatores*/

    for (k = 0; k < ordem; k++){
        a = 0;
        for (i = 0; i < ordem; i++){
            b = 0;

            if (i != LINHA){
                for (j = 0; j < ordem; j++){
                    if (j != k){
                        cofatores[k] [a][b] = matriz[i][j];
                        b++;
                    }
                }
                a++;
            }
        }
    }

    resultado = 0;
    for (i = 0; i < ordem; i++){
        if  ((i+LINHA)%2)
            resultado -= matriz[LINHA][i]*determinante(ordem-1, cofatores[i]);
        else
            resultado += matriz[LINHA][i]*determinante(ordem-1, cofatores[i]);
    }


     /* Liberacao dos cofatores*/
    for (i = 0; i < ordem; i++){
        for (j = 0; j < ordem-1; j++)
            free(cofatores[i][j]);

        free(cofatores[i]);
    }

    free(cofatores);
    /* Liberacao dos cofatores*/

    return resultado;
}

bool inversivel(int ordem, float** matriz){
    if (!determinante(ordem, matriz))
        return false;
    else
        return true;
}

float** matriz_cofatores(int ordem, float** matriz){
    int i, j, k, l, a, b;
    float **nova = NULL, **temp = NULL;


    if (!inversivel(ordem, matriz))
        exit(EXIT_FAILURE);

    if (ordem == 1)
        return matriz;

    nova = alocar_matriz(ordem);
    temp = alocar_matriz(ordem-1);

    for (i = 0; i < ordem; i++){
        for (j = 0; j < ordem; j++){

            a = 0;
            for (k = 0; k < ordem; k++){
                if (k == i)
                    continue;
                b = 0;
                for (l = 0; l < ordem; l++){
                    if (l == j)
                        continue;

                    temp[a][b] = matriz[k][l];
                    b++;
                }
                a++;
            }

            nova[i][j] = determinante(ordem-1, temp);

            if ((i+j)%2)
                nova[i][j] *= -1;
        }
    }
     
    desalocar_matriz(ordem-1, temp);

    return nova;
}

float** matriz_transposta(int ordem, float** matriz){
    int i, j;
    float **transposta = NULL;

    if (matriz == NULL)
        return NULL;

    transposta = alocar_matriz(ordem);

    for (i = 0; i < ordem; i++)
        for (j = 0; j < ordem; j++)
            transposta[j][i] = matriz[i][j]; 

    return transposta;
}

float* multiplica_coluna(int ordem, float** matriz, float* coluna){
    int i, j, k;
    float *produto = NULL;

    if (matriz == NULL || coluna == NULL)
        return NULL;
    
    produto = (float*) calloc(ordem, sizeof(float)); 
    //alocacao do vetor coluna


    for (i = 0; i < ordem; i++)
        for (j = 0; j < ordem; j++)
            produto[i] += matriz[i][j]*coluna[j];

    return produto;
}

float** multiplica_escalar(int ordem, float** matriz, float escalar){
    int i, j;
    
    //Preferi não alterar
    
    for (i = 0; i < ordem; i++)
        for (j = 0; j < ordem; j++)
            matriz[i][j] *= escalar;
            
    return matriz;
        
}

float** matriz_inversa(int ordem, float** matriz){
    float **adjunta = NULL;

    if (matriz == NULL)
        return NULL;

    if (!inversivel(ordem, matriz))
        return NULL;
        
    adjunta = matriz_transposta(ordem, matriz_cofatores(ordem, matriz));
    
    if (adjunta == NULL)
        return NULL;
        
    return multiplica_escalar(ordem, adjunta, 1/determinante(ordem, matriz));
    
}

float* solucionar_sistema(int ordem, float** matriz, float* coluna){
    return multiplica_coluna(ordem, matriz_inversa(ordem, matriz), coluna);
}

void imprimir_matriz(int ordem, float** matriz){
   int i, j;

    if (matriz == NULL)
        return;
    
    for (i = 0; i < ordem; i++){
        for (j = 0; j < ordem; j++)
            printf("%10.2f ", matriz[i][j]);
         printf("\n");
    }

    printf("\n");
}
