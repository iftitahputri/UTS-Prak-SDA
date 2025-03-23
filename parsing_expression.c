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



