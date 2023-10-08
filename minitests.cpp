#include<iostream>
#include<iomanip>
#include<random>
#include<limits>

int main(){
    std::random_device rd;
    std::uniform_real_distribution<long double> dist(0.0, 1.0);

    std::cout << std::setprecision(17);
    for(int n=0; n<10; ++n){
        std::cout << dist(rd) << ' ';
    }
    std::cout << '\n';

    long double ld = 1/69.0;
    std::cout << ld << std::endl;
    std::cout << sizeof(ld) << std::endl;

    std::cout << std::numeric_limits<long double>::digits10 << std::endl;

    return 0;
}