#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MusicNode {
    char artist[100];
    char song[100];
    struct MusicNode* next;
    struct MusicNode* prev;
};

typedef struct MusicNode MusicNode;

MusicNode* createNode(char artist[], char song[]) {
    MusicNode* newNode = (MusicNode*)malloc(sizeof(MusicNode));
    if (newNode) {
        strcpy(newNode->artist, artist);
        strcpy(newNode->song, song);
        newNode->next = NULL;
        newNode->prev = NULL;
    }
    return newNode;
}

void insertAtEnd(MusicNode** head, char artist[], char song[]) {
    MusicNode* newNode = createNode(artist, song);
    if (!newNode) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    if (*head == NULL) {
        *head = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    } else {
        MusicNode* last = (*head)->prev;
        last->next = newNode;
        newNode->prev = last;
        newNode->next = *head;
        (*head)->prev = newNode;
    }
}

void displayPlaylist(MusicNode* head) {
    if (head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    MusicNode* current = head;
    printf("Playlist:\n");
    do {
        printf("Artista: %s, Música: %s\n", current->artist, current->song);
        current = current->next;
    } while (current != head);
}

void sortPlaylist(MusicNode** head) {
    if (*head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    MusicNode* current = *head;
    MusicNode* sortedList = NULL;
    do {
        MusicNode* next = current->next;
        insertAtEnd(&sortedList, current->artist, current->song);
        current = next;
    } while (current != *head);

    current = *head;
    do {
        MusicNode* next = current->next;
        free(current);
        current = next;
    } while (current != *head);

    *head = sortedList;
}

void removeSong(MusicNode** head, char artist[], char song[]) {
    if (*head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    MusicNode* current = *head;
    MusicNode* toDelete = NULL;

    do {
        if (strcmp(current->artist, artist) == 0 && strcmp(current->song, song) == 0) {
            toDelete = current;
            break;
        }
        current = current->next;
    } while (current != *head);

    if (toDelete == NULL) {
        printf("A música '%s' de '%s' não foi encontrada na playlist.\n", song, artist);
        return;
    }

    if (toDelete == *head) {
        *head = (*head)->next;
    }

    MusicNode* prev = toDelete->prev;
    MusicNode* next = toDelete->next;
    prev->next = next;
    next->prev = prev;

    if (toDelete == *head) {
        *head = next;
    }

    free(toDelete);
    printf("A música '%s' de '%s' foi removida da playlist.\n", song, artist);
}

void searchSong(MusicNode* head, char song[]) {
    if (head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }

    MusicNode* current = head;
    int found = 0;
    do {
        if (strcmp(current->song, song) == 0) {
            found = 1;
            printf("A música '%s' é de '%s'.\n", song, current->artist);
        }
        current = current->next;
    } while (current != head);

    if (!found) {
        printf("A música '%s' não foi encontrada na playlist.\n", song);
    }
}

int main() {
    MusicNode* playlist = NULL;
    char filename[] = "musicas.txt";
    char artist[100], song[100];

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Arquivo '%s' não encontrado. Criando uma playlist vazia.\n", filename);
    } else {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%99[^;];%99[^\n]", artist, song);
            insertAtEnd(&playlist, artist, song);
        }
        fclose(file);
        printf("Playlist carregada do arquivo '%s'.\n", filename);
    }

    MusicNode* currentSong = playlist;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Exibir playlist pela ordem de cadastro\n");
        printf("2. Exibir playlist ordenada pelo nome das músicas\n");
        printf("3. Inserir nova música\n");
        printf("4. Remover uma música\n");
        printf("5. Buscar por uma música\n");
        printf("6. Avançar para próxima música\n");
        printf("7. Retornar à música anterior\n");
        printf("8. Sair\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayPlaylist(playlist);
                break;
            case 2:
                sortPlaylist(&playlist);
                displayPlaylist(playlist);
                break;
            case 3:
                printf("Digite o nome do artista: ");
                scanf("%s", artist);
                printf("Digite o nome da música: ");
                scanf("%s", song);
                insertAtEnd(&playlist, artist, song);

                file = fopen(filename, "a");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo para escrita.\n");
                } else {
                    fprintf(file, "%s;%s\n", artist, song);
                    fclose(file);
                }
                break;
            case 4:
                printf("Digite o nome do artista da música a ser removida: ");
                scanf("%s", artist);
                printf("Digite o nome da música a ser removida: ");
                scanf("%s", song);
                removeSong(&playlist, artist, song);

                file = fopen(filename, "w");
                if (file == NULL) {
                    printf("Erro ao abrir o arquivo para escrita.\n");
                } else {
                    MusicNode* current = playlist;
                    do {
                        fprintf(file, "%s;%s\n", current->artist, current->song);
                        current = current->next;
                    } while (current != playlist);
                    fclose(file);
                }
                break;
            case 5:
                printf("Digite o nome da música que deseja buscar: ");
                scanf("%s", song);
                searchSong(playlist, song);
                break;
            case 6:
                if (currentSong) {
                    currentSong = currentSong->next;
                    printf("Música atual: %s - %s\n", currentSong->artist, currentSong->song);
                } else {
                    printf("A playlist está vazia.\n");
                }
                break;
            case 7:
                if (currentSong) {
                    currentSong = currentSong->prev;
                    printf("Música atual: %s - %s\n", currentSong->artist, currentSong->song);
                } else {
                    printf("A playlist está vazia.\n");
                }
                break;
            case 8:
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
