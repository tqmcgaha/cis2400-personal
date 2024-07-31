#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double power(double x, int power);

double fact(int x);

double pi_approx(int x);
double pi_approx_fast(int x);

int main(int argc, char* argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: ./pi_approx <integer>\n");
    return EXIT_FAILURE;
  }

  int iters = atoi(argv[1]);
  
  double d = pi_approx(iters);

  printf("%d: %.50f\n", iters, d);
  d = pi_approx_fast(iters);
  printf("%d: %.50f\n", iters, d);

  return EXIT_SUCCESS;
}

double pi_approx(int x) {
  double res = 0.0;

  for (int i = 0; i <= x; i++) {
    res += fact(i) * fact(i * 2) * (25.0 * i - 3) / ( fact(3 * i) * pow(2.0, i));
  }

  return res * 2.0;
}

double pi_approx_fast(int x) {
  double res = 0.0;
  double fact = 1.0;
  double two_fact = 1.0;
  double three_fact = 1.0;
  double pow_two = 1.0;

  // calculated the 0th term to make the loop less ugly
  // would be
  // res = fact * (two_fact) * (25 * 0 - 3) / three_fact * pow_two

  res = -3.0;
  for (int i = 1; i <= x; i++) {
    pow_two *= 2;
    fact *= i;
    two_fact *= (i * 2) * (i * 2 - 1);
    three_fact *= (i*3) * (i * 3 - 1) * (i*3 - 2);
    
    //res += fact * two_fact * (25 * i - 3) / (three_fact * pow_two);
    res += (fact * two_fact) / (three_fact * pow_two) * (25.0 * i - 3);
  }

  return res * 2.0;
}

double power(double x, int power) {
  double res = 1;
  for (int i = 0 ; i <= power; i++) {
    res *= x;
  }
  return res;
}

double fact(int x) {
  double res = 1.0;
  for (int i = 1 ; i <= x; i++) {
    res *= i;
  }
  return res;
}
