//
// Created by dimitrije on 27/04/17.
//

#ifndef TOSTRING_CONSTANTS_H
#define TOSTRING_CONSTANTS_H

int32_t MIN_SMALL_BIN_EXP = -( 63 / 3 );
int32_t MAX_SMALL_BIN_EXP = 62;
uint32_t EXP_BIAS = 1023;
uint64_t SIGN_BIT_MASK = 0x8000000000000000L;
uint64_t SIGNIF_BIT_MASK = 0x000FFFFFFFFFFFFFL;
uint64_t EXP_BIT_MASK = 0x7FF0000000000000L;
uint32_t SIGNIFICAND_WIDTH = 53;
uint32_t EXP_SHIFT = SIGNIFICAND_WIDTH - 1;
uint64_t FRACT_HOB = (1LU << EXP_SHIFT);
uint64_t EXP_ONE = ((uint64_t)EXP_BIAS)<<EXP_SHIFT;

const int64_t LONG_5_POW_LENGTH = 27;


int32_t insignificantDigitsNumber[] = {
        0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3,
        4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7,
        8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 11, 11, 11,
        12, 12, 12, 12, 13, 13, 13, 14, 14, 14,
        15, 15, 15, 15, 16, 16, 16, 17, 17, 17,
        18, 18, 18, 19
};

const int insignificantDigitsNumberLength = 64;

const int64_t LONG_5_POW[] = {
        1L,
        5L,
        5L * 5,
        5L * 5 * 5,
        5L * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5L * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
};

const int N_5_BITS_LENGTH = 27;

int32_t N_5_BITS[] = {
        0,
        3,
        5,
        7,
        10,
        12,
        14,
        17,
        19,
        21,
        24,
        26,
        28,
        31,
        33,
        35,
        38,
        40,
        42,
        45,
        47,
        49,
        52,
        54,
        56,
        59,
        61,
};

int32_t SMALL_5_POW[] = {
        1,
        5,
        5 * 5,
        5 * 5 * 5,
        5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5,
        5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5
};

int32_t SMALL_5_POW_LENGTH = 14;

/* count leading zeroes */
#define _clz(x) __builtin_clz (x)

/* count trailing zeroes */
#define _ctz(x) __builtin_ctz (x)

/* Simulate 64-bit count leading zeroes */
#define clz(x) __extension__({ \
    unsigned long long __x = (x); \
    unsigned int __hx = (__x >> 32); \
    __hx ? _clz(__hx) : 32 + _clz(__x); \
})

/* Simulate 64-bit count trailing zeroes */
#define ctz(x) __extension__({ \
    unsigned long long __dx = (x); \
    unsigned int __ldx = __dx; \
    unsigned int __hdx = __dx >> 32; \
    __ldx ? _ctz(__ldx) : (63 ^ _clz(__hdx & -__hdx)); \
})

int estimateDecExp(uint64_t fractBits, int binExp) {

    long tmp = EXP_ONE | ( fractBits & SIGNIF_BIT_MASK );

    double d2 = *((double*)&tmp);
    double d = (d2-1.5D)*0.289529654D + 0.176091259 + (double)binExp * 0.301029995663981;

    long dBits = *((long*)&d);  //can't be NaN here so use raw

    int exponent = (int)((dBits & EXP_BIT_MASK) >> EXP_SHIFT) - EXP_BIAS;
    bool isNegative = (dBits & SIGN_BIT_MASK) != 0; // discover sign
    if(exponent>=0 && exponent<52) { // hot path
        long mask   = SIGNIF_BIT_MASK >> exponent;
        int r = (int)(( (dBits & SIGNIF_BIT_MASK) | FRACT_HOB )>>(EXP_SHIFT-exponent));
        return isNegative ? (((mask & dBits) == 0L ) ? -r : -r-1 ) : r;
    } else if (exponent < 0) {
        return (((dBits&~SIGN_BIT_MASK) == 0) ? 0 :
                ( (isNegative) ? -1 : 0) );
    } else { //if (exponent >= 52)
        return (int)d;
    }
}

#endif //TOSTRING_CONSTANTS_H
