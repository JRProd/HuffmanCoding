/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#include "HuffmanDecompressor.h"

HuffmanDecompressor::HuffmanDecompressor() {
}

HuffmanDecompressor::HuffmanDecompressor(HuffmanNode* root) {
    this->huffmanTree = root;
}

HuffmanDecompressor::~HuffmanDecompressor() {
    if(this->inputFile.is_open()) {
        this->inputFile.close();
    }
    if(this->outputFile.is_open()) {
        this->outputFile.close();
    }
    delete huffmanTree;
}

void HuffmanDecompressor::readTreeFromFile() {
    std::stack<HuffmanNode* > treeBuilding;

    do {
        //Get the node and the byte for that node
        uint8_t isNode = this->inputFile.get();
        uint8_t byte = this->inputFile.get();
        if(isNode == '\n' && byte == '\n') { // If reached deliniation of tree/file break
            break;
        } else if(isNode) {
            // Remove top two nodes form stack
            HuffmanNode* right = treeBuilding.top();
            treeBuilding.pop();
            HuffmanNode* left = treeBuilding.top();
            treeBuilding.pop();
            // Create new node with with children
            HuffmanNode* parent = new HuffmanNode(0, 0, left, right);
            // Add node to stack
            treeBuilding.push(parent);
        }
        else {
            uint32_t frequency = 0;
            // If the byte found represents the EOF
            if(byte == 255) {
                // Read the 4 byte freuquency of EOF
                this->inputFile.read(reinterpret_cast<char *>(&frequency), sizeof(frequency));
            }
            // Create new leaf node with frequency = 0 or EOF's frequency
            HuffmanNode* leaf = new HuffmanNode(byte, frequency);
            // Add node to stack
            treeBuilding.push(leaf);
        }
    } while (this->inputFile.good());

    // The top node on stack is the root
    this->huffmanTree = treeBuilding.top();
}

// Recursive code creation. Left is 0, right is 1
void HuffmanDecompressor::codeHuffman(const HuffmanNode* root, std::vector<bool> sequence) {
    // If the current root does not exist
    if(root == nullptr) {
        return;
    }
    // If the current root is a node
    if(root->isNode()) {
        // Build left side vector and continue on left side
        sequence.push_back(false);
        this->codeHuffman(root->getLeftChild(), sequence);
        sequence.pop_back();
        // Remove left side and build right side vector
        sequence.push_back(true);
        // Continue on right side
        this->codeHuffman(root->getRightChild(), sequence);
        sequence.pop_back();
    } else  { // If the current root is a leaf
        // Add the byte to bit mapping
        this->huffmanCoding[root->getByte()] = sequence;
    }
}

int HuffmanDecompressor::openSourceFile(std::string fileUrl) {
    // Define the source file to write the encoded file to
    this->inputFileName = fileUrl.substr(0, fileUrl.find('.'));
    this->inputFile.open(fileUrl, std::ios::binary);
    // Check if file opened successfully
    if(this->inputFile.good()) {
        return 1;
    }
    return 0;
}

int HuffmanDecompressor::openDestFile(std::string fileUrl) {
    // If no fileUrl is given, assume extention is .txt
    if(fileUrl.empty()) {
        this->outputFile.open(this->inputFileName + ".txt", std::ios::binary);
    } else {
        this->outputFile.open(fileUrl, std::ios::binary);
    }
    // Check if file opened successfully
    if(this->outputFile.good()) {
        return 1;
    }
    return 0;
}

// Decompress the file from the huffman coding
//  representation to the source representation
void HuffmanDecompressor::decompress(std::string nameToDecompressTo) {
    // Generate the huffman coding
    this->readTreeFromFile();
    std::vector<bool> sequence;
    this->codeHuffman(this->huffmanTree, sequence);

    // Open the destination file
    if(!(this->openDestFile(nameToDecompressTo))) {
        std::cerr << "Destination file failed to open\n";
        return;
    }

    const HuffmanNode* lookupNode = this->huffmanTree;
    // Define controls for the EOF character
    int eofsUsed = 0;
    bool firstEof = true;
    bool eofReached = false;
    do {
        // Get byte form file
        uint8_t byte = this->inputFile.get();
        for(int i = 0; i < 8; i++) { // Look through each bit of the byte
            if(lookupNode == nullptr) {
                break;
            }
            // Get the MSB of the byte
            if(byte & 0b10000000) {
                // 1 is Right
                lookupNode = lookupNode->getRightChild();
            } else {
                // 0 is Left
                lookupNode = lookupNode->getLeftChild();
            }
            // Move next MSB to MSB position
            byte<<=1;
            // If the next lookupNode is not a Node, aka leaf
            if(!lookupNode->isNode()) {
                // Handler for EOF leaf
                if(lookupNode->getByte() == 255) {
                    if(firstEof) {  // First time reaching leaf
                        // Get the frequency
                        // Toggle the firstEof controller
                        eofsUsed = lookupNode->getFrequency();
                        firstEof = !firstEof;
                    }
                    // If this is the last EOF
                    if(!(eofsUsed > 1)) {
                        // Toggle end of file control
                        eofReached = !eofReached;
                        break;
                    }
                    // Decrement number of EOF's remaining in file
                    eofsUsed--;
                }
                // Put the byte represented by the bit sequence into the file
                this->outputFile.put(lookupNode->getByte());
                // Go back to root of tree
                lookupNode = this->huffmanTree;
            }
        }
    } while (this->inputFile.good() && !eofReached);
}

std::map<uint8_t, std::vector<bool> > HuffmanDecompressor::getCoding() {
    return huffmanCoding;
}
