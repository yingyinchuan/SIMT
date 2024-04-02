#include <iostream>
#include <vector>
#include <string>
#include <cstddef>
#include <algorithm>
#include "HuffmanTree.hpp"

using std::string;
using std::vector;

struct ByteInfo
{
    char _ch;
    size_t _appearCount;
    string _chCount;
    ByteInfo(size_t appearCount = 0)
        : _appearCount(appearCount)
    {
    }

    ByteInfo operator+(const ByteInfo &other) const
    {
        return _appearCount + other._appearCount;
    }

    bool operator>(const ByteInfo &other) const
    {
        return _appearCount > other._appearCount;
    }

    bool operator!=(const ByteInfo &other) const
    {
        return _appearCount != other._appearCount;
    }

    bool operator==(const ByteInfo &other) const
    {
        return _appearCount == other._appearCount;
    }
};

class FileCompressHM
{
public:
    void CompressHM(const string &filePath);
    void UnCompressHM(const string &filePath);
    void GenerateHuffmanCode(HuffmanTreeNode<ByteInfo> *root);
    vector<ByteInfo> _fileInfo;

    FileCompressHM(/* args */);
    ~FileCompressHM();
};

FileCompressHM::FileCompressHM()
{
    _fileInfo.resize(256);
    for (int i = 0; i < 256; i++)
    {
        _fileInfo[i]._ch = i;
    }
}

FileCompressHM::~FileCompressHM()
{
}

void FileCompressHM::CompressHM(const string &filePath)
{
    FILE *in = fopen(filePath.c_str(), "r");
    if (in)
        return;

    char buffer[1024];
    while (true)
    {
        size_t size = fread(buffer, 1, 1024, in);
        if (size == 0)
            break;
        for (int i = 0; i < size; i++)
        {
            _fileInfo[buffer[i]]._appearCount++;
        }
    }
    ByteInfo invalid;
    HuffmanTree<ByteInfo> ht(_fileInfo, invalid);
}

void FileCompressHM::UnCompressHM(const string &filePath)
{
}

void FileCompressHM::GenerateHuffmanCode(HuffmanTreeNode<ByteInfo> *root)
{
    if (root == nullptr)
        return;
    GenerateHuffmanCode(root->_left);
    GenerateHuffmanCode(root->_right);

    if (root->_left == nullptr && root->_right == nullptr)
    {
        string &chCode = _fileInfo[root->_weight._ch]._chCount;
        HuffmanTreeNode<ByteInfo> *cur = root;
        HuffmanTreeNode<ByteInfo> *parent = cur->_parent;

        while (parent)
        {
            if (cur == parent->_left)
                chCode += "0";
            else if (cur == parent->_right)
                chCode += "1";

            cur = parent;
            parent = cur->_parent;
        }
        std::reverse(chCode.begin(), chCode.end());
    }
}
