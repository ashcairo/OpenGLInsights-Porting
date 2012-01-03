/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceFileManager.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCDeviceFileManager.h"


CCDeviceFileManager::CCDeviceFileManager(CCResourceType resourceType)
{
    file_opened = false;
    m_Size = 0;
    m_Position = 0;
}


const bool CCDeviceFileManager::open(const char *filePath)
{
    m_File.setFileName( QString( filePath ) );
    file_opened = m_File.open( QIODevice::ReadOnly );
    if( !file_opened )
    {
        return false;
    }

    m_File.seek( 0 );
    m_Size = m_File.size();
    m_Position	= 0;

    return true;
}


void CCDeviceFileManager::close()
{
    if( file_opened )
    {
        m_File.close();
    }

    m_Size = 0;
    m_Position = 0;
}


const uint CCDeviceFileManager::read(void *dest, const uint size)
{
    const uint sz = m_File.read( (char*)dest, size );
    ASSERT( sz == size );
    if( sz > 0 )
    {
        m_Position += sz;
        return sz;
    }

    printf( "File::Read(...) : WARNING! Error reading. Code %i\n", sz );
    return 0;
}


void CCDeviceFileManager::seek(const uint size)
{
    m_Position += size;
    m_File.seek( m_Position );
}


void CCDeviceFileManager::setPosition(const uint pos)
{
    m_File.seek( pos );
    m_Position = pos;
}


const bool CCDeviceFileManager::endOfFile()
{
    return m_Position >= m_Size;
}


const uint CCDeviceFileManager::size()
{
    return m_Size;
}


const uint CCDeviceFileManager::position()
{ 
    return m_Position;
}
