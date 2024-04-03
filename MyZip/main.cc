#include "FileCompressHuffman.hpp"

int main()
{
    FileCompressHM compressor;

    compressor.CompressHM("input.txt");

    compressor.UnCompressHM("test.mz");

    return 0;
}
