#ifndef BITMANIP_H
#define BITMANIP_H

#include <bitset>
#include <fstream>
#include <limits>

// http://en.cppreference.com/w/cpp/language/type_alias
using byte = unsigned char;

// http://en.cppreference.com/w/cpp/types/numeric_limits
constexpr std::size_t BITS_PER_BYTE = std::numeric_limits<byte>::digits;

// http://en.cppreference.com/w/cpp/utility/bitset
using bits_in_byte = std::bitset<BITS_PER_BYTE>;

// reverse order of bits in byte
// / e.g. 1110 -> 0111, 0010 -> 0100
// from https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
unsigned char reverse_bits_order(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

template<size_t s1,size_t s2>
void inject_bitset( std::bitset<s1>& bs1, const std::bitset<s2>& bs2, int start = s1-s2)
{
    for(size_t i=0; i<s2; i++)
        bs1[i+start] = bs2[i];
}

// read bits in n sequential bytes from file stream ifs into a single bitset
template<size_t nbytes>
std::bitset<BITS_PER_BYTE*nbytes> read_bits(std::ifstream& ifs)
{
    std::bitset<BITS_PER_BYTE*nbytes> bits;

    char c;
    size_t count = 0;
    while ((count < nbytes) && ifs.get(c))
    {
        unsigned char byte = (unsigned char) c;
        auto curr_bits = bits_in_byte( byte );
        inject_bitset(bits, curr_bits, count*BITS_PER_BYTE);

        ++count;
    }

    return bits;
}

// return a range from bitset "bits" of length s2, starting at index start_idx
template<size_t s1, size_t s2>
std::bitset<s2> range(std::bitset<s1> bits, size_t start_idx)
{
    std::bitset<s2> bits2;
    for (size_t i = 0; i < s2; ++i)
    {
        bits2[i] = bits[start_idx + i];
    }

    return bits2;
}

#endif
