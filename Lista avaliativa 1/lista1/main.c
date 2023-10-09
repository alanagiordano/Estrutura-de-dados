#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[50];
    char genero[50];
    char local[50];
    int albuns;
} Artista;

int comparar(const void *a, const void *b) {
    return strcmp(((Artista *)a)->nome, ((Artista *)b)->nome);
}

int ler(Artista **lista) {
    FILE *arquivo;
    arquivo = fopen("artistas.txt", "r");
    
    int numArtista;
    fscanf(arquivo, "%d\n", &numArtista);

    *lista = (Artista *)malloc(numArtista * sizeof(Artista));

    for (int i = 0; i < numArtista; i++) {
        fscanf(arquivo, "%s %s %s %d\n", (*lista)[i].nome, (*lista)[i].genero, (*lista)[i].local, &(*lista)[i].albuns);
    }

    qsort(*lista, numArtista, sizeof(Artista), comparar);

    fclose(arquivo);
    return numArtista;
}

void escrever(Artista *lista, int numArtista) {
    FILE *arquivo;
    arquivo = fopen("artistas.txt", "w");

    fprintf(arquivo, "%d\n", numArtista);

    for (int i = 0; i < numArtista; i++) {
        fprintf(arquivo, "%s %s %s %d\n", lista[i].nome, lista[i].genero, lista[i].local, lista[i].albuns);
    }

    fclose(arquivo);
}

void inserir(Artista **lista, int *numArtista, Artista novoArtista) {
    *numArtista += 1;
    *lista = (Artista *)realloc(*lista, (*numArtista) * sizeof(Artista));
    int posicao = 0;
    while (posicao < *numArtista - 1 && strcmp(novoArtista.nome, (*lista)[posicao].nome) > 0) {
        posicao++;
    }

    for (int i = *numArtista - 1; i > posicao; i--) {
        (*lista)[i] = (*lista)[i - 1];
    }

    (*lista)[posicao] = novoArtista;

    qsort(*lista, *numArtista, sizeof(Artista), comparar);
}

void remover(Artista **lista, int *numArtista, char nome[]) {
    int posicao = -1;

    for (int i = 0; i < *numArtista; i++) {
        if (strcmp(nome, (*lista)[i].nome) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao != -1) {
        for (int i = posicao; i < *numArtista - 1; i++) {
            (*lista)[i] = (*lista)[i + 1];
        }
        *numArtista -= 1;
        *lista = (Artista *)realloc(*lista, (*numArtista) * sizeof(Artista));
        qsort(*lista, *numArtista, sizeof(Artista), comparar);
    } else {
        printf("Artista com o nome %s não encontrado.\n", nome);
    }
}

void editar(Artista *lista, int numArtista, char nome[]) {
    int posicao = -1;

    for (int i = 0; i < numArtista; i++) {
        if (strcmp(nome, lista[i].nome) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao != -1) {
        printf("Digite o novo nome: ");
        scanf("%s", lista[posicao].nome);
        printf("Gênero musical: ");
        scanf("%s", lista[posicao].genero);
        printf("local de nascimento: ");
        scanf("%s", lista[posicao].local);
        printf("álbuns: ");
        scanf("%d", &lista[posicao].albuns);
        qsort(lista, numArtista, sizeof(Artista), comparar);
    } else {
        printf("Artista não encontrado.\n");
    }
}

int ArtistaBinario(Artista *lista, int numArtista, char nome[]) {
    int inicio = 0;
    int fim = numArtista - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int comparacao = strcmp(nome, lista[meio].nome);

        if (comparacao == 0) {
            return meio; 
        } else if (comparacao < 0) {
            fim = meio - 1; 
        } else {
            inicio = meio + 1; 
        }
    }

    return -1; 
}

int AlbumSequencial(Artista *lista, int numArtista, int albuns) {
    for (int i = 0; i < numArtista; i++) {
        if (lista[i].albuns == albuns) {
            return i; 
        }
    }

    return -1;
}

int main() {
    Artista *artistas = NULL;
    int numArtista = ler(&artistas);

    if (numArtista == 0) {
        return 1; 
    }

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Inserção ordenada de um novos artistas\n");
        printf("2. Remoção de um artista\n");
        printf("3. Edição de um artista\n");
        printf("4. Busca binária por um artista\n");
        printf("5. Busca sequencial por um álbum\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Artista novoArtista;
                printf("Digite o nome: ");
                scanf("%s", novoArtista.nome);
                printf("Gênero musical: ");
                scanf("%s", novoArtista.genero);
                printf("Local de nascimento: ");
                scanf("%s", novoArtista.local);
                printf("álbuns: ");
                scanf("%d", &novoArtista.albuns);
                inserir(&artistas, &numArtista, novoArtista);
                escrever(artistas, numArtista);
                break;
            }
            case 2: {
                char nome[50];
                printf("Digite o nome: ");
                scanf("%s", nome);
                remover(&artistas, &numArtista, nome);
                escrever(artistas, numArtista);
                break;
            }
            case 3: {
                char nome[50];
                printf("Digite o nome: ");
                scanf("%s", nome);
                editar(artistas, numArtista, nome);
                escrever(artistas, numArtista);
                break;
            }
            case 4: {
                char nome[50];
                printf("Digite o nome: ");
                scanf("%s", nome);
                int posicao = ArtistaBinario(artistas, numArtista, nome);
                if (posicao != -1) {
                    printf("Artista encontrado:\n");
                    printf("Nome: %s\n", artistas[posicao].nome);
                    printf("Gênero: %s\n", artistas[posicao].genero);
                    printf("Nascimento: %s\n", artistas[posicao].local);
                    printf("álbuns: %d\n", artistas[posicao].albuns);
                } 
                break;
            }
            case 5: {
                int albuns;
                printf("Digite o número de álbuns a ser buscado: ");
                scanf("%d", &albuns);
                int posicao = AlbumSequencial(artistas, numArtista, albuns);
                if (posicao != -1) {
                    printf("Artista encontrado com %d álbuns:\n", albuns);
                    printf("Nome: %s\n", artistas[posicao].nome);
                    printf("Gênero: %s\n", artistas[posicao].genero);
                    printf("Nascimento: %s\n", artistas[posicao].local);
                    printf("álbuns: %d\n", artistas[posicao].albuns);
                } 
                break;
            }
            case 6:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 6);

    return 0;
}
