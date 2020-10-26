#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

struct HFMNTree
{
    char symbol = 0;
    bool hasSymbol = false;
    HFMNTree * zero = nullptr;
    HFMNTree * one = nullptr;

};

class HFMNCode
{
public:
    HFMNCode( std::vector<int> codeLens, int maxCodeLen, char startingSymbol = 1)
    {
        //create HFMNTree
        HFMNTree * tree = new HFMNTree();
        HFMNTree * currentNode = tree;
        for(int i = 0; i < maxCodeLen - 1; i++) {
            currentNode->one = new HFMNTree();
            currentNode->zero = new HFMNTree();
            currentNode = currentNode->one;
        }
        int nHop = 0;
        for(int clen : codeLens) {
            currentNode = tree;
            for(int i = 0; i < clen - 1; i++ )
                currentNode = currentNode->one;
            if(!currentNode->zero->hasSymbol) {
                currentNode->zero->hasSymbol = true;
                currentNode->zero->symbol = startingSymbol;
            }
            else {
                currentNode->one->hasSymbol = true;
                currentNode->one->symbol = startingSymbol;
            }
            startingSymbol ++;
        }
        _codeTree = tree;
        //Create code lookup table
        _codeStrMap = {};
        traverseCreateMap(_codeTree);
       for(auto m : _codeStrMap)
               std::cout << m.first << "=>" << m.second << std::endl;
       std::cout << "-------------" << std::endl;
    }

    void printCodes()
    {
        traversePrint(_codeTree);
    }

    std::string decompress( const char * inputBuffer, unsigned int length, int padding )
    {
        std::string outputBuffer = "";
        HFMNTree * currentNode = _codeTree;
        for (int i = 0; i < length; i++) {
            unsigned char inputByte = inputBuffer[i];
            for ( int i = 7; i > 0 ; i--) {
                bool state = ( inputByte >> i ) & 1;
                std::cout << (state ? '1' : '0') << std::endl;
                if ( state && currentNode->one )
                        currentNode = currentNode->one;
                else if ( (!state) && currentNode->zero )
                        currentNode = currentNode->zero;
                else {
                    char symbol = currentNode->symbol;
                    outputBuffer += symbol;
                    currentNode = state ? _codeTree->one : _codeTree->zero;
                }
            }       
        }
        for( int i = 0; i < padding; i++)
            outputBuffer.pop_back();
        return outputBuffer;

    }

    std::string compress( const char * inputBuffer, unsigned int length, int &padding )
    {
        padding = 0;
        std::string outputBuffer = "";
        int bitNo = 7;
        char currentByte = 0;
        for (int i = 0; i < length; i++ ){
            std::string codeStr = _codeStrMap[inputBuffer[i]];
            for(char c : codeStr) {
                char bitMask = (char)1 << bitNo;
                if (c == '1') currentByte |= bitMask;
                //std::cout << c << std::endl;
                bitNo--;
                if(bitNo == 0){
                    std::cout << "current byte " << std::hex << (int)currentByte << std::endl;

                    bitNo = 7;
                    outputBuffer += currentByte;
                    currentByte = 0;
                } 
            }
        }
        padding = bitNo - 1;
        outputBuffer += currentByte;
        return outputBuffer;
    }


private:
    void traversePrint(HFMNTree * node, std::string code = "")
    {
        if(node->hasSymbol)
            std::cout << code << " -> " << node->symbol << std::endl;
        if(node->zero)
            traversePrint(node->zero, code + '0');
        if(node->one)
            traversePrint(node->one,code + '1');

    }
    void traverseCreateMap(HFMNTree * node, std::string code = "")
    {
        if(node->hasSymbol)
            _codeStrMap[node->symbol] = code;
        if(node->zero)
            traverseCreateMap(node->zero, code + '0');
        if(node->one)
            traverseCreateMap(node->one,code + '1');

    }
private:
    HFMNTree * _codeTree;
    std::unordered_map<char, std::string> _codeStrMap;
};


int main()
{
    std::vector<int> lens = {2, 1, 3, 3};
    HFMNCode code(lens, 4, 'A');
    code.printCodes();
    std::string testData = "ABCD";
    int padding;
    std::string encodedData = code.compress(testData.c_str(), testData.length(), padding);
    std::string decompressedData = code.decompress(encodedData.c_str(), encodedData.length(), padding);
    std::cout << "padding : " << padding << std::endl;
    std::cout << encodedData << " ==> " << decompressedData << std::endl;
    return 0;
}
