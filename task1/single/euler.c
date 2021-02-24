#include <stdio.h>
#include <stdlib.h>

double factorial(int n) {
    double fact = 1;
    if (n < 0) {
        printf("Error! Factorial of a negative number doesn't exist.");
        return -1;
    }
    else
        for (int i = 1; i <= n; ++i) {
            fact *= i;
        }
    return fact;
}

int main(int argc, char** argv) {
  int depth = 20;
  if (argc > 1) {
    depth = atoi(argv[1]);
  }

  double e = 0.0;
  for (int i = 0; i < depth; ++i) {
      e += 1 / factorial(i);
  }

  printf("calculated e: %g", e);
  return 0;
}
