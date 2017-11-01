/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * The BinaryBuffer module handles the translation of
 *  bit sequence huffman codings to bytes for io
 *  operations.
 *
 * Uses a bitset as the buffer. I use binary operations
 *  to put a n-size sequence into the buffer. Then ues
 *  bitshift operations to pop bytes of the buffer.
 *
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#ifndef BINARY_BUFFER
    #define BINARY_BUFFER

#include <bitset>
#include <vector>

class BinaryBuffer {
private:
    int bufferSize;
    std::bitset<32> bitBuffer;

public:
    BinaryBuffer();

    bool push(std::vector<bool> bits);
    uint8_t pop();
    uint8_t peek() const;

    int size() const;
};

#endif
