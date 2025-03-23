#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

char stack[MAX][MAX];  
int top = -1;

void inToPost();
void postToIn();
char *postToPre(char *);
void preToPost();
void inToPre();
void preToIn();

void pushStr(char *);
char *popStr();
char peek();
int isEmpty();
int precedence(char);
int space(char);
int isOperator(char);
int isOperand(char);
void reverse_string(char *);

int main (){
    int choice;
    char postfix[MAX], infix[MAX], prefix[MAX], result[MAX];
    
    printf("Pilih opsi:\n");
    printf("1. Infix ke Postfix\n");
    printf("2. Postfix ke Infix\n");
    printf("3. Postfix ke Prefix\n");
    printf("4. Prefix ke Postfix\n");
    printf("5. Infix ke Prefix\n");
    printf("6. Prefix ke Infix\n");
    printf("Pilihan: ");
    scanf("%d", &choice);
    getchar(); // Membersihkan buffer newline

    switch (choice){
    case 1:
        inToPost();
        break;
    case 2:
        postToIn();
        break;
    case 3: 
        printf("Masukkan ekspresi postfix: ");
        fgets(postfix, MAX, stdin);
        if (postfix[strlen(postfix) - 1] == '\n')
            postfix[strlen(postfix) - 1] = '\0';
        printf("Prefix = %s\n", postToPre(postfix));
        break;
    case 4:
        preToPost();
        break;
    case 5:
        inToPre();
        break;
    case 6:
        preToIn();
        break;
    default:
        printf ("Tidak valid");
    }

    return 0;
}

void inToPost() {
    char infix[MAX], postfix[MAX], symbol, temp[MAX];
    int i, j = 0;
    top = -1;
    
    printf("Masukkan ekspresi infix: ");
    fgets(infix, MAX, stdin);
    if (infix[strlen(infix) - 1] == '\n') 
        infix[strlen(infix) - 1] = '\0';

    for (i = 0; i < strlen(infix); i++) {
        symbol = infix[i];
        if (!space(symbol) && isOperand(symbol)) {
            postfix[j++] = symbol;
        } else if (!space(symbol) && isOperator(symbol)) {
            while (!isEmpty() && precedence(peek()) >= precedence(symbol)) {
                postfix[j++] = popStr()[0];
            }
            snprintf(temp, MAX, "%c", symbol);
            pushStr(temp);
        } else if (symbol == '(') {
            snprintf(temp, MAX, "%c", symbol);
            pushStr(temp);
        } else if (symbol == ')') {
            while (!isEmpty() && peek() != '(') {
                postfix[j++] = popStr()[0];
            }
            popStr();
        }
    }
    while (!isEmpty()) {
        postfix[j++] = popStr()[0];
    }
    postfix[j] = '\0';
    printf("Postfix = %s\n", postfix);
}

void postToIn() {
    char postfix[MAX], symbol, op1[MAX], op2[MAX], temp[MAX];
    int i;
    top = -1;
    
    printf("Masukkan ekspresi postfix: ");
    fgets(postfix, MAX, stdin);
    if (postfix[strlen(postfix) - 1] == '\n')
        postfix[strlen(postfix) - 1] = '\0';

    for (i = 0; i < strlen(postfix); i++) {
        symbol = postfix[i];
        if (symbol == ' ') continue;
        if (isOperand(symbol)) {
            char operandStr[2] = {symbol, '\0'};
            pushStr(operandStr);
        } else if (isOperator(symbol)) {
            if (top < 1) {
                printf("Error: Format postfix salah\n");
                return;
            }
            strcpy(op2, popStr());
            strcpy(op1, popStr());
            snprintf(temp, MAX, "(%s %c %s)", op1, symbol, op2);
            pushStr(temp);
        }
    }
    if (top == 0) {
        printf("Infix = %s\n", popStr());
    } else {
        printf("Error: Format postfix tidak valid\n");
    }
}

char *postToPre(char *post_exp) {
    static char result[MAX];
    char symbol, op1[MAX], op2[MAX], temp[MAX];
    int i;
    top = -1;
    
    for (i = 0; i < strlen(post_exp); i++) {
        symbol = post_exp[i];
        if (isOperand(symbol)) {
            char operand[2] = {symbol, '\0'};
            pushStr(operand);
        } else if (isOperator(symbol)) {
            if (top < 1) {
                printf("Error: Format postfix salah\n");
                exit(1);
            }
            strcpy(op1, popStr());
            strcpy(op2, popStr());
            snprintf(temp, MAX, "%c%s%s", symbol, op2, op1);
            pushStr(temp);
        }
    }
    if (top == 0) {
        strcpy(result, popStr());
        return result;
    } else {
        printf("Error: Format postfix tidak valid\n");
        exit(1);
    }
}

void preToPost() {
    char prefix[MAX], temp[MAX];
    printf("Enter Prefix expression: ");
    fgets(prefix, MAX, stdin);
    if (prefix[strlen(prefix) - 1] == '\n')
        prefix[strlen(prefix) - 1] = '\0';

    int len = strlen(prefix);

    for (int i = len - 1; i >= 0; i--) { 
        if (isOperator(prefix[i])) {  
            char op1[MAX], op2[MAX], newExpr[MAX];

            strcpy(op1, popStr());
            strcpy(op2, popStr());

            snprintf(newExpr, MAX, "%s %s %c", op1, op2, prefix[i]);
            pushStr(newExpr);
        } else {  
            snprintf(temp, MAX, "%c", prefix[i]);
            pushStr(temp);
        }
    }

    printf("Postfix = %s\n", popStr());
}

void pushStr(char *str) {
    if (top < MAX - 1) {
        top++;
        strcpy(stack[top], str);
    } else {
        printf("Stack overflow!\n");
    }
}

char *popStr() {
    if (top == -1) {
        printf("Stack underflow!\n");
        exit(1);
    }
    return stack[top--];
}

char peek() {
    if (!isEmpty()) {
        return stack[top][0];
    }
    return '\0';
}

int isEmpty() {
    return top == -1;
}

int space(char c) {
    return (c == ' ' || c == '\t');
}

int precedence(char symbol) {
    switch (symbol) {
        case '^': return 3;
        case '/':
        case '*': return 2;
        case '+':
        case '-': return 1;
        default: return 0;
    }
}

int isOperand(char symbol) {
    return ((symbol >= '0' && symbol <= '9') || (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'));
}

int isOperator(char symbol) {
    return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '^');
}



