#include "libmx.h"

double mx_pow(double n, unsigned int pow){
    double rez = 1.;

    for(unsigned int i = 1; i <= pow; i++){
        rez *= n;
    }

    return rez;
}


