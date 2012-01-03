/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceURLManager.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCDeviceURLManager.h"
#include <QNetworkProxyQuery>


CCDeviceURLManager::CCDeviceURLManager()
{
    QNetworkProxyQuery npq( QUrl( QLatin1String( "http://www.google.com" ) ) );
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
    if( listOfProxies.size() > 0 )
    {
        QList<QNetworkProxy>::iterator i;
        for( i = listOfProxies.begin(); i!=listOfProxies.end(); ++i )
        {
            QNetworkProxy proxy = *i;
            manager.setProxy( proxy );
        }
    }

    connect( &manager, SIGNAL( finished(QNetworkReply*) ), SLOT( downloadFinished(QNetworkReply*) ) );
}


void CCDeviceURLManager::processRequest(CCURLRequest *inRequest)
{
    inRequest->state = CCURLRequest::in_flight;
    currentRequests.add( inRequest );

    QUrl url = QUrl::fromEncoded( inRequest->url.buffer );
    QNetworkRequest request( url );
    QNetworkReply *reply = manager.get( request );
    //manager.deleteResource( request );
    ASSERT( reply != NULL );
}


const bool CCDeviceURLManager::readyToRequest()
{
    return true;
}


void CCDeviceURLManager::clear()
{
    currentRequests.length = 0;
}


void CCDeviceURLManager::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();

    for( int i=0; i<currentRequests.length; ++i )
    {
        CCURLRequest *currentRequest = currentRequests.list[i];
        if( CCText::Contains( currentRequest->url.buffer, url.toString().toUtf8().constData() ) )
        {
            // Parse the headers
            QList<QByteArray> headers = reply->rawHeaderList();
            for( int i=0; i<headers.size(); ++i )
            {
                CCText *headerName = new CCText( headers[i].constData() );
                CCText *headerData = new CCText( reply->rawHeader( headerName->buffer ).constData() );
                currentRequest->header.add( headerName, headerData );
            }

            QNetworkReply::NetworkError error = reply->error();
            if( error != QNetworkReply::NoError )
            {
                //fprintf( stderr, "Download of %s failed: %s\n", url.toEncoded().constData(), qPrintable( reply->errorString() ) );
                currentRequest->state = CCURLRequest::failed;
            }
            else
            {
                currentRequest->state = CCURLRequest::succeeded;
                QByteArray replyData = reply->readAll();
                currentRequest->data.set( replyData.constData(), (uint)replyData.size() );
            }

            currentRequests.remove( currentRequest );
            break;
        }
    }

    reply->deleteLater();
}
