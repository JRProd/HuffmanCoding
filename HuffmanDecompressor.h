/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * The Decompression module handles the recreation of
 * the huffman tree, bit to byte encoding, and the
 * reconstruction of the source file from the encoded
 * file.
 *
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#ifndef HUFFMAN_DECOMPRESSOR
    #define HUFFMAN_DECOMPRESSOR

#include <map> // Used for byte to bit encoding
#include <stack> // Used to recreate the binary tree from file
#include <string>
#include <vector> // Vector of bools represents bit sequences

#include <iostream> // Used to prompt user of errors
#include <fstream> // Reading and writing form file

#include "HuffmanNode.h" // Binary tree nodes;

class HuffmanDecompressor {
private:
    HuffmanNode* huffmanTree;

    std::string inputFileName;
    std::ifstream inputFile;
    std::ofstream outputFile;

    std::map<uint8_t, std::vector<bool> > huffmanCoding;

    // Construction the tree from the file
    void readTreeFromFile();
    void codeHuffman(const HuffmanNode* root, std::vector<bool> sequence);


public:
    HuffmanDecompressor();
    HuffmanDecompressor(HuffmanNode* root);

    ~HuffmanDecompressor();

    int openSourceFile(std::string fileUrl);
    int openDestFile(std::string fileUrl);
    void decompress(std::string nameToCompressTo);

    std::map<uint8_t, std::vector<bool> > getCoding();
};

#endif
