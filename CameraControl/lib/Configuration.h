/*Configuration.h
*****************
This file contains the following:
-A method for parsing Command line arguments
-A class for representing the configuration of a system
-->Camera Addresses
-->Desired Image Counts
-->Desired Image Paths
-->Image Size
-->Etc

This file contains code from Pleora's provided sample libraries
*/

#include <PvSampleTransmitterConfig.h>
#include <cstring>

//Not sure what these are, or if needed
#define DEFAULT_WIDTH ( 0 )
#define DEFAULT_HEIGHT ( 0 )
//Default MAC address to connect to
#define DEFAULT_MAC ( "00:11:1c:03:09:12" )
//Not important at the moment
#define DEFAULT_IP ( "BLAH" )
//Default number of images taken by one execution
#define DEFAULT_IMAGECOUNT ( 6 )
//Default Location of Image Data Storage
#define DEFAULT_IMAGEPATH ( "Data/" )
#define DEFAULT_BUFFERCOUNT ( 10 )

//using namespace std;

//template <class T>
//inline bool ParseOptionFlag( int aCount, const char **Args, 
//inline bool ParseOption( int aCount, const char **aArgs, const char *aOption, T &aValue );

class Configuration : public PvSampleTransmitterConfig
{
public:

    Configuration()
        : mWidth( DEFAULT_WIDTH )
        , mHeight( DEFAULT_HEIGHT )
		, mMAC( DEFAULT_MAC )
		, mIP( DEFAULT_IP )
		, mImageCount( DEFAULT_IMAGECOUNT )
		, mImagePath( DEFAULT_IMAGEPATH )
		, mBufferCount( DEFAULT_BUFFERCOUNT )
    {
    }

    ~Configuration()
    {
    }

    uint32_t GetWidth() const { return mWidth; }
    uint32_t GetHeight() const { return mHeight; }
	string GetMAC() const { return mMAC; }
	string GetIP() const { return mIP; }
	uint32_t GetImageCount() const { return mImageCount; }
	string GetImagePath() const { return mImagePath; }
	uint32_t GetBufferCount() const { return mBufferCount; }

    void ParseCommandLine( int aCount, const char **aArgs )
    {
        if ( ParseOptionFlag( aCount, aArgs, "--help" ) )
        {
            PrintHelp();
            exit( 0 );
        }

    //    PvSampleTransmitterConfig::ParseCommandLine( aCount, aArgs );

        ParseOption<uint32_t>( aCount, aArgs, "--width", mWidth );
        ParseOption<uint32_t>( aCount, aArgs, "--height", mHeight );
        ParseOption<string>( aCount, aArgs, "--MAC", mMAC );
		ParseOption<string>( aCount, aArgs, "--IP", mIP);
		ParseOption<uint32_t>( aCount, aArgs, "--imagecount", mImageCount );
		ParseOption<string>(aCount, aArgs, "--imagepath", mImagePath );
		ParseOption<uint32_t>(aCount, aArgs, "--buffercount", mBufferCount );
    }

    void PrintHelp()
    {
        cout << "This program captures images using a code set of variables." << endl; 
		cout << "These variables can be set by using the following tags as comandline arguments." << endl; 
		cout << "Otherwise they remain the default values." << endl << endl;

        cout << "--width=<width in pixels>" << endl;
        cout << "Default: " << DEFAULT_WIDTH << endl << endl;

        cout << "--height=<height in pixels>" << endl;
        cout << "Default: " << DEFAULT_HEIGHT << endl << endl;

        cout << "--MAC=<MAC address of the camera>" << endl;
        cout << "Default: " << DEFAULT_MAC << endl << endl;

        cout << "--IP=<IP address of the camera>" << endl;
		cout << "Default: " << DEFAULT_IP << endl <<endl;
	
		cout << "--imagecount=<Desired image count>" << endl;
		cout << "Default: " << DEFAULT_IMAGECOUNT << endl;
   	
		cout << "--imagepath=<Desired image path>" << endl;
		cout << "Default: " << DEFAULT_IMAGEPATH << endl;

		cout << "--buffercount=<Desired Buffer Count>" <<endl;
		cout << "Default: " << DEFAULT_BUFFERCOUNT << endl;
	 }

private:

    uint32_t mWidth;
    uint32_t mHeight;
	string mMAC;
	string mIP;
	uint32_t mImageCount;
	string mImagePath;
	uint32_t mBufferCount;

};

/*
inline bool ParseOption( int aCount, const char ** aArgs, const char *aOption, T &aValue )
{
    string lOption = aOption;
    lOption += "=";
    for ( int i = 1; i < aCount; i++ )
    {
        string lString = aArgs[i];

        size_t lPos = lString.find( aOption );
        if ( lPos != string::npos )
        {
            if ( lString.size() > lOption.size() )
            {
                string lParameter = lString.substr( lOption.size(), ( lString.size() - lOption.size() ) + 1 );
                istringstream iss( lParameter, istringstream::in );
                iss >> aValue;
                return true;
            }
        }
    }

    return false;
}
*/
