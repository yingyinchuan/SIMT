#include "FileCompressHuffman.hpp"

int main()
{
    FileCompressHM *C = new FileCompressHM();
    C->CompressHM("test.txt");

    FileCompressHM *UC = new FileCompressHM();
    UC->UnCompressHM("test.myzip");

    return 0;
}