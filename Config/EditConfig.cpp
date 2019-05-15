#include <PvSampleUtils.h>
#include <PvPropertyList.h>
#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>
#include <PvString.h>
#include <PvResult.h>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

#define CONFIG_FILE ( "../CameraControl/lib/Configuration.pvxml" )
#define PARAM_FILE ( "../CameraControl/lib/Configration.pvcfg" )
#define GENERAL_CONFIGURATION_TAG ( "GeneralParams" )
#define DEFAULT_BUFFERCOUNT ( 10 )
#define DEFAULT_IMAGECOUNT ( 6 )
#define DEFAULT_MAC_1 ( "00:11:1c:02:cd:77" )
#define DEFAULT_MAC_2 ( "00:11:1c:03:09:12" )
#define DEFAULT_IMAGEPATH ( "Data/" )
bool Delete();

bool Delete_And_Save( PvConfigurationWriter *aWriter, PvPropertyList *aList );

void ListProperties( PvPropertyList *lPropertyList );

void PrintHelp();

//Return 0 if successful write operation
//Return -1 if any failed operation
//Return 1 if any other successful operation
int main( int aCount, const char **aArgs ){
	//Check if user merely wants help, gives help then exits if so	
	if ( ParseOptionFlag( aCount, aArgs, "--help" ) )
    {
		PrintHelp();
		exit( 0 );
	}

	else if ( ParseOptionFlag( aCount, aArgs, "--delete" ) )
	{
		if( Delete() ) return 1;
		else
		{
			cout << "No file to delete(?)" << endl;
			return -1;
		}
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

		PvProperty pMAC_1 = PvProperty( "MAC_1", "" );
		pMAC_1.SetValue( PvString( DEFAULT_MAC_1 ) );
		lList.Add( pMAC_1 );

		PvProperty pMAC_2 = PvProperty( "MAC_2", "" );
		pMAC_2.SetValue( PvString( DEFAULT_MAC_2 ) );
		lList.Add( pMAC_2 );

		PvProperty pImageCount = PvProperty( "ImageCount", "" );
		pImageCount.SetValue( (int64_t) (DEFAULT_IMAGECOUNT) );
		lList.Add( pImageCount );
	
		ListProperties( &lList );
	
		lWriter.Store( &lList, ParamList );
		lWriter.Save( CONFIG_FILE );
		cout << "Defaults Written to new file" << endl;
		return 0;		
	}
	
	//If it fails to load a file it exits
	else if ( !lResult.IsOK() )
	{
		cout << "Error, could not open Configuration File" << endl;
		return -1;
	}

	//Attempts to restore a PvPropertyList stored in the file
	lResult = lReader.Restore( ParamList, &lList );
	//If It cannot find the property it creates a new one and writes the defualts
	if ( lResult.GetCodeString().GetAscii() == "NOT_FOUND" )
	{
		cout << "No Property List found at " << ParamList.GetAscii() << endl;		
		
		cout << "Writing Defaults to New file" << endl;
		PvProperty pBufferCount = PvProperty( "BufferCount", "" );
		pBufferCount.SetValue( (int64_t) (DEFAULT_BUFFERCOUNT) );
		lList.Add( pBufferCount);

		PvProperty pImagePath = PvProperty( "ImagePath", "" );
		pImagePath.SetValue( PvString( DEFAULT_IMAGEPATH ) );
		lList.Add( pImagePath );

		PvProperty pMAC_1 = PvProperty( "MAC_1", "" );
		pMAC_1.SetValue( PvString( DEFAULT_MAC_1 ) );
		lList.Add( pMAC_1 );

		PvProperty pMAC_2 = PvProperty( "MAC_2", "" );
		pMAC_1.SetValue( PvString( DEFAULT_MAC_2 ) );
		lList.Add( pMAC_2 );
		
		PvProperty pImageCount = PvProperty( "ImageCount", "" );
		pImagePath.SetValue( (int64_t) (DEFAULT_IMAGECOUNT) );
		lList.Add( pImageCount );
		
		if ( !Delete_And_Save( &lWriter, &lList ) )
		{
			return -1;
		}
		cout << "Defaults Written to new file" << endl;
		return 1;
	}

	//If it cannot load the PvPropertyList it exits	
	else if ( !lResult.IsOK() )
	{
		cout << "Error, failed to find a property list" << endl;
		return -1;
	}
		
	if ( ParseOptionFlag( aCount, aArgs, "--list" ) )
	{
		ListProperties( &lList );
		return 1;
	}

	bool change = false;

	if ( ParseOptionFlag( aCount, aArgs, "--reset" ) )
	{
		PvProperty *lProperty;
		cout << "Restting configuration to default settings" << endl;
		//Clear List of all contents
		lList.Clear();
	
		//Rewrite values to the list
		PvProperty pBufferCount = PvProperty( "BufferCount", "" );
		pBufferCount.SetValue( (int64_t) (DEFAULT_BUFFERCOUNT) );
		lList.Add( pBufferCount);

		PvProperty pImagePath = PvProperty( "ImagePath", "" );
		pImagePath.SetValue( PvString( DEFAULT_IMAGEPATH ) );
		lList.Add( pImagePath );

		PvProperty pMAC_1 = PvProperty( "MAC_1", "" );
		pMAC_1.SetValue( PvString( DEFAULT_MAC_1 ) );
		lList.Add( pMAC_1 );

		PvProperty pMAC_2 = PvProperty( "MAC_2", "" );
		pMAC_2.SetValue( PvString( DEFAULT_MAC_2 ) );
		lList.Add( pMAC_2 );
		
		PvProperty pImageCount = PvProperty( "ImageCount", "" );
		pImageCount.SetValue( (int64_t) (DEFAULT_IMAGECOUNT) );
		lList.Add( pImageCount );

		if ( !Delete_And_Save( &lWriter, &lList ) )
		{
			cout << "Reset Failure" << endl;
			return -1;
		}
		
		else
		{
			cout << "Reset Success" << endl;
			return 0;
		}
	}
	
	else
	{
		int64_t mImageCount;
		int64_t mBufferCount;
		string mMAC_1;
		string mMAC_2;
		string mImagePath;
		PvProperty *lProperty;
		PvPropertyList lCopy;


		if ( ParseOption<int64_t>( aCount, aArgs, "--imagecount", mImageCount ) )
		{
			PvProperty pImageCount = PvProperty( "ImageCount", "" );
			pImageCount.SetValue( mImageCount );
			lCopy.Add( pImageCount);
			change=true;
		}

		else
		{
			PvProperty pImageCount = PvProperty( *lList.GetProperty( "ImageCount" ) );
			lCopy.Add( pImageCount);
		}

		if ( ParseOption<string>(aCount, aArgs, "--imagepath", mImagePath ) )
		{
			change=true;
			PvProperty pImagePath = PvProperty( "ImagePath", "" );
			pImagePath.SetValue( PvString( mImagePath.c_str() ) );
			lCopy.Add( pImagePath );
		}

		else
		{
			PvProperty pImagePath = PvProperty( *lList.GetProperty( "ImagePath" ) );
			lCopy.Add( pImagePath );
		}

		if ( ParseOption<string>(aCount, aArgs, "--MAC_1", mMAC_1 ) )
		{
			change=true;
			PvProperty pMAC_1 = PvProperty( "MAC_1", "" );
			pMAC_1.SetValue( PvString( mMAC_1.c_str() ) );
			lCopy.Add( pMAC_1 );
		}
		
		else
		{
			PvProperty pMAC_1 = PvProperty( *lList.GetProperty( "MAC_1" ) );
			lCopy.Add( pMAC_1 );
		}	

		if ( ParseOption<string>(aCount, aArgs, "--MAC_2", mMAC_2 ) )
		{
			change=true;
			PvProperty pMAC_2 = PvProperty( "MAC_2", "" );
			pMAC_2.SetValue( PvString( mMAC_2.c_str() ) );
			lCopy.Add( pMAC_2 );
		}
	
		else
		{	
			PvProperty pMAC_2 = PvProperty( *lList.GetProperty ( "MAC_2" ) );
			lCopy.Add( pMAC_2 );
		}
		
		if ( ParseOption<int64_t>(aCount, aArgs, "--buffercount", mBufferCount ) )
		{
			change=true;
			PvProperty pBufferCount = PvProperty( "BufferCount", "" );
			pBufferCount.SetValue( mBufferCount );
			lCopy.Add( pBufferCount );
		}
		
		else
		{
			PvProperty pBufferCount = PvProperty( *lList.GetProperty( "BufferCount" ) );
			lCopy.Add( pBufferCount );
		}

		if ( change )
		{		
			if ( !Delete_And_Save( &lWriter, &lCopy ) )
			{
				return -1;
			}
	
			else 
			{
				cout << "Updated the file" << endl;
				return 0;
			}
		}
	}
	

	cout << "File Exists; Nothing was changed" << endl;
	cout << "Enter './EditConfig --help' for help" << endl;
	return 1;

}

