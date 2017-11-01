/* ########## HUFFMAN COMPRESSION APPLICATION ##########
 * The UI module handles the user i/o requirements.
 *  The user is able to Compress and Decompress files
 *  by entering commands into the terminal
 *
 * Aurthor - Jake Rowland
 * Date - October 31, 2107
*/

#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"

#include <string>

void compress(std::string fileUrl) {
    HuffmanCompressor c;

    if (!(c.openSourceFile(fileUrl))) {
        std::cerr << "< Error opening the source file\n";
        return;
    }

    float compresion = c.compress();

    std::cout << "< File compressed by " << compresion*100 << "%\n";
}

void decompress(std::string fileUrl, std::string fileUrlDecompressed) {
    HuffmanDecompressor d;

    if(!(d.openSourceFile(fileUrl))) {
        std::cerr << "< Error opening the source file\n";
        return;
    }

    d.decompress(fileUrlDecompressed);

    std::cout << "< File decompressed\n";
}

int main(int argc, char const *argv[]) {
    std::string mode = "c";
    std::cout << "< ##################################################\n";
    std::cout << "< ##         Huffman Compression Application      ##\n";
    std::cout << "< ##           (C)ompress and (D)ecompress        ##\n";
    std::cout << "< ##                                              ##\n";
    std::cout << "< ##                 By Jake Rowland              ##\n";
    std::cout << "< ##################################################\n";
    std::cout << "< Enter a command and file to de/compress.\n";
    std::cout << "< Optionally add file to de/compress to\n";
    std::cout << "< (Q)uit to leave program\n<\n";


    do {
        std::string cmd;
        std::cout << "> ";
        getline(std::cin, cmd, '\n');

        std::string action = cmd.substr(0, cmd.find(' ')); //Get action. Up to first space
        std::string fileSource = cmd.substr(cmd.find(' ') + 1, cmd.length()); // Get the file/files to use
        std::string fileDestination = ""; // Default destination file.
        if(fileSource.find(' ') != std::string::npos) { // If space in files split to two.
            fileDestination = fileSource.substr(fileSource.find(' ') + 1, fileSource.length()); // Get second file
            fileSource = fileSource.substr(0, fileSource.find(' ')); // Remove second file from source string
        }

        // Check if file with spaces was entered
        if(fileDestination.find(' ') != std::string::npos) {
            std::cout << "< Files cannot contain spaces. Please try again\n";
            continue;
        }

        if(action.compare("Q") == 0) {
            break;
        } else if(action.compare("C") == 0) {
            compress(fileSource);
        } else if(action.compare("D") == 0) {
            std::string ext = fileSource.substr(fileSource.find('.') + 1, fileSource.length());
            if(ext.compare("huff") != 0) {
                std::cout << "< File for decompression must have the .huff extention\n";
                continue;
            }
            decompress(fileSource, fileDestination);
        } else {
            std::cout << "< Command " << action << " not recognized!\n";
            std::cout << "< Please use (C)ompress, (D)ecompress, or (Q)uit\n";
        }
    } while(true);

    std::cout << "< Bye!\n";
    return 0;
}
