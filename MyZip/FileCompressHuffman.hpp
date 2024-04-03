#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "HuffmanTree.hpp"

using namespace std;

struct ByteInfo
{
    unsigned char _ch;
    size_t _appearCount;
    string _chCode;
    ByteInfo(size_t appearCount = 0) : _ch(0), _appearCount(appearCount) {}

    ByteInfo operator+(const ByteInfo &other) const
    {
        return ByteInfo(_appearCount + other._appearCount);
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
    FileCompressHM();
    ~FileCompressHM();

private:
    void GenerateHuffmanCode(HuffmanTreeNode<ByteInfo> *root);
    void WriteHeadInfo(const string &filePath, ofstream &out);
    string GetFilePostFix(const string &filePath);
    void GetLine(ifstream &in, string &strInfo);
    vector<ByteInfo> _fileInfo;
};

FileCompressHM::FileCompressHM()
{
    _fileInfo.resize(256);
    for (int i = 0; i < 256; i++)
    {
        _fileInfo[i]._ch = i;
    }
}

FileCompressHM::~FileCompressHM() {}

void FileCompressHM::CompressHM(const string &filePath)
{
    ifstream in(filePath, ios::binary);
    if (!in)
        return;

    unsigned char buffer[1024];
    while (!in.eof())
    {
        in.read(reinterpret_cast<char *>(buffer), 1024);
        size_t size = in.gcount();
        for (size_t i = 0; i < size; i++)
        {
            _fileInfo[buffer[i]]._appearCount++;
        }
    }
    HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());

    GenerateHuffmanCode(ht.GetRoot());

    string fileName = "test.myzip";
    ofstream out(fileName, ios::binary);
    WriteHeadInfo(filePath, out);

    in.clear();
    in.seekg(0, ios::beg);
    unsigned char bits = 0;
    int bitcount = 0;

    while (!in.eof())
    {
        in.read(reinterpret_cast<char *>(buffer), 1024);
        size_t size = in.gcount();
        for (size_t i = 0; i < size; i++)
        {
            unsigned char ch = buffer[i];
            string &strCode = _fileInfo[ch]._chCode;
            for (size_t j = 0; j < strCode.size(); j++)
            {
                bits <<= 1;
                if (strCode[j] == '1')
                    bits |= 1;
                bitcount++;
                if (bitcount == 8)
                {
                    out.put(bits);
                    bits = 0;
                    bitcount = 0;
                }
            }
        }
    }

    if (bitcount != 0)
    {
        bits <<= (8 - bitcount);
        out.put(bits);
    }
}

void FileCompressHM::UnCompressHM(const string &filePath)
{
    if (GetFilePostFix(filePath) != "myzip")
    {
        cout << "Please use correct format." << endl;
        return;
    }

    ifstream in(filePath, ios::binary);
    if (!in)
    {
        cout << "File path does not exist." << endl;
        return;
    }

    string file_name = "unzip.";
    string strInfo;
    GetLine(in, strInfo);
    file_name += strInfo;
    GetLine(in, strInfo);
    size_t lineCount = stoi(strInfo);
    size_t fileSize = 0;
    for (size_t i = 0; i < lineCount; ++i)
    {
        GetLine(in, strInfo);
        if (strInfo.empty())
        {
            strInfo += '\n';
            GetLine(in, strInfo);
        }
        _fileInfo[strInfo[0]]._ch = strInfo[0];
        _fileInfo[strInfo[0]]._appearCount = stoi(strInfo.substr(2));
        fileSize += _fileInfo[strInfo[0]]._appearCount;
    }

    HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());
    ofstream out(file_name, ios::binary);
    unsigned char buffer[1024];
    HuffmanTreeNode<ByteInfo> *cur = ht.GetRoot();
    size_t decodedBytes = 0;
    while (decodedBytes < fileSize)
    {
        in.read(reinterpret_cast<char *>(buffer), 1024);
        size_t size = in.gcount();
        for (size_t i = 0; i < size && decodedBytes < fileSize; i++)
        {
            unsigned char bits = buffer[i];
            for (int j = 0; j < 8 && decodedBytes < fileSize; j++)
            {
                if (bits & 0x80)
                    cur = cur->_right;
                else
                    cur = cur->_left;
                bits <<= 1;
                if (cur->_left == nullptr && cur->_right == nullptr)
                {
                    out.put((cur->_weight)._ch);
                    cur = ht.GetRoot();
                    decodedBytes++;
                }
            }
        }
    }
}

void FileCompressHM::GenerateHuffmanCode(HuffmanTreeNode<ByteInfo> *root)
{
    if (root == nullptr)
        return;
    GenerateHuffmanCode(root->_left);
    GenerateHuffmanCode(root->_right);

    if (root->_left == nullptr && root->_right == nullptr)
    {
        string &chCode = _fileInfo[root->_weight._ch]._chCode;
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
        reverse(chCode.begin(), chCode.end());
    }
}

void FileCompressHM::WriteHeadInfo(const string &filePath, ofstream &out)
{
    string HeadInfo;
    HeadInfo += GetFilePostFix(filePath);
    HeadInfo += '\n';

    string chInfo;
    int chCount = 0;
    for (auto &e : _fileInfo)
    {
        if (e._appearCount == 0)
            continue;

        chInfo += e._ch;
        chInfo += ':';
        chInfo += to_string(e._appearCount);
        chInfo += '\n';

        chCount++;
    }
    HeadInfo += to_string(chCount);
    HeadInfo += '\n';

    out.write(HeadInfo.c_str(), HeadInfo.size());
    out.write(chInfo.c_str(), chInfo.size());
}

string FileCompressHM::GetFilePostFix(const string &filePath)
{
    return filePath.substr(filePath.find_last_of('.') + 1);
}

void FileCompressHM::GetLine(ifstream &in, string &strInfo)
{
    strInfo.clear();
    while (!in.eof())
    {
        char ch;
        in.get(ch);
        if (ch == '\n')
            break;
        strInfo += ch;
    }
}
