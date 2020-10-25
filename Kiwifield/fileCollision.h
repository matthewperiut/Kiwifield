#pragma once
#include <fstream>
#include <vector>

using namespace std;

#define uchar unsigned char

// Conversion between 256 value and 8 bit correspondant
class ByteFunc
{
public:
    static uchar toChar(bool a[8]);
    static uchar toChar(vector<bool> a);
    static void toBinary(int n, bool* result);
};

class FileCollision
{
public:
    static void SaveVector(vector<vector<bool>> collision, string filepath);
    static vector<vector<bool>> LoadVector(int width, int height, string path);
};