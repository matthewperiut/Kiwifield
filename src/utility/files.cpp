#include "files.h"
#include "../lib/olcPixelGameEngine.h"
#include "../lib/svpng.h"
#include "../lib/lodepng.h"

#include <vector>
#include <iostream>
#include <string>

using namespace olc;

// Re-encodes a PNG with the smallest of several lodepng settings. Leaves the file alone on any error.
static void CompressPng(std::string filepath) {
    std::vector<unsigned char> image;
    unsigned w, h;
    std::vector<unsigned char> buffer;

    if (lodepng::load_file(buffer, filepath) != 0)
        return;
    if (lodepng::decode(image, w, h, buffer) != 0)
        return;
    buffer.clear();

    //Now encode as hard as possible with several filter types and window sizes

    lodepng::State state;
    state.encoder.filter_palette_zero = 0; //We try several filter types, including zero, allow trying them all on palette images too.
    state.encoder.add_id = false; //Don't add LodePNG version chunk to save more bytes
    state.encoder.text_compression = 1; //Not needed because we don't add text chunks, but this demonstrates another optimization setting
    state.encoder.zlibsettings.nicematch = 258; //Set this to the max possible, otherwise it can hurt compression
    state.encoder.zlibsettings.lazymatching = 1; //Definitely use lazy matching for better compression
    state.encoder.zlibsettings.windowsize = 32768; //Use maximum possible window size for best compression

    size_t bestsize = 0;
    bool inited = false;

    LodePNGFilterStrategy strategies[4] = { LFS_ZERO, LFS_MINSUM, LFS_ENTROPY, LFS_BRUTE_FORCE };

    // min match 3 allows all deflate lengths. min match 6 is similar to "Z_FILTERED" of zlib.
    int minmatches[2] = { 3, 6 };

    int autoconverts[2] = { 0, 1 };

    // Try out all combinations of everything
    for (int i = 0; i < 4; i++)   //filter strategy
        for (int j = 0; j < 2; j++)   //min match
            for (int k = 0; k < 2; k++)   //block type (for small images only)
                for (int l = 0; l < 2; l++) { //color convert strategy
                    if (bestsize > 3000 && (k > 0 || l > 0)) continue; /* these only make sense on small images */
                    std::vector<unsigned char> temp;
                    state.encoder.filter_strategy = strategies[i];
                    state.encoder.zlibsettings.minmatch = minmatches[j];
                    state.encoder.zlibsettings.btype = k == 0 ? 2 : 1;
                    state.encoder.auto_convert = autoconverts[l];
                    if (lodepng::encode(temp, image, w, h, state) != 0)
                        continue;

                    if (!inited || temp.size() < bestsize)
                    {
                        bestsize = temp.size();
                        temp.swap(buffer);
                        inited = true;
                    }
                }

    if (inited)
        lodepng::save_file(buffer, filepath);
}

void SaveSprite(Sprite* spr, std::string filepath) {
    FILE* fp = fopen(filepath.c_str(), "wb");
    if (!fp)
        return;

    // Pixels as RGBA bytes, row by row
    std::vector<unsigned char> pixels;
    pixels.reserve(spr->width * spr->height * 4);
    for (int y = 0; y < spr->height; y++)
        for (int x = 0; x < spr->width; x++)
        {
            Pixel color = spr->GetPixel(vi2d(x, y));
            pixels.push_back(color.r);
            pixels.push_back(color.g);
            pixels.push_back(color.b);
            pixels.push_back(color.a);
        }

    if (!pixels.empty())
        svpng(fp, spr->width, spr->height, pixels.data(), 1);
    fclose(fp);

    // Synchronous on purpose: a background compress could overwrite a newer save of the same file
    CompressPng(filepath);
}
