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
    char postfix[MAX];
    
    printf ("+=========================================+\n");
    printf("|\t    Choose the Convertion:      \t|\n");
    printf("|=========================================|");
    printf("|           1. Infix to Postfix           |\n");
    printf("|           2. Postfix to Infix           |\n");
    printf("|           3. Postfix to Prefix          |\n");
    printf("|           4. Prefix to Postfix          |\n");
    printf("|           5. Infix to Prefix            |\n");
    printf("|           6. Prefix to Infix            |\n");
    printf("|           7. Quit                       |\n");
    printf ("+=========================================+\n");
    printf(">> ");
    
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

void inToPre() {
    char infix[MAX], reversedInfix[MAX], reversedPrefix[MAX], prefix[MAX];
    char temp[MAX];
    int i, j = 0;

    printf("Masukkan ekspresi infix: ");
    fgets(infix, MAX, stdin);
    if (infix[strlen(infix) - 1] == '\n')
        infix[strlen(infix) - 1] = '\0';

    // 1. Reverse infix expression
    reverse_string(infix);
    
    // 2. Tukar '(' dengan ')' dan sebaliknya
    for (i = 0; infix[i] != '\0'; i++) {
        if (infix[i] == '(') 
            reversedInfix[i] = ')';
        else if (infix[i] == ')') 
            reversedInfix[i] = '(';
        else 
            reversedInfix[i] = infix[i];
    }
    reversedInfix[i] = '\0';

    // 3. Konversi infix (dibalik) menjadi postfix
    for (i = 0; reversedInfix[i] != '\0'; i++) {
        char symbol = reversedInfix[i];

        if (isOperand(symbol)) {
            reversedPrefix[j++] = symbol;
        } else if (symbol == '(') {
            pushStr("(");
        } else if (symbol == ')') {
            while (!isEmpty() && peek() != '(') {
                reversedPrefix[j++] = popStr()[0];
            }
            popStr(); // Pop '('
        } else {
            while (!isEmpty() && precedence(peek()) >= precedence(symbol)) {
                reversedPrefix[j++] = popStr()[0];
            }
            snprintf(temp, MAX, "%c", symbol);
            pushStr(temp);
        }
    }

    while (!isEmpty()) {
        reversedPrefix[j++] = popStr()[0];
    }
    reversedPrefix[j] = '\0';

    // 4. Reverse hasil postfix -> jadi prefix
    reverse_string(reversedPrefix);
    strcpy(prefix, reversedPrefix);

    printf("Prefix = %s\n", prefix);
}

void preToIn() {
    char prefix[MAX], infix[MAX];
    char temp[MAX];
    int i;

    printf("Masukkan ekspresi prefix: ");
    fgets(prefix, MAX, stdin);
    prefix[strcspn(prefix, "\n")] = '\0';  // Menghapus newline di akhir input

    reverse_string(prefix);  // Balikkan ekspresi Prefix agar diproses dari belakang

    for (i = 0; prefix[i] != '\0'; i++) {
        char c = prefix[i];

        if (isOperand(c)) {
            char operand[2] = {c, '\0'};
            pushStr(operand);
        } else {
            if (top < 1) {
                printf("Error: Format prefix salah\n");
                return;
            }

            char op1[MAX], op2[MAX];
            strcpy(op1, popStr());
            strcpy(op2, popStr());

            snprintf(temp, MAX, "(%s %c %s)", op1, c, op2);
            pushStr(temp);
        }
    }

    if (top == 0) {
        strcpy(infix, popStr());
        printf("Infix = %s\n", infix);
    } else {
        printf("Error: Format prefix tidak valid\n");
    }
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

void reverse_string(char *str) {
    int length = strlen(str);
    int start = 0, end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}



