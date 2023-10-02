#define _USE_MATH_DEFINES  

#include <functional>
#include <cmath>


double dichotomy_method(const std::function<double(double)>& f, double a, double b, double epsilon)
{
    int counter = 0;
    double delta = epsilon / 4, c, d;

    while ((b - a) / 2 >= epsilon)
    {
        c = (b + a - delta) / 2;
        d = (b + a + delta) / 2;

        if (f(c) >= f(d))
            a = d;
        else
            b = c;

        counter += 2;
    }

    return (b + a) / 2;
}
double gold_ratio_method(const std::function<double(double)>& f, double a, double b, double epsilon)
{
    int counter = 2;
    const double sqrt5 = std::sqrt(5);

    double c = (b - a) * (3 - sqrt5) / 2 + a,
        d = (b - a) * (sqrt5 - 1) / 2 + a,
        f_c = f(c),
        f_d = f(d);

    while ((b - a) / 2 >= epsilon)
    {
        if (f_c <= f_d)
        {
            b = d;
            d = c;
            c = (b - a) * (3 - sqrt5) / 2 + a;
            f_d = f_c;
            f_c = f(c);
        }
        else
        {
            a = c;
            c = d;
            d = (b - a) * (sqrt5 - 1) / 2 + a;
            f_c = f_d;
            f_d = f(d);
        }

        ++counter;

    }

    return (b + a) / 2;
}
double newton_method(const std::function<double(double)>& df, const std::function<double(double)>& ddf,
    double a, double b, double epsilon)
{
    int df_count = 0, ddf_count = 1;
    double x = a, df_value = df(x), ddf_value;

    while (abs(df_value) > epsilon)
    {
        ddf_value = ddf(x);
        df_value = df(x);

        x -= df_value / ddf_value;
        ++df_count; ++ddf_count;

    }

    return x;
}
double tangent_method(const std::function<double(double)>& f, const std::function<double(double)>& der_f,
    double a, double b, double epsilon)
{
    int count = 0, count_der = 2;
    if (der_f(a) >= 0) return a;
    if (der_f(b) <= 0) return b;

    double c, value;

    printf("%*c%*c%*c%*s%*s%*s\n", 15, 'a', 15, 'b', 15, 'c', 15, "f\'(c)", 15, "count f", 15, "count f\'");
    while ((b - a) / 2 >= epsilon)
    {
        c = (f(b) - f(a) + a * der_f(a) - b * der_f(b)) / (der_f(a) - der_f(b));

        count += 2; count_der += 3;

        value = der_f(c);

        printf("%*.4f%*.4f%*.4f%*.4f%*u%*u\n", 15, a, 15, b, 15, c, 15, value, 15, count, 15, count_der);
        if (value > 0)
            b = c;
        else if (value < 0)
            a = c;
        else
            return c;
    }

    printf("%*.4f%*.4f%*.4f%*.4f%*u%*u\n", 15, a, 15, b, 15, c, 15, value, 15, count, 15, count_der);
    printf("\nTANGENT METHOD USED F AND F\' %u AND %u TIMES\n", count, count_der);

    return (b + a) / 2;
}
double gradient_descent_method(const std::function<double(double)>& df,
    double a, double b, double epsilon)
{
    const double alpha = 0.01;
    double x = a, prev_x, df_value = df(a), prev_df_value;
    int count_df = 1;

    printf("%*c%*s%*s%*s%*s\n", 15, 'x', 15, "prev_x", 15, "df_value", 15, "prev_df_value", 15, "count f\'");
    do {
        prev_x = x;
        prev_df_value = df_value;
        x -= alpha * df_value;
        df_value = df(x);
        ++count_df;
        printf("%*.4A%*.4A%*.4A%*.4A%*u\n", 15, x, 15, prev_x, 15, df_value, 15, prev_df_value, 15, count_df);
    } while (abs(prev_x - x) >= epsilon && abs(df_value - prev_df_value) >= epsilon);

    printf("\nGRADIENT DESCENT METHOD USED F\' %uTIMES\n", count_df);
    return x;
}
int main()
{
    int right_digits = 5;

    const double a = 0, b = M_PI, epsilon = 8 / std::pow(16, right_digits + 1);
    const std::function<double(double)> f {[](double x) {return exp(.84 * x) + exp(.2 * x) - 121.0 * sin(x); }};
    const std::function<double(double)> der_f {[](double x) {return .84 * exp(.84 * x) + .2 * exp(.2 * x) - 121.0 * cos(x); }};
    const std::function<double(double)> der2_f {[](double x) {return .7056 * exp(.84 * x) + .04 * exp(.2 * x) + 121.0 * sin(x); }};

    printf("MIN - %.3A\n\n", clr(dichotomy_method(f, a, b, epsilon), right_digits));
    printf("MIN - %.3A\n\n", clr(gold_ratio_method(f, a, b, epsilon), right_digits));
    printf("MIN - %.3A\n\n", clr(tangent_method(f, der_f, a, b, epsilon), right_digits));
    printf("MIN - %.3A\n\n", clr(newton_method(der_f, der2_f, a, b, epsilon), right_digits));
    printf("MIN - %.3A\n\n", clr(gradient_descent_method(der_f, a, b, epsilon), right_digits));
}