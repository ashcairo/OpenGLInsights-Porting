/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceFileManager.h
 * Description : Qt specific file manager
 *
 * Created     : 27/04/10
 * Author(s)   : Jerome Portier, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCDEVICEFILEMANAGER_H__
#define __CCDEVICEFILEMANAGER_H__


#include <QtCore/QFile>
#include <QtCore/QDir>
#include "CCFileManager.h"


class CCDeviceFileManager : public CCFileManager
{
public:
    CCDeviceFileManager(CCResourceType resourceType);

    const bool open(const char *filePath);
	void close();
	const uint read(void *dest, const uint size);
    void seek(const uint size);
    void setPosition(const uint pos);
    const bool endOfFile();
	const uint size();
    const uint position();
	
protected:
    QFile m_File;
    bool file_opened;
    uint m_Size;
    uint m_Position;
};



#endif // __CCDEVICEFILEMANAGER_H__
