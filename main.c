#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_OPTIONS 10

struct Question {
    char Text[MAX_LINE_LENGTH];
    char Answer[MAX_LINE_LENGTH];
    char Options[MAX_OPTIONS][MAX_LINE_LENGTH];
    int OptionsCount;
};

void shuffleOptions(char options[MAX_OPTIONS][MAX_LINE_LENGTH], int count) {
    int i, j;
    char temp[MAX_LINE_LENGTH];

    for (i = count - 1; i > 0; i--) {
        j = rand() % (i + 1);
        strcpy(temp, options[i]);
        strcpy(options[i], options[j]);
        strcpy(options[j], temp);
    }
}

void shuffleQuestions(struct Question questions[], int count) {
    int i, j;
    struct Question temp;

    for (i = count - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

void readInputFile(const char* filePath, struct Question questions[], int* count) {
    FILE* file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int questionIndex = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == 'Q' && strchr(line, ':') != NULL) {
            strcpy(questions[questionIndex].Text, line);
            fgets(questions[questionIndex].Answer, sizeof(questions[questionIndex].Answer), file);

            int optionIndex = 0;
            while (fgets(questions[questionIndex].Options[optionIndex], sizeof(line), file) != NULL &&
                   questions[questionIndex].Options[optionIndex][0] != '\n') {
                questions[questionIndex].Options[optionIndex][strlen(questions[questionIndex].Options[optionIndex]) - 1] = '\0'; 
                optionIndex++;

                if (optionIndex >= MAX_OPTIONS) {
                    fprintf(stderr, "Too many options for a question\n");
                    exit(EXIT_FAILURE);
                }
            }

            questions[questionIndex].OptionsCount = optionIndex;
            questionIndex++;
        } else if (line[0] != '\n') {
            fprintf(stderr, "Invalid input line: %s", line);
        }
    }

    *count = questionIndex;
    fclose(file);
}

void printQuestionsToConsole(const struct Question questions[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s", questions[i].Text);

        printf("Options: ");
        for (int j = 0; j < questions[i].OptionsCount; j++) {
            printf("%s ", questions[i].Options[j]);
        }

        printf("\n\n");
    }
}

void printAnswerSheetToConsole(const struct Question questions[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s", questions[i].Text);
        printf("Correct Answer: %s\n\n", questions[i].Answer);
    }
}

void generateBooklets(struct Question questions[], int count, int numBooklets, int questionsPerBooklet) {
    shuffleQuestions(questions, count);

    for (int i = 0; i < numBooklets; i++) {
        printf("\nBooklet %d - Questions:\n", i + 1);

        for (int j = 0; j < questionsPerBooklet; j++) {
            shuffleOptions(questions[i].Options, questions[i].OptionsCount);
            printf("%sOptions: ", questions[i].Text);
            for (int k = 0; k < questions[i].OptionsCount; k++) {
                printf("%s ", questions[i].Options[k]);
            }
            printf("\n\n");
        }

        printf("\nBooklet %d - Answer Sheet:\n", i + 1);
        printAnswerSheetToConsole(questions, questionsPerBooklet);
    }
}

int main() {
    const char* inputFile = "C:\\Users\\user\\OneDrive\\Masaüstü\\booklet\\sample.txt";
    int numBooklets = 3;
    int questionsPerBooklet = 5;

    struct Question questions[MAX_OPTIONS];
    int questionCount;

    readInputFile(inputFile, questions, &questionCount);

    printf("Total questions in the input file: %d\n", questionCount);

    if (questionCount < numBooklets * questionsPerBooklet) {
        printf("Not enough questions in the input file.\n");
        printf("Total questions required: %d\n", numBooklets * questionsPerBooklet);
        return EXIT_FAILURE;
    }

    generateBooklets(questions, questionCount, numBooklets, questionsPerBooklet);

    return EXIT_SUCCESS;
}
