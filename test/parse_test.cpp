//
// Created by dimitrije on 4/23/17.
//

#include <cstdint>
#include <cstddef>
#include <tgmath.h>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std;

double fast_atof(char *str, size_t len) {
    char c = str[len];
    str[len] = '\0';

    double r = 0.0;
    bool neg = false;
    if (*str == '-') {
        neg = true;
        ++str;
    }
    while (*str >= '0' && *str <= '9') {
        r = (r*10.0) + (*str - '0');
        ++str;
    }
    if (*str == '.') {
        double f = 0.0;
        int n = 0;
        ++str;
        while (*str >= '0' && *str <= '9') {
            f = (f*10.0) + (*str - '0');
            ++str;
            ++n;
        }
        r += f / std::pow(10.0, n);
    }
    if (neg) {
        r = -r;
    }

    str[len] = c;

    r = round(r * 100.0) / 100;

    return r;
}

double fast_atof_2(char *str, size_t len) {
    char c = str[len];
    str[len] = '\0';

    int32_t y = 0;
    char x = 0;

    bool neg = false;
    if (*str == '-') {
        neg = true;
        ++str;
    }
    while (*str >= '0' && *str <= '9') {
        y = (y*10) + (*str - '0');
        ++str;
    }
    if (*str == '.') {

        x = (*(str+1) - '0');

        if(*(str+2) >= '0' && *(str+2) <= '9') {
            x += 10 * (*(str+2) - '0');
        }
    }
    if (neg) {
        y = -y;
    }

    str[len] = c;

    return y + (x / 100.0);
}

