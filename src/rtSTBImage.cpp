#include <iostream>
#include <rtSTBImage.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#include <stb_image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

#include <filesystem>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
rtImage::rtImage( const std::filesystem::path &imageFile )
{
    // Loads image data from the specified file.If the image was not loaded
    // successfully, width() and height() will return 0.
    if (!Load(imageFile.string()))
    {
        std::cerr << "ERROR: Could not load image file '" << imageFile << "'.\n";
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
rtImage::~rtImage()
{
    delete[] _bdata;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void rtImage::Resize(int width, int height, bool zeroInit)
{
    if (_bdata)
    {
        if ( width * height > _imageWidth * _imageHeight )
        {
            delete[] _bdata;
            _bdata = nullptr;
        }
    }

    _imageWidth  = width;
    _imageHeight = height;
    _bytesPerScanline = _bytesPerPixel * _imageWidth;

    if ( !_bdata )
    {
        auto totalBytes = _imageWidth * _imageHeight * _bytesPerPixel;
        _bdata = new unsigned char[totalBytes];
    }

    if ( zeroInit )
    {
        memset(_bdata, 0, _imageWidth * _imageHeight * _bytesPerPixel);
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool rtImage::Load( const std::string &fileName )
{
    // Loads the linear (gamma=1) image data from the given file name. Returns
    // true if the load succeeded. The resulting data buffer contains the three
    // [0.0, 1.0] floating-point values for the first pixel (red, then green,
    // then blue). Pixels are contiguous, going left to right for the width of
    // the image, followed by the next row below, for the full height of the
    // image.

    auto n = _bytesPerPixel; // Dummy out parameter: original components per pixel
    auto fdata = stbi_loadf( fileName.c_str(), &_imageWidth, &_imageHeight, &n, _bytesPerPixel );
    if ( !fdata )
    {
        _imageWidth = 0;
        _imageHeight = 0;
        return false;
    }

    _bytesPerScanline = _imageWidth * _bytesPerPixel;
    ConvertToBytes(fdata);
    STBI_FREE(fdata);

    return true;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool rtImage::Save( const std::string &fileName ) const
{
    std::filesystem::path path(fileName.c_str());

    if ( path.extension() == ".png" )
    {
        return ( 0 != stbi_write_png( fileName.c_str(),
                                      Width(),
                                      Height(),
                                      _bytesPerPixel,
                                      _bdata,
                                      _bytesPerScanline ) );
    }

    return false;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
const unsigned char *rtImage::PixelData( int x, int y ) const
{
    // Return the address of the three RGB bytes of the pixel at x,y. If there
    // is no image data, returns magenta.
    static unsigned char magenta[] = { 255, 0, 255 };
    if ( !_bdata )
    {
        assert(false);
        return magenta;
    }

    x = Clamp( x, 0, _imageWidth );
    y = Clamp( y, 0, _imageHeight );

    return _bdata + y * _bytesPerScanline + x * _bytesPerPixel;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
unsigned char rtImage::FloatToByte( float value )
{
    if ( value <= 0.0 )
    {
        return 0;
    }

    if ( 1.0 <= value )
    {
        return 255;
    }

    return static_cast<unsigned char>(256.0 * value);
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void rtImage::ConvertToBytes(float *fdata)
{
    // Convert the linear floating point pixel data to bytes, storing the
    // resulting byte data in the `bdata` member.
    int totalBytes = _imageWidth * _imageHeight * _bytesPerPixel;
    _bdata = new unsigned char[totalBytes];

    // Iterate through all pixel components, converting from [0.0, 1.0] float
    // values to unsigned [0, 255] byte values.
    auto *bptr = _bdata;
    auto *fptr = fdata;
    for ( auto i = 0; i < totalBytes; i++, fptr++, bptr++ )
    {
        *bptr = FloatToByte( *fptr );
    }
}
