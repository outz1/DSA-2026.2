#include <stdio.h>

//F (n) = F (n −1) + F (n −2)
long long int fibonacci(long long int n) {
    if (n == 0 || n == 1) {
        return 1;
    }

    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(void) {
    int N;
    scanf("%d", &N);

    printf("%d\n", fibonacci(N));

    return 0;
}