#include "math.hpp"
#include <SDL.h>
#include <tuple>

std::vector<std::tuple<double,double,double,double,double>> dft( std::vector<double>&vals, const int N ){

    std::vector<std::tuple<double,double,double,double,double>> X(N);

    for(int k = 0; k<N; k++){

        double re = 0;
        double im = 0;

        for(int n=0; n<N ; n++){

            const double phi = (2.f * M_PI * (k*1.f) * (n*1.f)) / (N*1.f);
            re += vals[n] * std::cos(phi);
            im -= vals[n] * std::sin(phi);

        }

        double freq = k;
        double amp = std::sqrt(re*re+im*im);
        double phase = std::atan2(im, re); 
        re = re/N;
        im = im/N;

        X[k] = std::make_tuple(re, im, freq, amp, phase);
    }

    return X;

}