#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>
#include <vector>
#include <windows.h>

using namespace std;

using scalar_t = long double;


//Gold ratio method for searching min for real-variable functions
double gold_ratio_method(const function<double(double)>& f, double a, double b, double epsilon)
{
    int counter = 2;
    const double sqrt5 = sqrt(5);

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




// Euclidean_norm of n-dimensional vector x

scalar_t norm(const vector<scalar_t> x)
{
    scalar_t norm = 0;

    for (int i = 0; i < x.size(); ++i)
        norm += x[i] * x[i];

    return sqrt(norm);
}





//calculating vector with vector-function in point

vector<scalar_t> get(const vector<function<scalar_t(const vector<scalar_t>&)>>& f,   //vector-function
                     const vector<scalar_t> x                                        //point
)
{
    vector<scalar_t> f_x(x.size());

    for (int i = 0; i < x.size(); ++i)
        f_x[i] = f[i](x);

    return f_x;
}




// Coordinate descent method

vector<scalar_t> coordinate_descent(const function<scalar_t(scalar_t, scalar_t, scalar_t)>& f, 
                                    scalar_t epsilon,
                                    vector<scalar_t> x0       //start point
)
{
    size_t dim = x0.size();
    vector<scalar_t> x = x0;
    vector<scalar_t> prev_x = x0;

    do
    {
        prev_x = x;
        x[0] = gold_ratio_method(bind(f, placeholders::_1, x[1], x[2]), -5, 5, epsilon / 3);
        x[1] = gold_ratio_method(bind(f, x[0], placeholders::_1, x[2]), -5, 5, epsilon / 3);
        x[2] = gold_ratio_method(bind(f, x[0], x[1], placeholders::_1), -5, 5, epsilon / 3);

    } while (abs(f(x[0], x[1], x[2]) - f(prev_x[0], prev_x[1], prev_x[2])) >= epsilon);

    return x;
}



// Gradient descent method with "crushing" step

vector<scalar_t> gradient_descent(
    const function<scalar_t(const vector<scalar_t>&)>& f,
    const vector<function<scalar_t(const vector<scalar_t>&)>>& g,           //gradient (vector-function) of f
    const vector<scalar_t>& x0,                                             //start point
    scalar_t alpha,                                                         //parametr of method (step)
    scalar_t delta,                                                         //parametr of method
    scalar_t lambda,                                                        //parametr of method lambda from (0, 1)
    scalar_t epsilon                                                        //parametr of method
)
{
    //dimension
    size_t dim = g.size();

    vector<scalar_t> x(dim);
    vector<scalar_t> prev_x = x0;

    while (norm(get(g, x)) >= delta)
    {
        //x = prev_x - alpha * gradient(prev_x);
        for (int i = 0; i < dim; ++i)
            x[i] = prev_x[i] - alpha * g[i](prev_x);
        
        //"Break up" the step and recalculate step if necessary
        while (f(x) - f(prev_x) > -alpha * epsilon * (norm(get(g, x)) * norm(get(g, x))))
        {
            alpha *= lambda;
            for (int i = 0; i < dim; ++i)
                x[i] = prev_x[i] - alpha * g[i](prev_x);
        }

        //update "prev_x"
        swap(prev_x, x);
    }
  
    return x;
}





// Gradient descent method

vector<scalar_t> gradient_descent(
    const function<scalar_t(const vector<scalar_t>&)>& f,
    const vector<function<scalar_t(const vector<scalar_t>&)>>& g,           //gradient (vector-function) of f
    const vector<scalar_t>& x0,                                             //start point
    scalar_t alpha,                                                         //parametr of method (step)
    scalar_t delta                                                          //parametr of method
)
{
    //dimension
    size_t dim = g.size();

    vector<scalar_t> x(dim);
    vector<scalar_t> prev_x = x0;

    while (norm(get(g, x)) >= delta)
    {
        //x = prev_x - alpha * gradient(prev_x);
        for (int i = 0; i < dim; ++i)
            x[i] = prev_x[i] - alpha * g[i](prev_x);

        //update "prev_x"
        swap(prev_x, x);
    }

    return x;
}




int main()
{

    //Our target function
    function<scalar_t(const vector<scalar_t>&)> f =
        [](const vector<scalar_t>& x) -> scalar_t {
        return 3 * x[0] + 4 * x[1] + 5 * x[2] + exp(3 * x[0]) + exp(-3 * x[0]) + exp(4 * x[1]) + exp(-4 * x[1]) + exp(5 * x[2]) + exp(-5 * x[2]);
    };

    //Partial derivative for X
    function<scalar_t(const vector<scalar_t>&)> dfdx =
        [](const vector<scalar_t>& x) -> scalar_t {

        return 3 + 3 * exp(3 * x[0]) - 3 * exp(-3 * x[0]);
    };

    //Partial derivative for Y
    function<scalar_t(const vector<scalar_t>&)> dfdy =
        [](const vector<scalar_t>& x) -> scalar_t {

        return 4 + 4 * exp(4 * x[1]) - 4 * exp(-4 * x[1]);
    };

    //Partial derivative for Z
    function<scalar_t(const vector<scalar_t>&)> dfdz =
        [](const vector<scalar_t>& x) -> scalar_t {

        return 5 + 5 * exp(5 * x[2]) - 5 * exp(-5 * x[2]);
    };

    //Initialization of vector-gradient
    vector< function<scalar_t(const vector<scalar_t>&)> > g {dfdx, dfdy, dfdz};


    //TEST OF METHODS

    //Param of gradient descent method
    scalar_t a = 0.5;
    scalar_t d = 0.000001;
    scalar_t l = 0.5;
    scalar_t e = 0.5;
    vector<scalar_t> start_point(3, scalar_t(0));



    vector<scalar_t> min = gradient_descent(f, g, start_point, a, d, l, e);

    for (auto x : min)
        cout << x << ' ';
    cout << endl;



    min = gradient_descent(f, g, start_point, 0.01, d);

    for (auto x : min)
        cout << x << ' ';
    cout << endl;


    e = 0.0000001;
    auto f2 = [](scalar_t x, scalar_t y, scalar_t z) -> scalar_t
    {
        return 3 * x + 4 * y + 5 * z + exp(3 * x) + exp(-3 * x) + exp(4 * y) + exp(-4 * y) + exp(5 * z) + exp(-5 * z);
    };
    min = coordinate_descent(f2, e, start_point);
        
    for (auto x : min)
        cout << x << ' ';
}