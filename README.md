## STEPS:

1. Generate Orientation Pattern (STABLE)
2. Generate Timing Pattern (STABLE)
3. Generate Format Pattern
    - Error Correction Level (EC)
        - Reed-Solomon's Codes: RS(n, k) of s=8 bits, 
            n=total bytes, 
            k=data bytes, 
            n-k=parity bytes,
            2t=n-k, t=number of bytes that can be recovered

            - Version 2 Level L: 32 bytes,
                total number of codewords: 34
                EC codewords per block: 10
                n-k=10 bytes, k=34 bytes, n=44 bytes
            - Galois Field Used For QR Codes: GF(256) 0-255 (8-bits)
            - All numbers in Galois Field can be represented by 2^n (n: 0-255) if
            a number is larger than 255 we % with 285 in binary (XOR)
            - Message Polynomial (m): coeffecients are codewords of encoded data divided into 8 bits
                Convert Data codewords to decimal then multiply by (x^n of codewords-1)
                Multiply the polynomial by x^n (n is EC codewords)
            - Generator Polynomial (g) for 10 EC codewords: 
                a^251x^9+a^67x^8+a^46x^7+a^61x^6+a^118x^5+a^70x^4+a^64x^3+a^94x^2+a^32x+a^45
            - Make the two polynomials the have the same exponent in the lead
            - The EC Code words are the coeffecients of the terms of the remainder from the polynomial
                long division of the messsage polynomial by the generator polynomial

    - Mask Pattern
4. Data Encoding
    - Total number of bits required: 272 bits
    - Mode Indicator: Byte mode (0100)
    - Character Count Indicator: 8 bits (pad with zeros)
    - Encoded Data: Convert Characters according to ASCII then to binary
    - Terminator (to the right): Up to 4 bits
    - If not multiple of eight add zeros to left
    - Pad bytes: 11101100 00010001
