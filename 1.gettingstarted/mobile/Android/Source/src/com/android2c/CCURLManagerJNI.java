/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 Ð 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCURLManagerJNI.java
 * Description : Loads URL requests then interfaces with ndk.
 *
 * Created     : 09/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

package com.android2c;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import android.util.Log;


public class CCURLManagerJNI 
{
	protected void processRequest(String url) 
	{
		// Might as well encode the URL here with Java's better String methods
		url = url.replaceAll( " ", "%20" );
		
		try
		{
        	URL urlObject = new URL( url );
            HttpURLConnection connection = (HttpURLConnection)urlObject.openConnection();
			
			// This will start the connection
			InputStream response = connection.getInputStream();
			
			// Get the response
			int status = connection.getResponseCode();
			if( status != 200 )
			{
				Log.e( CCJNI.library, "The connection to " + url + " was unsuccessful." );
				
				// See if we can get any useful error information (for 4xx and 5xx codes)
				if( status % 400 < 100 || status % 500 < 100 )
				{
					InputStream error = ( (HttpURLConnection)connection ).getErrorStream();
					Log.e( CCJNI.library, error.toString() );
				}
				
				CCJNI.urlManagerDownloadFinished( url, false, null, 0, null, null, 0 );
			}
			else
			{
				// Extract headers
				int headersLength = connection.getHeaderFields().size();
				String[] headerNames = new String[headersLength];
				String[] headerValues = new String[headersLength];
				for( int i=0; i<headersLength; ++i )
				{
					String key = connection.getHeaderFieldKey( i );
					String value = connection.getHeaderField( i );
					headerNames[i] = key;
					headerValues[i] = value;
				}
				
				// Get the response
                //int contentLength = connection.getContentLength();
                int dataSize = 1024 * 1024; // One megabyte muhahahahahaha
                byte[] data = new byte[dataSize];
                int offset = 0;
                {
                	int bytesRead = 0;
                    while( bytesRead >= 0 )
                    {
                    	offset += bytesRead;
                    	bytesRead = response.read( data, offset, dataSize - offset );
                    }
                }

                connection.disconnect();
				
                CCJNI.urlManagerDownloadFinished( url, true, data, offset+1, headerNames, headerValues, headersLength );
			}
		}
		catch (MalformedURLException e)
		{
			Log.e( CCJNI.library, "The supplied URL is incorrectly formed: " + url );
			CCJNI.urlManagerDownloadFinished( url, false, null, 0, null, null, 0 );
		}
		catch (IOException e)
		{
			Log.e( CCJNI.library, "There was an unknown I/O exception:" );
			e.printStackTrace();
			CCJNI.urlManagerDownloadFinished( url, false, null, 0, null, null, 0 );
		}
	}
}
