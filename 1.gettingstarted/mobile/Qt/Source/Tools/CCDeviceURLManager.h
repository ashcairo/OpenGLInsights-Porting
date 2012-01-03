/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceURLManager.h
 * Description : Qt specific url manager.
 *
 * Created     : 14/08/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __DEVICEURLMANAGER_H__
#define __DEVICEURLMANAGER_H__


#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <stdio.h>

#include "CCPlatform.h"
#include "CCBaseTools.h"
#include "CCBaseTypes.h"
struct CCURLRequest;

class CCDeviceURLManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;

public:
    CCDeviceURLManager();
    void processRequest(CCURLRequest *inRequest);
    void clear();
    const bool readyToRequest();

public slots:
    void downloadFinished(QNetworkReply *reply);

protected:
    CCList<CCURLRequest> currentRequests;
};


#endif // __DEVICEURLMANAGER_H__