int main(){

    vector<string> y;

    y.reserve(100);

    y.push_back("10.22");
    y.push_back("11.22");
    y.push_back("12.22");
    y.push_back("13.22");
    y.push_back("14.22");
    y.push_back("15.22");
    y.push_back("16.22");
    y.push_back("17.22");
    y.push_back("18.22");
    y.push_back("19.22");
    y.push_back("20.22");
    y.push_back("22.22");
    y.push_back("23.22");
    y.push_back("24.22");
    y.push_back("25.22");
    y.push_back("26.22");
    y.push_back("27.22");
    y.push_back("28.22");
    y.push_back("29.22");
    y.push_back("30.22");
    y.push_back("31.22");
    y.push_back("32.22");
    y.push_back("33.22");
    y.push_back("34.22");
    y.push_back("35.22");
    y.push_back("36.22");
    y.push_back("37.22");
    y.push_back("38.22");
    y.push_back("39.22");
    y.push_back("40.22");
    y.push_back("41.22");
    y.push_back("42.22");

    y.push_back("10.22");
    y.push_back("11.22");
    y.push_back("12.22");
    y.push_back("13.22");
    y.push_back("14.22");
    y.push_back("15.22");
    y.push_back("16.22");
    y.push_back("17.22");
    y.push_back("18.22");
    y.push_back("19.22");
    y.push_back("20.22");
    y.push_back("22.22");
    y.push_back("23.22");
    y.push_back("24.22");
    y.push_back("25.22");
    y.push_back("26.22");
    y.push_back("27.22");
    y.push_back("28.22");
    y.push_back("29.22");
    y.push_back("30.22");
    y.push_back("31.22");
    y.push_back("32.22");
    y.push_back("33.22");
    y.push_back("34.22");
    y.push_back("35.22");
    y.push_back("36.22");
    y.push_back("37.22");
    y.push_back("38.22");
    y.push_back("39.22");
    y.push_back("40.22");
    y.push_back("41.22");
    y.push_back("42.22");


    y.push_back("10.22");
    y.push_back("11.22");
    y.push_back("12.22");
    y.push_back("13.22");
    y.push_back("14.22");
    y.push_back("15.22");
    y.push_back("16.22");
    y.push_back("17.22");
    y.push_back("18.22");
    y.push_back("19.22");
    y.push_back("20.22");
    y.push_back("22.22");
    y.push_back("23.22");
    y.push_back("24.22");
    y.push_back("25.22");
    y.push_back("26.22");
    y.push_back("27.22");
    y.push_back("28.22");
    y.push_back("29.22");
    y.push_back("30.22");
    y.push_back("31.22");
    y.push_back("32.22");
    y.push_back("33.22");
    y.push_back("34.22");
    y.push_back("35.22");
    y.push_back("36.22");
    y.push_back("37.22");
    y.push_back("38.22");
    y.push_back("39.22");
    y.push_back("40.22");
    y.push_back("41.22");
    y.push_back("42.22");


    y.push_back("10.22");
    y.push_back("11.22");
    y.push_back("12.22");
    y.push_back("13.22");
    y.push_back("14.22");
    y.push_back("15.22");
    y.push_back("16.22");
    y.push_back("17.22");
    y.push_back("18.22");
    y.push_back("19.22");
    y.push_back("20.22");
    y.push_back("22.22");
    y.push_back("23.22");
    y.push_back("24.22");
    y.push_back("25.22");
    y.push_back("26.22");
    y.push_back("27.22");
    y.push_back("28.22");
    y.push_back("29.22");
    y.push_back("30.22");
    y.push_back("31.22");
    y.push_back("32.22");
    y.push_back("33.22");
    y.push_back("34.22");
    y.push_back("35.22");
    y.push_back("36.22");
    y.push_back("37.22");
    y.push_back("38.22");
    y.push_back("39.22");
    y.push_back("40.22");
    y.push_back("41.22");
    y.push_back("42.22");

    y.push_back("10.22");
    y.push_back("11.22");
    y.push_back("12.22");
    y.push_back("13.22");
    y.push_back("14.22");
    y.push_back("15.22");
    y.push_back("16.22");
    y.push_back("17.22");
    y.push_back("18.22");
    y.push_back("19.22");
    y.push_back("20.22");
    y.push_back("22.22");
    y.push_back("23.22");
    y.push_back("24.22");
    y.push_back("25.22");
    y.push_back("26.22");
    y.push_back("27.22");
    y.push_back("28.22");
    y.push_back("29.22");
    y.push_back("30.22");
    y.push_back("31.22");
    y.push_back("32.22");
    y.push_back("33.22");
    y.push_back("34.22");
    y.push_back("35.22");
    y.push_back("36.22");
    y.push_back("37.22");
    y.push_back("38.22");
    y.push_back("39.22");
    y.push_back("40.22");
    y.push_back("41.22");
    y.push_back("42.22");

    y.push_back("10.22");
    y.push_back("11.22");
    y.push_back("12.22");
    y.push_back("13.22");
    y.push_back("14.22");
    y.push_back("15.22");
    y.push_back("16.22");
    y.push_back("17.22");
    y.push_back("18.22");
    y.push_back("19.22");
    y.push_back("20.22");
    y.push_back("22.22");
    y.push_back("23.22");
    y.push_back("24.22");
    y.push_back("25.22");
    y.push_back("26.22");
    y.push_back("27.22");
    y.push_back("28.22");
    y.push_back("29.22");
    y.push_back("30.22");
    y.push_back("31.22");
    y.push_back("32.22");
    y.push_back("33.22");
    y.push_back("34.22");
    y.push_back("35.22");
    y.push_back("36.22");
    y.push_back("37.22");
    y.push_back("38.22");
    y.push_back("39.22");
    y.push_back("40.22");
    y.push_back("41.22");
    y.push_back("42.22");

    double d = 0;
    auto start = std::chrono::system_clock::now();

    for(auto it = y.begin(); it != y.end(); ++it) {
        d += fast_atof_2((char*)it->c_str(), it->length());
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;
    std::cout << elapsed.count() << '\n';

    printf("%.18lg\n", d);
}

/*
 *  13590
    5064.240000

 */