bool Delete()
{
	cout << "Deleting File..." << endl;
	int counter = 0;
	bool removed = false;
	cout << "Remove operation attempt " << counter << endl;
	while ( remove( CONFIG_FILE ) != 0 )
	{
		if ( counter > 5 )
		{
			cout << "Failed to remove file" << endl;
			cout << "Saving operation Failed" << endl;
			return false;
		}
		counter ++;
		cout << "Remove operation attempt " << counter << endl;	
	}
	
	cout << "Remove Operation Successful" << endl;
	cout << "Deleted File" << endl;
	return true;
}

bool Delete_And_Save( PvConfigurationWriter *aWriter, PvPropertyList *aList )
{
	cout << "Deleting File to be re-written..." << endl;
	int counter = 0;
	bool removed = false;
	cout << "Remove operation attempt " << counter << endl;
	while ( remove( CONFIG_FILE ) != 0 )
	{
		if ( counter > 5 )
		{
			cout << "Failed to remove file" << endl;
			cout << "Saving operation Failed" << endl;
			return false;
		}
		counter ++;
		cout << "Remove operation attempt " << counter << endl;	
	}
	
	cout << "Remove Operation Successful" << endl;

	string param_string = GENERAL_CONFIGURATION_TAG;
	const PvString ParamList = PvString( param_string.c_str() );
	
	cout << "Saving file..." << endl;
	aWriter->Store( aList, ParamList );
	aWriter->Save( CONFIG_FILE );
	cout << "File Saved" << endl;
	return true;
}

