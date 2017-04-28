//
// Created by dimitrije on 4/28/17.
//

#ifndef HOBBITREWRITE_DOUBLE_CONVERTER_H
#define HOBBITREWRITE_DOUBLE_CONVERTER_H

#include <assert.h>
#include <string>

using namespace std;

class double_converter {

private:

    bool isNegative;
    int decExponent;
    int firstDigitIndex;
    int nDigits;
    char digits[20];
    int digits_length = 20;

public:

    bool decimalDigitsRoundedUp;
    bool exactDecimalConversion;

    void dtoa(uint32_t exp, uint64_t bits, uint32_t significant_bits, bool format);

    void roundup();

    int32_t insignificantDigitsForPow2(int32_t p2);

    void developLongDigits(int32_t decExponent, int64_t lvalue, int32_t insignificantDigits);

    string getBinaryToASCIIConverter(double d, bool isCompatibleFormat);

    void copy_to_string(char* to);
};



#endif //HOBBITREWRITE_DOUBLE_CONVERTER_H
