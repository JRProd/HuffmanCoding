/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#include "HuffmanNode.h"

HuffmanNode::HuffmanNode(uint8_t byte, int frequency) {
    this->byte = byte;
    this->frequency = frequency;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
}

HuffmanNode::HuffmanNode(uint8_t byte, int frequency, HuffmanNode* left, HuffmanNode* right) {
    this->byte = byte;
    this->frequency = frequency;
    this->leftChild = left;
    this->rightChild = right;
}

HuffmanNode::~HuffmanNode() {
    if(leftChild != nullptr) {
        delete leftChild;
    }
    if(rightChild != nullptr) {
        delete rightChild;
    }
}

// If the HuffmanNode has children then its a node
bool HuffmanNode::isNode() const {
    if(this->leftChild) {
        return true;
    } else if (this->rightChild) {
        return true;
    } else {
        return false;
    }
}

uint8_t HuffmanNode::getByte() const{
    return this->byte;
}

int HuffmanNode::getFrequency() const{
    return this->frequency;
}

const HuffmanNode* HuffmanNode::getLeftChild() const{
    return this->leftChild;
}

const HuffmanNode* HuffmanNode::getRightChild() const{
    return this->rightChild;
}

bool HuffmanNode::operator<(const HuffmanNode* rhs) {
    return this->frequency < rhs->frequency;
}

bool HuffmanNode::operator>(const HuffmanNode* rhs) {
    return this->frequency > rhs->frequency;
}
