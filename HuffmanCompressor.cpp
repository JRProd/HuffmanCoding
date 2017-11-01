/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#include "HuffmanCompressor.h"

HuffmanCompressor::HuffmanCompressor() {
}

HuffmanCompressor::HuffmanCompressor(HuffmanNode* root) {
    this->huffmanTree = root;
}

// Deallocate dynamic memory and close file streams
HuffmanCompressor::~HuffmanCompressor() {
    if(this->inputFile.is_open()) {
        this->inputFile.close();
    }
    if(this->outputFile.is_open()) {
        this->outputFile.close();
    }
    delete huffmanTree;
}

void HuffmanCompressor::generateTree() {
    // Generate the frequency for each byte
    std::map<uint8_t, int> frequency;
    do {
        uint8_t byte = this->inputFile.get();
        if(frequency[byte] != 0) {
            frequency[byte] ++;
        } else {
            frequency[byte] = 1;
        }
    } while (this->inputFile.good());

    // Define comparison between Huffman nodes for priority_queue
    auto cmp = [](HuffmanNode* left, HuffmanNode* right) {
            return left->getFrequency() > right->getFrequency();
        };
    // Add Huffman Nodes to queue
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(cmp)> pQueue(cmp);
    for(std::map<uint8_t, int>::const_iterator it = frequency.begin(); it != frequency.end(); ++it) {
        pQueue.push(new HuffmanNode(it->first, it->second));
    }

    // Define byte to represent nodes
    const uint8_t combined = 0;
    while(pQueue.size() > 1) {
        // Get the top two Huffman Nodes of the queue
        HuffmanNode* leftChild = pQueue.top();
        pQueue.pop();
        HuffmanNode* rightChild = pQueue.top();
        pQueue.pop();
        // Create parent with those two nodes as children
        HuffmanNode* parent = new HuffmanNode(combined, leftChild->getFrequency() + rightChild->getFrequency(), leftChild, rightChild);
        // Add parent to queue
        pQueue.push(parent);
    }

    // The last element on the queue is the root node
    this->huffmanTree = pQueue.top();

    // Reset input stream to begining of file
    this->inputFile.clear();
    this->inputFile.seekg(0, std::ios::beg);
}

// Recursive code creation. Left is 0, right is 1
void HuffmanCompressor::codeHuffman(const HuffmanNode* root, std::vector<bool> sequence) {
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

// Recursive post order binary tree reading. Write bytes to the file
void HuffmanCompressor::writeHuffmanTreeToFile(const HuffmanNode* root) {
    if(root == nullptr) {
        return;
    }
    // Post order
    this->writeHuffmanTreeToFile(root->getLeftChild());
    this->writeHuffmanTreeToFile(root->getRightChild());
    // Write the node state to 2 bytes in the file.
    //  First byte is 0-1 if the root is a node
    //  Second byte is the byte of the node. 0b00000000 if root is node
    this->outputFile.put(root->isNode());
    this->outputFile.put(root->getByte());

    // Special case if root is my EOF representation
    if(root->getByte() == 255) {
        // Some files use EOF byte more than once. Store frequency in 4 bytes
        int frequency = root->getFrequency();
        this->outputFile.write(reinterpret_cast<const char *>(&frequency), sizeof(frequency));
    }

}

int HuffmanCompressor::openSourceFile(std::string fileUrl) {
    // Define the source file to write the encoded file to
    this->inputFileName = fileUrl.substr(0, fileUrl.find('.'));
    this->inputFile.open(fileUrl, std::ios::binary);
    // Check if file opened successfully
    if(this->inputFile.good()) {
        return 1;
    }
    return 0;
}

int HuffmanCompressor::openDestFile() {
    // Open file with source file name and .huff extention
    this->outputFile.open(this->inputFileName + ".huff", std::ios::binary);
    // Checck if file opened successfully
    if(this->outputFile.good()) {
        return 1;
    }
    return 0;
}

// Compress file using huffman encoding.
//  Return the ratio of compression.
float HuffmanCompressor::compress() {
    // Generate the huffman coding
    this->generateTree();
    std::vector<bool> sequence;
    this->codeHuffman(this->huffmanTree, sequence);

    // Open the destination file
    if(!(this->openDestFile())) {
        std::cerr << "Destination file failed to open\n";
        return 0.0;
    }

    // Write the tree encoding to file. Separate tree and and data with newline
    this->writeHuffmanTreeToFile(huffmanTree);
    // Deliniate tree and file encoding
    this->outputFile.put('\n');
    this->outputFile.put('\n');

    int inputFileSize = 0;
    int outputFileSize = (huffmanCoding.size()*2) + 6; // Defined by size of tree writen to file plus two newlines
    // While there are still bytes in file
    do {
        uint8_t byte = this->inputFile.get();
        inputFileSize += sizeof(uint8_t);
        // Add the encoded byte to buffer
        if(!(this->bitBuffer.push(huffmanCoding[byte]))) {
            std::cout<< "Error ocurred adding bit sequence to bit bufffer\n";
            break;
        } else {
            // If the bits create a full byte
            while(this->bitBuffer.size() >= 8) {
                // Write the byte to file
                uint8_t byte = this->bitBuffer.pop();
                this->outputFile.put(byte);
                outputFileSize++;
            }
        }
    } while (this->inputFile.good());

    // Get the last < 8 bits as a byte to finish off the file.
    if(this->bitBuffer.size() > 0) {
        uint8_t byte = this->bitBuffer.peek();
        this->outputFile.put(byte);
        outputFileSize++;
    }

    // Return the compression ratio
    return (float)(inputFileSize-outputFileSize)/inputFileSize;
}

std::map<uint8_t, std::vector<bool> > HuffmanCompressor::getCoding() {
    return huffmanCoding;
}
