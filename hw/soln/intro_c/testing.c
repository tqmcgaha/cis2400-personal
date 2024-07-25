#include <stdio.h>
#include <stdlib.h>


double nilakantha(int x);

double chudnovsky(int x);
double ramanujan(int x);

double power(double x, int power);

int fact(int x);

// based off of the scond one under https://en.wikipedia.org/wiki/List_of_formulae_involving_%CF%80#Efficient_infinite_series
double pi_approx(int x);

int main(int argc, char* argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage: ./pi_approx <integer>\n");
    return EXIT_FAILURE;
  }

  int iters = atoi(argv[1]);

  
  for (int i = 0; i < iters; i++) {
    //double d = pi_approx(i);
    double d = ramanujan(i);
    // double d = nilakantha(i);
    printf("%d: %.50f\n", i, d);
  }

}


double pi_approx(int x) {
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

double nilakantha(int x) {
  // forget the math
  double sign = 1.0;
  double res = 3.0;

  for (int i = 1; i <= x; i++) {
    res += sign * ( 4 / ((2.0 * i) * (2.0 * i + 1) * (2.0 * i + 2)));
    sign *= -1.0;
  }
  
  return res;
}

double power(double x, int power) {
  double res = 1;
  for (int i = 0 ; i <= power; i++) {
    res *= x;
  }
  return res;
}

int fact(int x) {
  int res = 1;
  for (int i = 1 ; i <= x; i++) {
    res *= i;
  }
  return res;
}

double chudnovsky(int x) {
  double res = 0.0;

  double sign = 1.0;
  double fact = 1.0;
  double three_fact = 1.0;
  double six_fact = 1.0;

  double c3 = 100.02499687578100594479218787636; 

  for (int i = 1; i <= x; i++) {
    
  }

  return (4270934400.0 / c3) / res;
}

double ramanujan(int x) {
  double c1 = 1103.0;
  double c2 = 26390.0;
  double c3 = 396.0;
  double c4 = 3465.1767812046761433263377964948;

  double four_fact = 1.0;
  double fact = 1.0;
  double pow = 1.0;
  double res = c1;

  for (int i = 1; i <= x; i++) {
    four_fact *= (i * 4.0) * (i * 4.0 - 1.0) * (i * 4.0 - 2.0) * (i * 4.0 - 3.0);
    fact *= i;
    pow *= (c3 * c3 * c3 * c3);
    res += (four_fact * (c1 + c2 * i)) / ((fact * fact * fact * fact) * pow);
  }

  return c4 / res;
}

