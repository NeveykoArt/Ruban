#include <stdio.h>
#include <math.h>

const double eps = 1e-4;

int func_count = 0;

double func(double x, double y) {
    func_count++;
    return x * x - 6 * x + y * y - 8 * y;
}

double find_y(double x, double left_y, double right_y, int *count_y) {
    double new_left = left_y + 0.382 * (right_y - left_y);
    double new_right = left_y + 0.618 * (right_y - left_y);
    double f1 = func(x, new_left);
    double f2 = func(x, new_right);
    while (fabs(right_y - left_y) > eps) {
        if (f1 > f2) {
            left_y = new_left;
            new_left = left_y + 0.382 * (right_y - left_y);
            new_right = left_y + 0.618 * (right_y - left_y);
            f1 = f2;
            f2 = func(x, new_right);
        } else {
            right_y = new_right;
            new_left = left_y + 0.382 * (right_y - left_y);
            new_right = left_y + 0.618 * (right_y - left_y);
            f2 = f1;
            f1 = func(x, new_left);
        }
        (*count_y)++;
    }
    return (left_y + right_y) / 2.0;
}

void find_xy(double left_x, double right_x, double left_y, double right_y, double *res_x, double *res_y, double *res) {
    int count_x = 0;
    int count_y = 0;
    double new_left_x = left_x + 0.382 * (right_x - left_x);
    double new_right_x = left_x + 0.618 * (right_x - left_x);
    double new_left_y = find_y(new_left_x, left_y, right_y, &count_y);
    double new_right_y = find_y(new_right_x, left_y, right_y, &count_y);
    double f1 = func(new_left_x, new_left_y);
    double f2 = func(new_right_x, new_right_y);

    while (fabs(right_x - left_x) > eps) {
        if (f1 > f2) {
            left_x = new_left_x;
            new_left_x = left_x + 0.382 * (right_x - left_x);
            new_right_x = left_x + 0.618 * (right_x - left_x);
            new_left_y = new_right_y;
            new_right_y = find_y(new_right_x, left_y, right_y, &count_y);
            f1 = f2;
            f2 = func(new_right_x, new_right_y);
        } else {
            right_x = new_right_x;
            new_left_x = left_x + 0.382 * (right_x - left_x);
            new_right_x = left_x + 0.618 * (right_x - left_x);
            new_right_y = new_left_y;
            new_left_y = find_y(new_left_x, left_y, right_y, &count_y);
            f2 = f1;
            f1 = func(new_left_x, new_left_y);
        }
        printf("left %f; right %f; y_iter %d\n", left_x, right_x, count_y);
        count_x++;
        count_y = 0;
    }
    printf("x iter: %d\n", count_x);
    *res_x = (left_x + right_x) / 2.0;
    *res_y = find_y(*res_x, left_y, right_y, &count_y);
    *res = func(*res_x, *res_y);
}

int main() {
    double res_x, res_y, res;
    find_xy(-10, 10, -10, 10, &res_x, &res_y, &res);
    printf("Result: x = %f, y = %f, f(x,y) = %f\n", res_x, res_y, res);
    return 0;
}