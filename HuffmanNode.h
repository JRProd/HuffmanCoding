/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * The BinaryTree module handles the association of
 * huffman nodes to create the tree. Has convienence
 * functions to navigate through tree and
 *
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#ifndef HUFFMAN_NODE
    #define HUFFMAN_NODE

#include <cstddef>
#include <cstdint>

class HuffmanNode {
private:
    HuffmanNode* leftChild;
    HuffmanNode* rightChild;
    uint8_t byte;
    int frequency;
public:
    HuffmanNode(uint8_t byte, int frequency);
    HuffmanNode(uint8_t byte, int frequency, HuffmanNode* left, HuffmanNode* right);

    ~HuffmanNode();

    // Get functions
    bool isNode() const;
    uint8_t getByte() const;
    int getFrequency() const;
    const HuffmanNode* getLeftChild() const;
    const HuffmanNode* getRightChild() const;

    bool operator<(const HuffmanNode* rhs);
    bool operator>(const HuffmanNode* rhs);
};

#endif