void ListProperties( PvPropertyList *lPropertyList )
{
	cout << "Property List Size: " << lPropertyList->GetSize() << endl;
	int counter = 0;
	PvProperty *lPointer = lPropertyList->GetFirst();
	while ( lPointer != NULL )
	{
		cout << "["<<counter<<"]"<< " Property Name: " << lPointer->GetName().GetAscii() << endl;
		cout << "   " << " Property value: " << lPointer->GetValue().GetAscii() << endl;
		cout << endl;
		
		lPointer = lPropertyList->GetNext();
		counter ++;
	}
	return;
}


void PrintHelp()
{
    cout << "This program can be used to set image collection parameters for ./CameraControl." << endl; 
    cout << "These parameters can be set/changed by using the following tags as comandline arguments." << endl; 
    cout << "Otherwise they remain the default values." << endl << endl;

	cout << "--reset" << endl;
	cout << "Resets the configuration to Default settings" << endl << endl;

	cout << "--list" << endl;
	cout << "Lists the Properties in the Config file" << endl << endl;

	cout << "--delete" << endl;
	cout << "Deletes the configuration file" << endl << endl;

    cout << "--buffercount=<Desired Buffer Count>" << endl;
    cout << "Default: " << DEFAULT_BUFFERCOUNT << endl <<endl;

    cout << "--imagecount=<Desired image count>" << endl;
    cout << "Default: " << DEFAULT_IMAGECOUNT << endl << endl;

	cout << "--MAC_1=<Desired MAC Address>" << endl;
	cout << "Default: " << DEFAULT_MAC_1 << endl << endl;

	cout << "--MAC_2=<Desired MAC Address>" << endl;
	cout << "Default: " << DEFAULT_MAC_2 << endl << endl;

    cout << "--imagepath=<Desired image path>" << endl;
    cout << "Default: " << DEFAULT_IMAGEPATH << endl << endl;
}
