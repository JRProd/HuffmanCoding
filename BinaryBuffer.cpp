/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#include "BinaryBuffer.h"


BinaryBuffer::BinaryBuffer() {
    this->bufferSize = 0;
    bitBuffer.reset();
}

/* Adds bits given by vector to higher order bits in the bitBuffer.
 * Adds MSB of the vector to the MSB free in the bitBuffer.
*/
bool BinaryBuffer::push(std::vector<bool> bits) {
    // If the buffer would fillup return erro
    int bitsUsing = bits.size() + this->bufferSize;
    if(bitsUsing > 32) {
        return false;
    }

    // Starting at MSB of vector. If bit is 1, add it to MSB free in buffer
    //  e.g. <1,0,0,1,1,1> Take MSB (left most 1) and add to rightmost free bit
    //  of the buffer. Continue until the vector is empty
    for(int i = 0; i < bits.size(); i++) {
        bool bit = bits[i];
        // Ignore the 0 bits because the buffer is already zero
        if(bit) {
            bitBuffer |= (1<<(31-this->bufferSize));
        }
        this->bufferSize++;
    }
    return true;
}

uint8_t BinaryBuffer::pop() {
    // Get the top byte
    uint8_t byte = peek();

    // Remove top byte from buffer
    this->bitBuffer<<=8;
    // Change size of buffer
    this->bufferSize-=8;
    return byte;
}


uint8_t BinaryBuffer::peek() const {
    // Convert the buffer to 1 byte size, using the MS 8 bits
    std::bitset<32> byteSequence = this->bitBuffer >> 24;
    // Convert to long
    unsigned long byte = byteSequence.to_ulong();
    // Return as 1 byte
    return (uint8_t)byte;
}

int BinaryBuffer::size() const {
    return this->bufferSize;
}
