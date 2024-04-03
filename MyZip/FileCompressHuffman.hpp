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
    unsigned char _ch;
    size_t _appearCount;
    string _chCode;
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
    FileCompressHM();
    ~FileCompressHM();

private:
    void GenerateHuffmanCode(HuffmanTreeNode<ByteInfo> *root);
    void WriteHeadInfo(const string &filePath, FILE *out);
    string GetFilePostFix(const string &filePath);
    void GetLine(FILE *in, string &strInfo);
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

FileCompressHM::~FileCompressHM()
{
}

void FileCompressHM::CompressHM(const string &filePath)
{
    FILE *in = fopen(filePath.c_str(), "rb");
    if (in == nullptr)
        return;

    unsigned char buffer[1024];
    while (true)
    {
        size_t size = fread(buffer, 1, 1024, in);
        if (size == 0)
            break;
        for (size_t i = 0; i < size; i++)
        {
            _fileInfo[buffer[i]]._appearCount++;
        }
    }
    HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());

    GenerateHuffmanCode(ht.GetRoot());

    string fileName = "test.myzip";
    FILE *out = fopen(fileName.c_str(), "wb");
    WriteHeadInfo(filePath, out);

    fseek(in, 0, SEEK_SET);
    unsigned char bits = 0;
    int bitcount = 0;

    while (true)
    {
        size_t size = fread(buffer, 1, 1024, in);
        if (0 == size)
            break;

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
                    fputc(bits, out);
                    bits = 0;
                    bitcount = 0;
                }
            }
        }
    }

    if (bitcount != 0)
    {
        bits <<= (8 - bitcount);
        fputc(bits, out);
    }

    fclose(in);
    fclose(out);
}

void FileCompressHM::UnCompressHM(const string &filePath)
{
    if (GetFilePostFix(filePath) != "myzip")
    {
        std::cout << "请使用正确格式" << std::endl;
        return;
    }

    FILE *in = fopen(filePath.c_str(), "rb");
    if (in == nullptr)
    {
        std::cout << "文件路径不存在" << std::endl;
        return;
    }

    string file_name = "unzip.";
    string strInfo;
    GetLine(in, strInfo);
    file_name += strInfo;
    GetLine(in, strInfo);
    size_t lineCount = atoi(strInfo.c_str()); // 获取记录字符出现次数的行数
    size_t fileSize = 0;                      // 初始化文件大小
    for (size_t i = 0; i < lineCount; ++i)
    {
        GetLine(in, strInfo);
        if (strInfo.empty())
        {
            strInfo += '\n';
            GetLine(in, strInfo);
        }
        _fileInfo[strInfo[0]]._ch = strInfo[0];
        _fileInfo[strInfo[0]]._appearCount = atoi(strInfo.c_str() + 2);
        fileSize += _fileInfo[strInfo[0]]._appearCount; // 更新文件大小
    }

    // 重新构建哈夫曼树
    HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());
    // 解压缩
    FILE *out = fopen(file_name.c_str(), "wb");
    unsigned char buffer[1024];
    HuffmanTreeNode<ByteInfo> *cur = ht.GetRoot();
    size_t decodedBytes = 0; // 记录已解压的字节数
    while (decodedBytes < fileSize)
    {
        size_t size = fread(buffer, 1, 1024, in);
        if (size == 0)
            break;

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
                    fputc((cur->_weight)._ch, out);
                    cur = ht.GetRoot();
                    decodedBytes++;
                }
            }
        }
    }

    fclose(in);
    fclose(out);
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
        std::reverse(chCode.begin(), chCode.end());
    }
}

void FileCompressHM::WriteHeadInfo(const string &filePath, FILE *out)
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
        chInfo += std::to_string(e._appearCount);
        chInfo += '\n';

        chCount++;
    }
    HeadInfo += std::to_string(chCount);
    HeadInfo += '\n';

    fwrite(HeadInfo.c_str(), 1, HeadInfo.size(), out);
    fwrite(chInfo.c_str(), 1, chInfo.size(), out);
}

string FileCompressHM::GetFilePostFix(const string &filePath)
{
    return filePath.substr(filePath.find_last_of('.') + 1);
}

void FileCompressHM::GetLine(FILE *in, string &strInfo)
{
    strInfo.clear();
    while (!feof(in))
    {
        unsigned char ch = fgetc(in);
        if (ch == '\n')
            break;
        strInfo += ch;
    }
}