#pragma once

#include <string>
#include <algorithm>
#include <filesystem>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
class rtImage
{
public:

    rtImage() = default;
    rtImage( const std::filesystem::path &imageFile );
    ~rtImage();

    void Resize(int width, int height, bool zeroInit = true);
    bool Load( const std::string &fileName );

    int  Width() const;
    int  Height() const;
    bool Save( const std::string &fileName ) const;

    unsigned char* PixelData();
    const unsigned char* PixelData() const;
    const unsigned char* PixelData( int x, int y ) const;

private:

    static int           Clamp( int x, int low, int high );
    static unsigned char FloatToByte( float value );
    void                 ConvertToBytes(float *fdata);

    const int      _bytesPerPixel   = 3;
    unsigned char *_bdata           = nullptr;
    int            _imageWidth      = 0;
    int           _imageHeight      = 0;
    int           _bytesPerScanline = 0;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline int rtImage::Width() const
{
    return _imageWidth;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline int rtImage::Height() const
{
    return _imageHeight;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline unsigned char* rtImage::PixelData()
{
    return _bdata;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline const unsigned char* rtImage::PixelData() const
{
    return _bdata;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
inline int rtImage::Clamp( int x, int low, int high )
{
    return std::clamp( x, low, high );
}
