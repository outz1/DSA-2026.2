#include <stdio.h>

long long calcularMenorNumeroDeMovimentos(int numeroDeDiscos)
{
    if(numeroDeDiscos == 1)
    {
        return 1;   
    }

    return 2 * calcularMenorNumeroDeMovimentos(numeroDeDiscos - 1) + 1;
}

int main()
{
    int numeroDeDiscos;

    scanf("%d", &numeroDeDiscos);

    printf("%lld\n", calcularMenorNumeroDeMovimentos(numeroDeDiscos));
}
