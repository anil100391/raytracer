#pragma once

#include <string>
#include <filesystem>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class rtImage
{
public:

    rtImage() = default;
    rtImage( const std::filesystem::path &imageFile );
    ~rtImage();

    bool Load( const std::string &fileName );
    int  Width() const;
    int  Height() const;

    const unsigned char* PixelData( int x, int y ) const;

private:

    static int           Clamp( int x, int low, int high );
    static unsigned char FloatToByte( float value );
    void                 ConvertToBytes();

    const int      _bytesPerPixel   = 3;
    float         *_fdata           = nullptr;
    unsigned char *_bdata           = nullptr;
    int            _imageWidth      = 0;
    int           _imageHeight      = 0;
    int           _bytesPerScanline = 0;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline int rtImage::Width() const
{
    return _fdata ? _imageWidth : 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline int rtImage::Height() const
{
    return _fdata ? _imageHeight : 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline int rtImage::Clamp( int x, int low, int high )
{
    return std::clamp( x, low, high );
}
