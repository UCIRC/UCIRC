#include <PvSampleUtils.h>
#include <PvPropertyList.h>
#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>
#include <PvString.h>
#include <PvResult.h>
#include <cstring>
#include <stdlib.h>

#define CONFIG_FILE ( "../CameraControl/lib/Configuration.pvxml" )
#define GENERAL_CONFIGURATION_TAG ( "GeneralParams" )
#define DEFAULT_BUFFERCOUNT ( 10 )
#define DEFAULT_IMAGECOUNT ( 6 )
#define DEFAULT_IMAGEPATH ( "Data" )


void PrintHelp();


int main( int aCount, const char **aArgs ){
	//Check if user merely wants help, gives help then exits if so	
	if ( ParseOptionFlag( aCount, aArgs, "--help" ) )
    {
		PrintHelp();
		exit( 0 );
	}

	//String Indicating wear the param list is stored
	string param_string = GENERAL_CONFIGURATION_TAG;
	const PvString ParamList = PvString( param_string.c_str() );
	PvConfigurationReader lReader;
	PvConfigurationWriter lWriter;
	PvPropertyList lList;
	PvResult lResult;
	//Attempt to Load the Config File
	lResult = lReader.Load( CONFIG_FILE );
	
	//NEED TO ADD FAILSAFES AND CHECK IF RESULTS.AREOK
	//If It does not Find the file, it writes to a new one of the same name
	if ( lResult.GetCodeString() == "CANNOT_OPEN_FILE" ){
		cout << "File Does not exist or cannot be opened" << endl;
	
		cout << "Writing Defaults to New file" << endl;
		PvProperty pBufferCount = PvProperty( "BufferCount", "" );
		pBufferCount.SetValue( (int64_t) (DEFAULT_BUFFERCOUNT) );
		lList.Add( pBufferCount);

		PvProperty pImagePath = PvProperty( "ImagePath", "" );
		pImagePath.SetValue( PvString( DEFAULT_IMAGEPATH ) );
		lList.Add( pImagePath );


		PvProperty pImageCount = PvProperty( "ImageCount", "" );
		pImagePath.SetValue( (int64_t) (DEFAULT_IMAGECOUNT) );
		lList.Add( pImageCount );
		
		lWriter.Store( &lList, ParamList );
		lWriter.Save( CONFIG_FILE );
		cout << "Defaults Written to new file" << endl;
		return 1;		
	}
	
	//If it fails to load a file it exits
	else if ( !lResult.IsOK() )
	{
		cout << "Error, could not open Configuration File" << endl;
		return 0;
	}

	//Attempts to restore a PvPropertyList stored in the file
	lResult = lReader.Restore( ParamList, &lList );
	//If It cannot find the property it creates a new one and writes the defualts
	if ( lResult.GetCodeString() == "NOT_FOUND" )
	{
		cout << "No Property List found at " << ParamList.GetAscii() << endl;		
		
		cout << "Writing Defaults to New file" << endl;
		PvProperty pBufferCount = PvProperty( "BufferCount", "" );
		pBufferCount.SetValue( (int64_t) (DEFAULT_BUFFERCOUNT) );
		lList.Add( pBufferCount);

		PvProperty pImagePath = PvProperty( "ImagePath", "" );
		pImagePath.SetValue( PvString( DEFAULT_IMAGEPATH ) );
		lList.Add( pImagePath );


		PvProperty pImageCount = PvProperty( "ImageCount", "" );
		pImagePath.SetValue( (int64_t) (DEFAULT_IMAGECOUNT) );
		lList.Add( pImageCount );
		
		lWriter.Store( &lList, ParamList );
		lWriter.Save( CONFIG_FILE );
		cout << "Defaults Written to new file" << endl;
		return 1;		
	}

	//If it cannot load the PvPropertyList it exits	
	else if ( !lResult.IsOK() )
	{
		cout << "Error, failed to find a property list" << endl;
		return 0;
	}
	
		



	if ( ParseOptionFlag( aCount, aArgs, "--reset" ) )
	{
		PvProperty *lProperty;
		cout << "Restting configuration to default settings" << endl;
		lProperty = lList.GetProperty( "BufferCount");
		lProperty->SetValue( (int64_t) (DEFAULT_BUFFERCOUNT ) );
		lProperty = lList.GetProperty( "ImagePath" );
		lProperty->SetValue( PvString( DEFAULT_IMAGEPATH ) );
		lProperty = lList.GetProperty( "ImageCount" );
		lProperty->SetValue( (int64_t) (DEFAULT_IMAGECOUNT) );	
	}
	
	else
	{
		int64_t mImageCount;
		int64_t mBufferCount;
		string mImagePath;
		PvProperty *lProperty;

		if ( ParseOption<int64_t>( aCount, aArgs, "--imagecount", mImageCount ) )
		{
			lProperty = lList.GetProperty( "ImageCount");
			lProperty->SetValue( mImageCount );
		}
		if ( ParseOption<string>(aCount, aArgs, "--imagepath", mImagePath ) )
		{
			lProperty = lList.GetProperty( "ImagePath" );
			lProperty->SetValue( PvString( mImagePath.c_str() ) );
		}
		if ( ParseOption<int64_t>(aCount, aArgs, "--buffercount", mBufferCount ) )
		{
			lProperty = lList.GetProperty( "BufferCount" );
			lProperty->SetValue( mBufferCount );
		}
		
	}
	
	
	lWriter.Store( &lList, ParamList );
	lWriter.Save( CONFIG_FILE );

	return 1;

}

void PrintHelp()
{
    cout << "This program can be used to set image collection parameters for ./CameraControl." << endl; 
    cout << "These parameters can be set/changed by using the following tags as comandline arguments." << endl; 
    cout << "Otherwise they remain the default values." << endl << endl;

	cout << "--reset" << endl;
	cout << "Resets the configuration to Default settings" << endl << endl;

    cout << "--buffercount=<Desired Buffer Count>" << endl;
    cout << "Default: " << DEFAULT_BUFFERCOUNT << endl <<endl;

    cout << "--imagecount=<Desired image count>" << endl;
    cout << "Default: " << DEFAULT_IMAGECOUNT << endl << endl;

    cout << "--imagepath=<Desired image path>" << endl;
    cout << "Default: " << DEFAULT_IMAGEPATH << endl << endl;
}
