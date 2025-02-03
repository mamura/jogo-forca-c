#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void clear()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

char* chooseWord(int theme)
{
    const char* themes[] = {
        "frutas.txt",
        "animais.txt"
    };
    FILE *file;
    char filePath[100];
    char *word = (char*) malloc(100 * sizeof(char)); // Aloca memória para a palavra
    char words[100][100]; // Array das palavras
    int numWords = 0;

    snprintf(filePath, sizeof(filePath), "./temas/%s", themes[theme - 1]);
    file = fopen(filePath, "r");

    if(file == NULL) {
        printf("Erro ao abrir o arquivo de palavras. \n");
        printf("%s", filePath);
        exit(1);
    }

    while( fgets(words[numWords], sizeof(words[numWords]), file) != NULL ) {
        words[numWords][strcspn(words[numWords], "\n")] = 0;
        numWords++;
    }

    fclose(file);

    // Selecionando uma palavra aleatória
    srand(time(NULL));
    int index = rand() % numWords;
    strcpy(word, words[index]);

    return word;
}

void drawHeader()
{
    printf("\n\n");
    printf("*******************************************\n");
    printf("*                   ___                   *\n");
    printf("*   JOGO DA FORCA  |[_]|   JOGO DA FORCA  *\n");
    printf("*                  |+ ;|                  *\n");
    printf("*                  '---'                  *\n");
    printf("*******************************************\n");
    printf("\n");
}

void drawForca(int errors, char* word)
{
    printf("#  _______       \n");
    printf("# |/      |      \n");
    printf("# |      %c%c%c  \n", (errors>=1?'(':' '), (errors>=1?'_':' '), (errors>=1?')':' '));
    printf("# |      %c%c%c  \n", (errors>=3?'\\':' '), (errors>=2?'|':' '), (errors>=3?'/': ' '));
    printf("# |       %c     \n", (errors>=2?'|':' '));
    printf("# |      %c %c   \n", (errors>=4?'/':' '), (errors>=4?'\\':' '));
    printf("# |              \n");
    printf("#_|___            %s  \n", word);
    printf("*******************************************\n");
    printf("\n\n");

}

void main()
{
    int attempts = 6;
    int theme;
    int wrongAttempts = 0;
    int gotItRight = 0;

    clear();
    drawHeader();
    printf("###### SELECIONE UM TEMA PARA O JOGO ######\n");
    printf("## [1] - FRUTAS                          ##\n");
    printf("## [2] - ANIMAIS                         ##\n");
    printf("###########################################\n");
    printf("\n");
    scanf("%d", &theme);

    char* secretWord = chooseWord(theme);
    char showSecret[strlen(secretWord) + 1];

    for(int i = 0; i < strlen(secretWord); i++) {
        showSecret[i] = '_';
    }
    showSecret[strlen(secretWord)] = '\0';

    while(wrongAttempts < attempts && !gotItRight) {
        char letter;
        getchar();
        
        printf("##           Tentativa %d de %d          ##\n", wrongAttempts, attempts);
        printf("# Escolha uma letra: ");
        scanf(" %c", &letter);

        int letterFound = 0;
        for (int i = 0; i < strlen(secretWord); i++) {
            if (secretWord[i] == letter) {
                showSecret[i] = letter;  // Revela a letra no lugar correto
                letterFound = 1;
            }
        }

        if (!letterFound) {
            wrongAttempts++;
        }
        
        gotItRight = 1;
        for (int i = 0; i < strlen(secretWord); i++) {
            if (showSecret[i] == '_') {
                gotItRight = 0; // Se ainda houver um '_', a palavra não foi completamente descoberta
                break;
            }
        }

        clear();
        drawHeader();
        drawForca(wrongAttempts, showSecret);
    }

    if (gotItRight) {
        printf("Parabéns, você acertou a palavra: %s\n", secretWord);
    } else {
        printf("Você perdeu! A palavra era: %s\n", secretWord);
    }

    free(secretWord);
}