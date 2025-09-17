#include "Display.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace hidrometro
{
    Display::Display() 
    : baseW(0), baseH(0) 
{}

void Display::setBaseImage(const std::string& path)
{
    base.clear();
    baseW = baseH = 0;

    if (path.empty()) return;

    int w=0, h=0, comp=0;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &comp, 3);
    if (!data) return;

    base.assign(data, data + (w*h*3));
    baseW = w;
    baseH = h;
    stbi_image_free(data);
}

static bool glyph5x7(char c, unsigned char out[5])
{
if (c == ' ') { out[0]=out[1]=out[2]=out[3]=out[4]=0x00; return true; }

static const unsigned char DIG[10][5] = 
{
    {0x3E,0x51,0x49,0x45,0x3E}, 
    {0x00,0x42,0x7F,0x40,0x00}, 
    {0x42,0x61,0x51,0x49,0x46}, 
    {0x21,0x41,0x45,0x4B,0x31}, 
    {0x18,0x14,0x12,0x7F,0x10}, 
    {0x27,0x45,0x45,0x45,0x39}, 
    {0x3C,0x4A,0x49,0x49,0x30}, 
    {0x01,0x71,0x09,0x05,0x03}, 
    {0x36,0x49,0x49,0x49,0x36}, 
    {0x06,0x49,0x49,0x29,0x1E}  
};
if (c >= '0' && c <= '9') {
    const unsigned char* g = DIG[c - '0'];
    for (int i=0;i<5;++i) out[i]=g[i];
    return true;
}

switch (c)
{
    case 'O': 
    { 
        unsigned char g[5]={0x3E,0x41,0x41,0x41,0x3E}; for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'D': 
    {
        unsigned char g[5]={0x7F,0x41,0x41,0x22,0x1C}; for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'M': 
    {
        unsigned char g[5]={0x7F,0x02,0x0C,0x02,0x7F}; for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'E': 
    {
        unsigned char g[5]={0x7F,0x49,0x49,0x49,0x41}; for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'T': 
    {
        unsigned char g[5]={0x01,0x01,0x7F,0x01,0x01}; for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'R': 
    {
        unsigned char g[5]={0x7F,0x09,0x19,0x29,0x46}; for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'L': 
    {
        unsigned char g[5]={0x7F,0x40,0x40,0x40,0x40}; for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }

}

switch (c)
{
    case '[': 
    { unsigned char g[5]={0x00,0x7F,0x41,0x41,0x00}; for(int i=0;i<5;++i) out[i]=g[i]; 
    return true; 
    }
    case ']': 
    { unsigned char g[5]={0x00,0x41,0x41,0x7F,0x00}; for(int i=0;i<5;++i) out[i]=g[i]; 
    return true; 
    }
    case '=': 
    { unsigned char g[5]={0x14,0x14,0x14,0x14,0x14}; for(int i=0;i<5;++i) out[i]=g[i]; 
    return true; 
    }
    case '.': 
    { unsigned char g[5]={0x00,0x40,0x00,0x00,0x00}; for(int i=0;i<5;++i) out[i]=g[i]; 
    return true; 
    }
    case ':': 
    { unsigned char g[5]={0x00,0x36,0x36,0x00,0x00}; for(int i=0;i<5;++i) out[i]=g[i]; 
    return true; 
    }

    case 'S': 
    {
        unsigned char g[5] = {0x46,0x49,0x49,0x49,0x31};
        for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'H': 
    {
        unsigned char g[5] = {0x7F,0x08,0x08,0x08,0x7F};
        for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'A': 
    {
        unsigned char g[5] = {0x7E,0x09,0x09,0x09,0x7E};
        for(int i=0;i<5;++i) out[i]=g[i]; 
        return true; 
    }
    case 'V':
    {  

    unsigned char g[5] = {0x07, 0x38, 0x40, 0x38, 0x07};
    for (int i = 0; i < 5; ++i) out[i] = g[i];
    return true;
    }
}

out[0]=out[1]=out[2]=out[3]=out[4]=0x00;
    return false;
}

void putPixelRGB(std::vector<unsigned char>& img, int W, int H, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    if (x < 0 || y < 0 || x >= W || y >= H) return;
    int idx = (y*W + x) * 3;
    img[idx+0] = r;
    img[idx+1] = g;
    img[idx+2] = b;
}

static void fillRect(std::vector<unsigned char>& img, int W, int H,
                     int x, int y, int w, int h,
                     unsigned char r, unsigned char g, unsigned char b)
{
    if (w <= 0 || h <= 0) return;
    int x2 = x + w, y2 = y + h;
    if (x < 0) x = 0; if (y < 0) y = 0;
    if (x2 > W) x2 = W; if (y2 > H) y2 = H;
    for (int yy = y; yy < y2; ++yy)
        for (int xx = x; xx < x2; ++xx)
            putPixelRGB(img, W, H, xx, yy, r, g, b);
}

static void strokeRect(std::vector<unsigned char>& img, int W, int H,
                       int x, int y, int w, int h,
                       unsigned char r, unsigned char g, unsigned char b)
{
    if (w <= 0 || h <= 0) return;
    for (int xx = x; xx < x + w; ++xx) {
        putPixelRGB(img, W, H, xx, y, r, g, b);
        putPixelRGB(img, W, H, xx, y + h - 1, r, g, b);
    }
    for (int yy = y; yy < y + h; ++yy) {
        putPixelRGB(img, W, H, x, yy, r, g, b);
        putPixelRGB(img, W, H, x + w - 1, yy, r, g, b);
    }
}

static void fillCircle(std::vector<unsigned char>& img, int W, int H,
                       int cx, int cy, int rad,
                       unsigned char r, unsigned char g, unsigned char b)
{
    if (rad <= 0) return;
    int r2 = rad * rad;
    for (int y = -rad; y <= rad; ++y) {
        for (int x = -rad; x <= rad; ++x) {
            if (x*x + y*y <= r2)
                putPixelRGB(img, W, H, cx + x, cy + y, r, g, b);
        }
    }
}

static void ring(std::vector<unsigned char>& img, int W, int H,
                 int cx, int cy, int outerR, int thickness,
                 unsigned char r, unsigned char g, unsigned char b)
{
    if (outerR <= 0 || thickness <= 0) return;
    int innerR = outerR - thickness;
    int out2 = outerR*outerR;
    int in2  = (innerR>0? innerR*innerR : 0);
    for (int y = -outerR; y <= outerR; ++y)
        for (int x = -outerR; x <= outerR; ++x) {
            int d2 = x*x + y*y;
            if (d2 <= out2 && d2 >= in2)
                putPixelRGB(img, W, H, cx + x, cy + y, r, g, b);
        }
}


void drawChar(std::vector<unsigned char>& img, int W, int H, int x, int y, char c, int scale, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char col[5];
    glyph5x7(c, col);

    for (int cx=0; cx<5; ++cx)
    {
        unsigned char column = col[cx];
        for (int cy=0; cy<7; ++cy)
        {
            bool on = (column >> cy) & 0x01;
            if (on)
            {
                for (int sx=0; sx<scale; ++sx)
                for (int sy=0; sy<scale; ++sy)
                    putPixelRGB(img, W, H, x + cx*scale + sx, y + cy*scale + sy, r, g, b);
            }
        }
    }
}

void drawTextLine(std::vector<unsigned char>& img, int W, int H, int x, int y, const std::string& text, int scale, unsigned char r, unsigned char g, unsigned char b)
{
    int cursor = x;
    for (size_t i=0;i<text.size();++i)
    {
        drawChar(img, W, H, cursor, y, text[i], scale, r, g, b);
        cursor += (5*scale) + scale; 
    }
}

const std::vector<unsigned char>& Display::renderEstado(const Hidrometro& h, int& W, int& Hgt, int& comp)
{
    comp = 3;

    int winX = 0, winY = 0, winW = 0, winH = 0;
    int lX   = 0, lY   = 0;
    int dlX  = 0, dlY  = 0;
    int smallW = 0, smallH = 0;

    W = 640; Hgt = 360;
    buffer.assign(W*Hgt*3, 255);

    int cx = W/2, cy = Hgt/2;
    int outerR = (int)(std::min(W, Hgt) * 0.45);
    int bezel  = (int)(outerR * 0.09);
    ring      (buffer, W, Hgt, cx, cy, outerR, bezel,   0,100,200);  // aro azul
    fillCircle(buffer, W, Hgt, cx, cy, outerR - bezel, 250,250,250); // face clara

    winW = 220;
    winH = 32;
    winX = cx - winW/2;
    winY = cy - 70;
    fillRect  (buffer, W, Hgt, winX, winY, winW, winH, 255,255,255);
    strokeRect(buffer, W, Hgt, winX, winY, winW, winH,   0,0,0);

    {
        int cellW = winW / 4;
        for (int i = 1; i < 4; ++i) {
            int x = winX + i*cellW;
            for (int yy = winY; yy < winY + winH; ++yy)
                putPixelRGB(buffer, W, Hgt, x, yy, 0, 0, 0);
        }
    }

    // caixinhas de DL e L (mais compactas)
    smallW = 60; smallH = 26;
    dlX = cx - 90;  dlY = cy + 28;  // mais para dentro do aro
    lX  = cx +  40; lY  = cy + 28;

    drawTextLine(buffer, W, Hgt, dlX - 42, dlY + 6, "DL=", 3, 0,0,0);
    drawTextLine(buffer, W, Hgt, lX  - 32, lY  + 6, "L=",  3, 0,0,0);

    fillRect  (buffer, W, Hgt, dlX, dlY, smallW, smallH, 255,255,255);
    strokeRect(buffer, W, Hgt, dlX, dlY, smallW, smallH, 0,0,0);
    fillRect  (buffer, W, Hgt, lX,  lY,  smallW, smallH, 255,255,255);
    strokeRect(buffer, W, Hgt, lX,  lY,  smallW, smallH, 0,0,0);

    const auto d = h.getMedicao().getDigitos();
    int m3k = d[0], m3c = d[1], m3d = d[2], m3u = d[3], L = d[4], dL = d[5];
    double m3total = h.getMedicao().getTotalM3();  // <- já declarado aqui para uso no M3=

    {
        const int scale = 3;
        int charW = 5*scale + scale;
        int textW = 4 * charW;
        int cxText = winX + (winW - textW) / 2;
        int cyText = winY + (winH - 7*scale) / 2;

        std::string s;
        s.push_back(char('0'+m3k));
        s.push_back(char('0'+m3c));
        s.push_back(char('0'+m3d));
        s.push_back(char('0'+m3u));

        drawTextLine(buffer, W, Hgt, cxText, cyText, s, scale, 0,0,0);
    }

    {
        const int scale = 3;
        int charW = 5*scale + scale;
        std::string sL = std::to_string(L);
        std::string sDL = std::to_string(dL);
        int lCenteredX = lX  + (smallW - (int)(sL.size()  * charW))  / 2;
        int dlCenteredX = dlX + (smallW - (int)(sDL.size() * charW)) / 2;
        int baseY = 4;
        drawTextLine(buffer, W, Hgt, lCenteredX,  lY  + baseY, sL,  scale, 0,0,0);
        drawTextLine(buffer, W, Hgt, dlCenteredX, dlY + baseY, sDL, scale, 0,0,0);
    }

    {
        const int scale = 3;
        int charW = 5*scale + scale;
        const std::string titulo = "ODOMETRO";
        int tw = (int)titulo.size() * charW;
        int tx = winX + (winW - tw)/2;
        int ty = winY - 24;
        drawTextLine(buffer, W, Hgt, tx, ty, titulo, scale, 0,0,0);
    }

    {
        std::ostringstream m3s;
        m3s << "M3=" << std::fixed << std::setprecision(4) << m3total;

        const int scaleM3 = 2;
        int charW = 5*scaleM3 + scaleM3;
        int m3TextW = static_cast<int>(m3s.str().size()) * charW;

        int m3x = cx - m3TextW/2;
        int m3y = cy + (outerR - bezel) - 50; // encostado no bordo interno do aro

        drawTextLine(buffer, W, Hgt, m3x, m3y, m3s.str(), scaleM3, 0,0,0);
    }

    drawTextLine(buffer, W, Hgt, 40, Hgt - 40, "SHA V2.0", 3, 0,0,0);

    return buffer;
}

}