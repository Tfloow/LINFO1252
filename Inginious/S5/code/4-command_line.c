#include <stdio.h>
#include <string.h>

int main() {
    int x, y;
    char op;
    char buffer[100];
    while (1){
        printf("Entrez une operation arithmetique (ex. 2 + 3) ou stop pour quitter : ");
        int err = fscanf(stdin, "%d %c %d", &x, &op, &y);
        if(err != 3) {
            fscanf(stdin, "%s", buffer);
            if(strcmp(buffer, "stop") == 0){
                break;
            }
            printf("Entree invalide :O \n");
            continue;
        }

        switch (op) {
            case '+':
                printf("%d\n", x + y);
                break;
            case '-':
                printf("%d\n", x - y);
                break;
            case '*':
                printf("%d\n", x * y);
                break;
            case '/':
                printf("%d\n", x / y);
                break;
            default:
                printf("Invalid operator\n");
                break;
        }
    }


    return 0;
}
