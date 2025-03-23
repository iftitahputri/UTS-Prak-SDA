#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

char stack[MAX][MAX];  
int top = -1;

void pushStr(char *);
char *popStr();
char peek();
int isEmpty();
int precedence(char);
int space(char);
int isOperator(char);
int isOperand(char);

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



