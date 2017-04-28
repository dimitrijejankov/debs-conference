//
// Created by dimitrije on 4/28/17.
//

#include <cstring>
#include "double_converter.h"
#include "constants.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

void double_converter::roundup() {
    int i = (this->firstDigitIndex + this->nDigits - 1);
    int q = digits[i];
    if (q == '9') {
        while (q == '9' && i > this->firstDigitIndex) {
            digits[i] = '0';
            q = digits[--i];
        }
        if (q == '9') {
            // carryout! High-order 1, rest 0s, larger exp.
            this->decExponent += 1;
            digits[this->firstDigitIndex] = '1';
            return;
        }
        // else fall through.
    }
    digits[i] = (char) (q + 1);
    decimalDigitsRoundedUp = true;
}

int32_t double_converter::insignificantDigitsForPow2(int32_t p2) {
    if(p2>1 && p2 < insignificantDigitsNumberLength) {
        return insignificantDigitsNumber[p2];
    }
    return 0;
}

void double_converter::developLongDigits( int32_t decExponent, int64_t lvalue, int32_t insignificantDigits ){
    if ( insignificantDigits != 0 ){
        // Discard non-significant low-order bits, while rounding,
        // up to insignificant value.
        long pow10 = LONG_5_POW[insignificantDigits] << insignificantDigits; // 10^i == 5^i * 2^i;
        long residue = lvalue % pow10;
        lvalue /= pow10;
        decExponent += insignificantDigits;
        if ( residue >= (pow10>>1) ){
            // round up based on the low-order bits we're discarding
            lvalue++;
        }
    }

    int  digitno = digits_length -1;
    int  c;
    if ( lvalue <= INT32_MAX ){
        assert (lvalue > 0L); // : lvalue; // lvalue <= 0
        // even easier subcase!
        // can do int arithmetic rather than long!
        int  ivalue = (int)lvalue;
        c = ivalue%10;
        ivalue /= 10;
        while ( c == 0 ){
            decExponent++;
            c = ivalue%10;
            ivalue /= 10;
        }
        while ( ivalue != 0){
            digits[digitno--] = (char)(c+'0');
            decExponent++;
            c = ivalue%10;
            ivalue /= 10;
        }
        digits[digitno] = (char)(c+'0');
    } else {
        // same algorithm as above (same bugs, too )
        // but using long arithmetic.
        c = (int)(lvalue%10L);
        lvalue /= 10L;
        while ( c == 0 ){
            decExponent++;
            c = (int)(lvalue%10L);
            lvalue /= 10L;
        }
        while ( lvalue != 0L ){
            digits[digitno--] = (char)(c+'0');
            decExponent++;
            c = (int)(lvalue%10L);
            lvalue /= 10;
        }
        digits[digitno] = (char)(c+'0');
    }
    this->decExponent = decExponent + 1;
    this->firstDigitIndex = digitno;
    this->nDigits = digits_length - digitno;
}

string double_converter::getBinaryToASCIIConverter(double d, bool isCompatibleFormat) {

    // convert to double
    uint64_t d_bits = *((uint64_t *) &d);;

    // discover sign
    bool is_negative = (d_bits & SIGN_BIT_MASK) != 0;

    // do something i guess...
    uint64_t fract_bits = d_bits & SIGNIF_BIT_MASK;

    // again som mojo
    uint32_t bin_exp = (uint32_t) ((d_bits & EXP_BIT_MASK) >> EXP_SHIFT);

    // Discover obvious special cases of NaN and Infinity.
    if (bin_exp == (int) (EXP_BIT_MASK >> EXP_SHIFT)) {
        if (fract_bits == 0L) {
            return is_negative ? "-inf" : "+inf";
        } else {
            return "nan";
        }
    }
    // Finish unpacking
    // Normalize denormalized numbers.
    // Insert assumed high-order bit for normalized numbers.
    // Subtract exponent bias.
    uint32_t n_significant_bits;
    if (bin_exp == 0) {

        if (fract_bits == 0L) {
            // not a denorm, just a 0!
            return is_negative ? "-0" : "+0";
        }

        uint32_t leading_zeros = (uint32_t) clz(fract_bits);
        uint32_t shift = leading_zeros - (63 - EXP_SHIFT);
        fract_bits <<= shift;
        bin_exp = 1 - shift;

        // recall bin_exp is  - shift count.
        n_significant_bits = 64 - leading_zeros;
    } else {

        fract_bits |= FRACT_HOB;
        n_significant_bits = EXP_SHIFT + 1;
    }

    bin_exp -= EXP_BIAS;
    this->isNegative = is_negative;

    // call the routine that actually does all the hard work.
    dtoa(bin_exp, fract_bits, n_significant_bits, isCompatibleFormat);

    return "";
}

