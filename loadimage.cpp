#include "loadimage.h"


GLuint loadimage(const char* filename)
{
    unsigned char *pixelBuffer = NULL;
    BITMAPINFOHEADER infoHeader;
    BITMAPFILEHEADER fileHeader;
    loadbitmap(filename, pixelBuffer, &infoHeader, &fileHeader);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    
    GLuint texObject;
    glGenTextures(1, &texObject);
    glBindTexture(GL_TEXTURE_2D, texObject);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D,
     0,
     infoHeader.biBitCount == 32 ? GL_RGBA : GL_RGB,
     infoHeader.biWidth,
     infoHeader.biHeight,
     0,
     infoHeader.biBitCount == 32 ? GL_RGBA : GL_BGR_EXT,
     GL_UNSIGNED_BYTE,
     pixelBuffer
    );
    
    delete[] pixelBuffer;
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    
    return texObject;
}
