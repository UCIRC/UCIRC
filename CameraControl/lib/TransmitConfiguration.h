#ifndef __PVSAMPLETRANSMITTERCONFIG_H__
#define __PVSAMPLETRANSMITTERCONFIG_H__

#include <PvSampleUtils.h>
#include <PvSystem.h>
#include <PvNetworkAdapter.h>
#include <PvPixelType.h>

#include <map>


#define DEFAULT_NUMBER_OF_BUFFERS ( 48 )
#define DEFAULT_SOURCE_ADDRESS ( "" )
#define DEFAULT_DESTINATION_ADDRESS ( "239.192.1.1" )
#define DEFAULT_DESTINATION_PORT ( 1042 )
#define DEFAULT_SOURCE_PORT ( 0 )
#define DEFAULT_BUFFER_COUNT ( 4 )
#define DEFAULT_PACKET_SIZE ( 1440 )
#define DEFAULT_FPS ( 30 )
#define DEFAULT_SILENT ( false )


#ifndef PV_GENERATING_DOXYGEN_DOC


// Application config
class PvSampleTransmitterConfig
{
public:

    PvSampleTransmitterConfig()
    {
        SetDefaults();
    }

    ~PvSampleTransmitterConfig()
    {
    }

    const char *GetSourceAddress() const { return mSourceAddress.c_str(); }
    uint16_t GetSourcePort() const { return mSourcePort; }
    const char *GetDestinationAddress() const { return mDestinationAddress.c_str(); }
    uint16_t GetDestinationPort() const { return mDestinationPort; }
    float GetFPS() const { return mFPS; }
    bool GetSilent() const { return mSilent; }
    uint32_t GetPacketSize() const { return mPacketSize; }
    uint32_t GetBufferCount() const { return mBufferCount; }


private:

    void SetDefaults()
    {
        // Only used to enumerate network interfaces, no need to call Find
        PvSystem lSystem;

        // Find default source address
        bool lFound = false;
        for ( uint32_t i = 0; i < lSystem.GetInterfaceCount() && !lFound; i++ )
        {
            const PvNetworkAdapter *lNetworkAdapter = dynamic_cast<const PvNetworkAdapter *>( lSystem.GetInterface( i ) );
            if ( lNetworkAdapter == NULL )
            {
                continue;
            }

            uint32_t lIPCount = lNetworkAdapter->GetIPAddressCount();
            for ( uint32_t j = 0; j < lIPCount; j++ )
            {
                PvString lIP = lNetworkAdapter->GetIPAddress( j );
                if ( strcmp( "0.0.0.0", lIP.GetAscii() ) != 0 )
                {
                    mSourceAddress = lIP.GetAscii();
                    lFound = true;
                }
            }
        }
        if ( !lFound )
        {
            cout << "No valid interfaces found." << endl;
            exit( 1 );
        }

        // Set static defaults
        mDestinationAddress = DEFAULT_DESTINATION_ADDRESS;
        mDestinationPort = DEFAULT_DESTINATION_PORT;
        mSourcePort = DEFAULT_SOURCE_PORT;
        mPacketSize = DEFAULT_PACKET_SIZE;
        mBufferCount = DEFAULT_BUFFER_COUNT;
        mFPS = DEFAULT_FPS;
        mSilent = DEFAULT_SILENT;
    }

    string mSourceAddress;
    uint16_t mSourcePort;

    string mDestinationAddress;
    uint16_t mDestinationPort;

    float mFPS;
    bool mSilent;

    uint32_t mPacketSize;
    uint32_t mBufferCount;

};


#endif // PV_GENERATING_DOXYGEN_DOC

#endif
