#include "OMPEval/omp/EquityCalculator.h"
#include <iostream>
int main()
{
    omp::EquityCalculator eq;
    eq.start({"AK", "QQ"});
    eq.wait();
    auto r = eq.getResults();
    std::cout << r.equity[0] << " " << r.equity[1] << std::endl;
}
