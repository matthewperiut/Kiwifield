#include "fileCollision.h"

uchar ByteFunc::toChar(bool a[8])
{
    uchar c = 0;
    for (int i = 0; i < 8; i++)
    {
        int j = 7 - i;
        c += a[i] * (pow(2, j));
    }
    return c;
}
uchar ByteFunc::toChar(vector<bool> a)
{
    uchar c = 0;
    for (int i = 0; i < 8; i++)
    {
        int j = 7 - i;
        c += a[i] * (pow(2, j));
    }
    return c;
}
void ByteFunc::toBinary(int n, bool* result)
{
    int pos = 7;
    while (n != 0)
    {
        result[pos] = (n % 2 == 0 ? 0 : 1);
        n /= 2;
        pos -= 1;
    }
    for (; pos > -1; pos--)
    {
        result[pos] = 0;
    }
}

void FileCollision::SaveVector(vector<vector<bool>> collision, string filepath)
{
    ofstream path;
    path.open(filepath, ios::out | ios::binary);
    bool buff[8]{ 0 };
    int count = 0;

    for (int y = 0; y < collision.size(); y++)
    {
        for (int x = 0; x < collision[y].size(); x++)
        {
            buff[count] = collision.at(y).at(x);
            count++;
            if (count > 7)
            {
                count = 0;
                uchar c = ByteFunc::toChar(buff);
                path.write((char*)&c, sizeof(char));
            }
        }
    }
    while (count != 0)
    {
        buff[count] = 0;
        count++;
        if (count > 7)
        {
            count = 0;
            uchar c = ByteFunc::toChar(buff);
            path.write((char*)&c, sizeof(char));
        }
    }
}
vector<vector<bool>> FileCollision::LoadVector(int width, int height, string path)
{
    vector<vector<bool>> result;
    ifstream input(path, ios::in);

    uchar c;
    int x = 0;
    int y = 0;
    bool buff[8]{ 0 };
    result.push_back({});
    while (input >> c)
    {
        ByteFunc::toBinary(int(c), buff);
        for (int i = 0; i < 8; i++)
        {
            if (!buff[i])
            {
                result[y].push_back(false);
                x++;
            }
            else if (buff[i])
            {
                result[y].push_back(true);
                x++;
            }
            if (x == width)
            {
                x = 0;
                y++;
                result.push_back({});
                continue;
            }
        }
    }
    input.close();
    return result;
}