void double_converter::dtoa(uint32_t binExp, uint64_t fractBits, uint32_t nSignificantBits, bool isCompatibleFormat) {
    assert(fractBits > 0) ; // fractBits here can't be zero or negative
    assert ((fractBits & FRACT_HOB)!=0)  ; // Hi-order bit should be set
    // Examine number. Determine if it is an easy case,
    // which we can do pretty trivially using float/long conversion,
    // or whether we must do real work.
    const int32_t tailZeros = ctz(fractBits);

    // number of significant bits of fractBits;
    int32_t nFractBits = EXP_SHIFT + 1 - tailZeros;

    // reset flags to default values as dtoa() does not always set these
    // flags and a prior call to dtoa() might have set them to incorrect
    // values with respect to the current state.
    decimalDigitsRoundedUp = false;
    exactDecimalConversion = false;

    // number of significant bits to the right of the point.
    int32_t nTinyBits = std::max( 0U, nFractBits - binExp - 1 );
    if ( binExp <= MAX_SMALL_BIN_EXP && binExp >= MIN_SMALL_BIN_EXP ){
        // Look more closely at the number to decide if,
        // with scaling by 10^nTinyBits, the result will fit in
        // a long.
        if ( (nTinyBits < LONG_5_POW_LENGTH) && ((nFractBits + N_5_BITS[nTinyBits]) < 64 ) ){
            //
            // We can do this:
            // take the fraction bits, which are normalized.
            // (a) nTinyBits == 0: Shift left or right appropriately
            //     to align the binary point at the extreme right, i.e.
            //     where a long int point is expected to be. The integer
            //     result is easily converted to a string.
            // (b) nTinyBits > 0: Shift right by EXP_SHIFT-nFractBits,
            //     which effectively converts to long and scales by
            //     2^nTinyBits. Then multiply by 5^nTinyBits to
            //     complete the scaling. We know this won't overflow
            //     because we just counted the number of bits necessary
            //     in the result. The integer you get from this can
            //     then be converted to a string pretty easily.
            //
            if ( nTinyBits == 0 ) {
                int32_t insignificant;
                if ( binExp > nSignificantBits ){
                    insignificant = insignificantDigitsForPow2(binExp - nSignificantBits - 1);
                } else {
                    insignificant = 0;
                }
                if ( binExp >= EXP_SHIFT ){
                    fractBits <<= (binExp-EXP_SHIFT);
                } else {
                    fractBits >>= (EXP_SHIFT-binExp) ;
                }
                developLongDigits( 0, fractBits, insignificant );
                return;
            }
            //
            // The following causes excess digits to be printed
            // out in the single-float case. Our manipulation of
            // halfULP here is apparently not correct. If we
            // better understand how this works, perhaps we can
            // use this special case again. But for the time being,
            // we do not.
            // else {
            //     fractBits >>>= EXP_SHIFT+1-nFractBits;
            //     fractBits//= long5pow[ nTinyBits ];
            //     halfULP = long5pow[ nTinyBits ] >> (1+nSignificantBits-nFractBits);
            //     developLongDigits( -nTinyBits, fractBits, insignificantDigits(halfULP) );
            //     return;
            // }
            //
        }
    }
    //
    // This is the hard case. We are going to compute large positive
    // integers B and S and integer decExp, s.t.
    //      d = ( B / S )// 10^decExp
    //      1 <= B / S < 10
    // Obvious choices are:
    //      decExp = floor( log10(d) )
    //      B      = d// 2^nTinyBits// 10^max( 0, -decExp )
    //      S      = 10^max( 0, decExp)// 2^nTinyBits
    // (noting that nTinyBits has already been forced to non-negative)
    // I am also going to compute a large positive integer
    //      M      = (1/2^nSignificantBits)// 2^nTinyBits// 10^max( 0, -decExp )
    // i.e. M is (1/2) of the ULP of d, scaled like B.
    // When we iterate through dividing B/S and picking off the
    // quotient bits, we will know when to stop when the remainder
    // is <= M.
    //
    // We keep track of powers of 2 and powers of 5.
    //
    int decExp = estimateDecExp(fractBits,binExp);
    int B2, B5; // powers of 2 and powers of 5, respectively, in B
    int S2, S5; // powers of 2 and powers of 5, respectively, in S
    int M2, M5; // powers of 2 and powers of 5, respectively, in M

    B5 = std::max( 0, -decExp );
    B2 = B5 + nTinyBits + binExp;

    S5 = std::max( 0, decExp );
    S2 = S5 + nTinyBits;

    M5 = B5;
    M2 = B2 - nSignificantBits;

    //
    // the long integer fractBits contains the (nFractBits) interesting
    // bits from the mantissa of d ( hidden 1 added if necessary) followed
    // by (EXP_SHIFT+1-nFractBits) zeros. In the interest of compactness,
    // I will shift out those zeros before turning fractBits into a
    // FDBigInteger. The resulting whole number will be
    //      d * 2^(nFractBits-1-binExp).
    //
    fractBits >>= tailZeros;
    B2 -= nFractBits-1;
    int common2factor = std::min( B2, S2 );
    B2 -= common2factor;
    S2 -= common2factor;
    M2 -= common2factor;

    //
    // HACK!! For exact powers of two, the next smallest number
    // is only half as far away as we think (because the meaning of
    // ULP changes at power-of-two bounds) for this reason, we
    // hack M2. Hope this works.
    //
    if ( nFractBits == 1 ) {
        M2 -= 1;
    }

    if ( M2 < 0 ){
        // oops.
        // since we cannot scale M down far enough,
        // we must scale the other values up.
        B2 -= M2;
        S2 -= M2;
        M2 =  0;
    }
    //
    // Construct, Scale, iterate.
    // Some day, we'll write a stopping test that takes
    // account of the asymmetry of the spacing of floating-point
    // numbers below perfect powers of 2
    // 26 Sept 96 is not that day.
    // So we use a symmetric test.
    //
    int ndigit = 0;
    bool low, high;
    long lowDigitDifference;
    int  q;

    //
    // Detect the special cases where all the numbers we are about
    // to compute will fit in int or long integers.
    // In these cases, we will avoid doing FDBigInteger arithmetic.
    // We use the same algorithms, except that we "normalize"
    // our FDBigIntegers before iterating. This is to make division easier,
    // as it makes our fist guess (quotient of high-order words)
    // more accurate!
    //
    // Some day, we'll write a stopping test that takes
    // account of the asymmetry of the spacing of floating-point
    // numbers below perfect powers of 2
    // 26 Sept 96 is not that day.
    // So we use a symmetric test.
    //
    // binary digits needed to represent B, approx.
    int Bbits = nFractBits + B2 + (( B5 < N_5_BITS_LENGTH )? N_5_BITS[B5] : ( B5*3 ));

    // binary digits needed to represent 10*S, approx.
    int tenSbits = S2+1 + (( (S5+1) < N_5_BITS_LENGTH )? N_5_BITS[(S5+1)] : ( (S5+1)*3 ));
    if ( Bbits < 64 && tenSbits < 64){
        if ( Bbits < 32 && tenSbits < 32){
            // wa-hoo! They're all ints!
            int b = ((int)fractBits * SMALL_5_POW[B5] ) << B2;
            int s = SMALL_5_POW[S5] << S2;
            int m = SMALL_5_POW[M5] << M2;
            int tens = s * 10;
            //
            // Unroll the first iteration. If our decExp estimate
            // was too high, our first quotient will be zero. In this
            // case, we discard it and decrement decExp.
            //
            ndigit = 0;
            q = b / s;
            b = 10 * ( b % s );
            m *= 10;
            low  = (b <  m );
            high = (b+m > tens );
            assert (q < 10); // excessively large digit
            if ( (q == 0) && ! high ){
                // oops. Usually ignore leading zero.
                decExp--;
            } else {
                digits[ndigit++] = (char)('0' + q);
            }
            //
            // HACK! Java spec sez that we always have at least
            // one digit after the . in either F- or E-form output.
            // Thus we will need more than one digit if we're using
            // E-form
            //
            if ( !isCompatibleFormat ||decExp < -3 || decExp >= 8 ){
                high = low = false;
            }
            while( ! low && ! high ){
                q = b / s;
                b = 10 * ( b % s );
                m *= 10;
                assert (q < 10); // excessively large digit
                if ( m > 0L ){
                    low  = (b <  m );
                    high = (b+m > tens );
                } else {
                    // hack -- m might overflow!
                    // in this case, it is certainly > b,
                    // which won't
                    // and b+m > tens, too, since that has overflowed
                    // either!
                    low = true;
                    high = true;
                }
                digits[ndigit++] = (char)('0' + q);
            }
            lowDigitDifference = (b<<1) - tens;
            exactDecimalConversion  = (b == 0);
        } else {
            // still good! they're all longs!
            long b = (fractBits * LONG_5_POW[B5] ) << B2;
            long s = LONG_5_POW[S5] << S2;
            long m = LONG_5_POW[M5] << M2;
            long tens = s * 10L;
            //
            // Unroll the first iteration. If our decExp estimate
            // was too high, our first quotient will be zero. In this
            // case, we discard it and decrement decExp.
            //
            ndigit = 0;
            q = (int) ( b / s );
            b = 10L * ( b % s );
            m *= 10L;
            low  = (b <  m );
            high = (b+m > tens );
            assert (q < 10); // excessively large digit
            if ( (q == 0) && ! high ){
                // oops. Usually ignore leading zero.
                decExp--;
            } else {
                digits[ndigit++] = (char)('0' + q);
            }
            //
            // HACK! Java spec sez that we always have at least
            // one digit after the . in either F- or E-form output.
            // Thus we will need more than one digit if we're using
            // E-form
            //
            if ( !isCompatibleFormat || decExp < -3 || decExp >= 8 ){
                high = low = false;
            }
            while( ! low && ! high ){
                q = (int) ( b / s );
                b = 10 * ( b % s );
                m *= 10;
                assert(q < 10);  // excessively large digit
                if ( m > 0L ){
                    low  = (b <  m );
                    high = (b+m > tens );
                } else {
                    // hack -- m might overflow!
                    // in this case, it is certainly > b,
                    // which won't
                    // and b+m > tens, too, since that has overflowed
                    // either!
                    low = true;
                    high = true;
                }
                digits[ndigit++] = (char)('0' + q);
            }
            lowDigitDifference = (b<<1) - tens;
            exactDecimalConversion  = (b == 0);
        }
    } else {
        // still good! they're all longs!
        cpp_int b = pow(cpp_int(5), (const unsigned int &) B5) * pow(cpp_int(2), (const unsigned int &) B2); //(fractBits * LONG_5_POW[B5] ) << B2;
        b *= fractBits;
        cpp_int s = pow(cpp_int(5), (const unsigned int &) S5) * pow(cpp_int(2), (const unsigned int &) S2); //LONG_5_POW[S5] << S2;
        cpp_int m = pow(cpp_int(5), (const unsigned int &) M5) * pow(cpp_int(2), (const unsigned int &) M2); //LONG_5_POW[M5] << M2;
        cpp_int tens = s * 10L;
        //
        // Unroll the first iteration. If our decExp estimate
        // was too high, our first quotient will be zero. In this
        // case, we discard it and decrement decExp.
        //
        ndigit = 0;
        cpp_int tmp1 = b / s;
        q = static_cast<int>(tmp1);
        b = 10L * ( b % s );
        m *= 10L;
        low  = (b <  m );
        high = (b+m > tens );
        assert (q < 10); // excessively large digit
        if ( (q == 0) && ! high ){
            // oops. Usually ignore leading zero.
            decExp--;
        } else {
            digits[ndigit++] = (char)('0' + q);
        }
        //
        // HACK! Java spec sez that we always have at least
        // one digit after the . in either F- or E-form output.
        // Thus we will need more than one digit if we're using
        // E-form
        //
        if ( !isCompatibleFormat || decExp < -3 || decExp >= 8 ){
            high = low = false;
        }
        while( ! low && ! high ){
            cpp_int tmp2 = b / s;
            q = static_cast<int>(tmp2);
            b = 10 * ( b % s );
            m *= 10;
            assert(q < 10);  // excessively large digit
            if ( m > 0L ){
                low  = (b <  m );
                high = (b+m > tens );
            } else {
                // hack -- m might overflow!
                // in this case, it is certainly > b,
                // which won't
                // and b+m > tens, too, since that has overflowed
                // either!
                low = true;
                high = true;
            }
            digits[ndigit++] = (char)('0' + q);
        }
        //lowDigitDifference = (b<<1) - tens;
        cpp_int tmp = (b * 2) - tens;
        lowDigitDifference = static_cast<int>(tmp);
        exactDecimalConversion  = (b == 0);
    }
    this->decExponent = decExp+1;
    this->firstDigitIndex = 0;
    this->nDigits = ndigit;
    //
    // Last digit gets rounded based on stopping condition.
    //
    if ( high ){
        if ( low ){
            if ( lowDigitDifference == 0L ){
                // it's a tie!
                // choose based on which digits we like.
                if ( (digits[this->firstDigitIndex + this->nDigits - 1]&1) != 0 ) {
                    roundup();
                }
            } else if ( lowDigitDifference > 0 ){
                roundup();
            }
        } else {
            roundup();
        }
    }
}

