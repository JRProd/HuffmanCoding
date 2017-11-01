/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * The Compression module handles the creation of the
 *  huffman tree, byte to bit encoding, and translation
 *  of source file to encoded .huff file.
 *
 * I made use of a circular bit buffer when writing to
 *  to the file. Because huffman coding does not assert
 *  a static code size from byte to bit, I needed to
 *  implement a buffer when writing to file.
 *
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#ifndef HUFFMAN_COMPRESSOR
    #define HUFFMAN_COMPRESSOR

#include <map> // Used for byte to bit encoding
#include <string>
#include <queue> // Used pQueue to generate huffman tree
#include <vector> // Vector of bools represents bit sequences less than 8 long

#include <iostream> // Used to prompt user of errors
#include <fstream> // Reading and writing form file

#include "HuffmanNode.h" // Binary tree nodes
#include "BinaryBuffer.h" // BinaryBuffer to convert bit sequences to bytes for writing to file

class HuffmanCompressor {
private:
    HuffmanNode* huffmanTree;
    BinaryBuffer bitBuffer;

    std::string inputFileName;
    std::ifstream inputFile;
    std::ofstream outputFile;

    std::map<uint8_t, std::vector<bool> > huffmanCoding;

    int openDestFile();

    // Construction functions to generate the huffman coding
    void generateTree();
    void codeHuffman(const HuffmanNode* root, std::vector<bool> sequence);

    void writeHuffmanTreeToFile(const HuffmanNode* root);

public:
    HuffmanCompressor();
    HuffmanCompressor(HuffmanNode* root);

    ~HuffmanCompressor();

    int openSourceFile(std::string fileUrl);
    float compress();

    std::map<uint8_t, std::vector<bool> > getCoding();
};

#endif
