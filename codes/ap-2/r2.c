#include <stdio.h>

long long int mdc(long long int A, long long int B) {
    if (B == 0) {
        return A;
    }
    return mdc(B, A % B);
}

int main() {
  long long int A, B;
  scanf("%lld %lld", &A, &B);
  printf("%lld\n", mdc(A, B));
  return 0;
}