void double_converter::copy_to_string(char* to) {

    assert (this->nDigits <= 19); // generous bound on size of nDigits
    int i = 0;
    if (isNegative) {
        to[0] = '-';
        i = 1;
    }
    if (this->decExponent > 0 && this->decExponent < 8) {
        // print digits.digits.
        int charLength = std::min(this->nDigits, this->decExponent);
        memcpy(to + i, digits + this->firstDigitIndex, sizeof(char) * charLength);
        i += charLength;
        if (charLength < this->decExponent) {
            charLength = this->decExponent - charLength;
            memset(to + i, '0', (size_t)charLength);
            i += charLength;
            to[i++] = '.';
            to[i++] = '0';
        } else {
            to[i++] = '.';
            if (charLength < this->nDigits) {
                int t = this->nDigits - charLength;
                memcpy(to + i, digits + this->firstDigitIndex + charLength, sizeof(char) * t);
                i += t;
            } else {
                to[i++] = '0';
            }
        }
    } else if (this->decExponent <= 0 && this->decExponent > -3) {
        to[i++] = '0';
        to[i++] = '.';
        if (this->decExponent != 0) {
            memset(to + i, '0', (size_t)(i - this->decExponent));
            i -= this->decExponent;
        }
        memcpy(to + i, digits + this->firstDigitIndex, (size_t)this->nDigits);
        i += this->nDigits;
    } else {
        to[i++] = digits[this->firstDigitIndex];
        to[i++] = '.';
        if (this->nDigits > 1) {
            memcpy(to + i, digits + this->firstDigitIndex + 1, (size_t) (this->nDigits - 1));
            i += this->nDigits - 1;
        } else {
            to[i++] = '0';
        }
        to[i++] = 'E';
        int e;
        if (this->decExponent <= 0) {
            to[i++] = '-';
            e = -this->decExponent + 1;
        } else {
            e = this->decExponent - 1;
        }
        // decExponent has 1, 2, or 3, digits
        if (e <= 9) {
            to[i++] = (char) (e + '0');
        } else if (e <= 99) {
            to[i++] = (char) (e / 10 + '0');
            to[i++] = (char) (e % 10 + '0');
        } else {
            to[i++] = (char) (e / 100 + '0');
            e %= 100;
            to[i++] = (char) (e / 10 + '0');
            to[i++] = (char) (e % 10 + '0');
        }
    }

    to[i] = '\0';
}