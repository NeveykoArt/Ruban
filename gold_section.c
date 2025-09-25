#include <stdio.h>
#include <math.h>

double epsilon = 0.000001;

double func(double x) {
	return (x * x - 6 * x);
}

double golden_section(double a, double b, double (*func)(double), double epsilon) {
	double phi = (1 + sqrt(5))/2;
	double rho = phi - 1.0;

	double c = b - rho * (b - a);
	double d = a + rho * (b + a);

	double fc = func(c);
	double fd = func(d);

	while (b - a > epsilon) {
		if (fc <= fd) {
			b = d;
			d = c;
			fd = fc;
			c = b - rho * (b - a);
			fc = func(c);
		} else {
			a = c;
			c = d;
			fc = fd;
			d = a + rho * (b - a);
			fd = func(d);
		}
	}

	return (a+b) / 2.0;
}

int main () {

	printf("%f\n", golden_section(0.0, 5.0, func, 0.000001));

	return 0;
}
