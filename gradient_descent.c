#include <stdio.h>
#include <math.h>

#define EPSILON 1e-4

int count = 0;

// Function pointer types for functions of two variables and their gradients
typedef double (*Func2D)(double x, double y);
typedef void (*GradFunc2D)(double x, double y, double* dx, double* dy);

// Struct to hold data needed by the line search function gfunc
typedef struct {
    Func2D func;
    double x;
    double y;
    double dx;
    double dy;
} LineSearchData;

// gfunc(t) = f(x - t*dx, y - t*dy)
double gfunc(double t, void* data) {
    LineSearchData* ls = (LineSearchData*)data;
    return ls->func(ls->x - t * ls->dx, ls->y - t * ls->dy);
}

// Golden section search to find minimum of gfunc over [left, right]
double golden_section_search(double (*g)(double, void*), void* data, double left, double right) {
    const double phi = 0.618;
    double f1 = g(left, data);
    double f2 = g(right, data);

    while ((right - left) > EPSILON) {
		count++;
        if (f1 < f2) {
            right = left + phi * (right - left);
            f2 = g(right, data);
        } else {
            left = left + (1 - phi) * (right - left);
            f1 = g(left, data);
        }
    }
    return (left + right) / 2.0;
}

// Gradient descent method with golden section search line search
void solve(Func2D func, GradFunc2D gradf, double* x_out, double* y_out) {
    double x = 10.0, y = 10.0;
    double prev_x = 0.0, prev_y = 0.0;
    double dx, dy;
    double t = 0.5;
	double prev_dx = 0;
	double prev_dy = 0;

	int step = 0;

    gradf(x, y, &dx, &dy);

    LineSearchData lsdata = {func, x, y, dx, dy};

    while (fabs(prev_x - x) > EPSILON || fabs(prev_y - y) > EPSILON) {
		printf("step = %d, f = %f , x = %f, y = %f, dx = %f, dy = %f, scalar = %f\n", step, t, x, y, x - prev_x, y - prev_y, prev_dx * dx + prev_dy * dy);
		step++;
        count = 0;
		prev_x = x;
        prev_y = y;

        lsdata.x = prev_x;
        lsdata.y = prev_y;
        lsdata.dx = dx;
        lsdata.dy = dy;

        t = golden_section_search(gfunc, &lsdata, 0.0, 10.0);

        x = prev_x - t * dx;
        y = prev_y - t * dy;

		prev_dx = dx;
		prev_dy = dy;

        gradf(x, y, &dx, &dy);
    }

	printf("answer = %f, iters = %d\n", t, count);
    *x_out = x;
    *y_out = y;
}

// Define the four functions and their gradients

double f0(double x, double y) {
    return x*x - 6*x + y*y - 8*y;
}

void gradf0(double x, double y, double* dx, double* dy) {
    *dx = 2*x - 6;
    *dy = 2*y - 8;
}

double f1(double x, double y) {
    return 2*x*x + 8*x + y*y - 6*y;
}

void gradf1(double x, double y, double* dx, double* dy) {
    *dx = 4*x + 8;
    *dy = 2*y - 6;
}

double f2(double x, double y) {
    return 2*x*x + 8*x + y*y - 6*y;
}

void gradf2(double x, double y, double* dx, double* dy) {
    *dx = 4*x + 8;
    *dy = 2*y - 6;
}

double f3(double x, double y) {
    return (x*x - y)*(x*x - y) + (y - 2)*(y - 2);
}

void gradf3(double x, double y, double* dx, double* dy) {
    *dx = 4*x*(x*x - y);
    *dy = -2*(x*x - y) + 2*(y - 2);
}

int main() {
    double x, y;

    solve(f0, gradf0, &x, &y);
    printf("Minimum of f0 at: %.6f %.6f\n\n", x, y); // Expected near 3, 4

    solve(f1, gradf1, &x, &y);
    printf("Minimum of f1 at: %.6f %.6f\n\n", x, y); // Expected near -4, 3

    solve(f2, gradf2, &x, &y);
    printf("Minimum of f2 at: %.6f %.6f\n\n", x, y); // Expected near -2, 3

    solve(f3, gradf3, &x, &y);
    printf("Minimum of f3 at: %.6f %.6f\n\n", x, y); // Near +-sqrt(2), 2

    return 0;
}