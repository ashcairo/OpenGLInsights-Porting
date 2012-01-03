/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCFBWebView.cpp
 *-----------------------------------------------------------
 */

#include "CCFBWebView.h"
#include <QWebFrame>
#include <QWebElement>
#include "CCDefines.h"
#include "CCFBApi.h"
#include <QNetworkProxyQuery>
#include "CCMainWindow.h"

#include "CCBaseTypes.h"
#include "CCFileManager.h"

static const char *userDataFile = "cache/fb.login";


CCFBWebView::CCFBWebView(QWidget *parentWidget)
{
    // Load our username and password
    {
        char *data;
        int fileSize = CCFileManager::getFileData( userDataFile, &data, Resource_Cached, false );
        if( fileSize > 0 )
        {
            CCText userData( data );
            CCList<char> *splitText = userData.split( "\n" );
            if( splitText->length == 2 )
            {
                m_username = splitText->list[0];
                m_password = splitText->list[1];
            }
            DELETE_POINTER( splitText );
        }
    }

    m_parentWidget = parentWidget;
    m_parentWidgetTitle =  m_parentWidget->windowTitle();

    m_webView = new QWebView( parentWidget );

    m_progress = 0;
    m_deleting = false;

    //create web view signals
    {
        connect( m_webView, SIGNAL( titleChanged(QString) ),   SLOT( adjustTitle() ) );
        connect( m_webView, SIGNAL( loadProgress(int) ),       SLOT( setProgress(int) ) );
        connect( m_webView, SIGNAL( loadFinished(bool) ),      SLOT( finishLoading(bool) ) );
        connect( m_webView, SIGNAL( linkClicked(const QUrl&)), SLOT( clickedLink(const QUrl&) ) );connect( m_webView->page(), SIGNAL( contentsChanged()), SLOT( contentsChanged() ) );
        connect( m_webView->page(), SIGNAL( contentsChanged()), SLOT( contentsChanged() ) );
    }

    m_networkManager = new QNetworkAccessManager( this );
    QNetworkProxyQuery npq( QUrl( QLatin1String( "http://www.google.com" ) ) );
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery( npq );
    if( listOfProxies.size() > 0 )
    {
        QList<QNetworkProxy>::iterator i;
        for( i = listOfProxies.begin(); i!=listOfProxies.end(); ++i )
        {
            QNetworkProxy proxy = *i;
            m_networkManager->setProxy( proxy );
        }
    }
    connect( m_networkManager,
             SIGNAL( sslErrors(QNetworkReply*, const QList<QSslError>&) ),
             this,
             SLOT( sslErrorHandler(QNetworkReply*, const QList<QSslError>&) ) );

    // this is the main url for the web Authentication and authorization to FB
    QString weburl = CCFBApi::GetAuthorizationURL().buffer;
    setNetManager( weburl );

    m_webView->show();
}


CCFBWebView::~CCFBWebView()
{
    CCMainWindow::instance->removeChild( this );
    if( m_networkManager != NULL )
    {
        delete m_networkManager;
    }

    if( m_webView != NULL )
    {
        delete m_webView;
    }
}


void CCFBWebView::resizeView(int x1, int y1, int x2, int y2)
{
    if( m_webView != NULL )
    {
        int width = x2 - x1;
        int height = y2 - y1;
        m_webView->setMaximumSize( width, height );
        m_webView->setGeometry( x1, y1, x2, y2 );
    }
}


void CCFBWebView::setNetManager(const QString &webURL)
{
    DEBUGLOG( "CCFBWebView: %s\n", webURL.toUtf8().constData() );

    m_webView->page()->setNetworkAccessManager( m_networkManager );

    QNetworkRequest m_request = QNetworkRequest();
    m_request.setUrl( QUrl( webURL ) );
    connect( m_networkManager, SIGNAL( finished( QNetworkReply* ) ),this, SLOT( finishedSlot( QNetworkReply* ) ) );
    m_networkManager->get( m_request );

    m_webView->load( QUrl( webURL ) );
}


void CCFBWebView::shutdown()
{
    // Save out our username and password
    {
        CCText data;
        data += m_username.toUtf8().constData();
        data += "\n";
        data += m_password.toUtf8().constData();
        QFile file( userDataFile );
        if( file.open( QIODevice::WriteOnly ) )
        {
            file.write( data.buffer, data.length );
            file.close();
        }
    }

    m_parentWidget = NULL;
    m_deleting = true;
    m_webView->hide();

    if( m_networkManager != NULL )
    {
        m_networkManager->deleteLater();
        m_networkManager = NULL;
    }

    if( m_webView != NULL )
    {
        m_webView->deleteLater();
        m_webView = NULL;
    }

    deleteLater();
    CCFBApi::onBrowserClosed();
}


void CCFBWebView::sslErrorHandler(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->ignoreSslErrors();
}


void CCFBWebView::clickedLink(const QUrl& url)
{
    setNetManager( url.toString() );
}


void CCFBWebView::finishedSlot(QNetworkReply *reply)
{
    if( m_deleting == false )
    {
        QNetworkReply::NetworkError networkError = reply->error();
        if( networkError == QNetworkReply::NoError )
        {
            QString fileContentType = reply->header( QNetworkRequest::ContentTypeHeader ).toString();
            if( fileContentType.contains( "html" ) )
            {
                QString urlString = reply->url().toString();
                if( CCFBApi::ProcessLogin( urlString.toUtf8().constData() ) )
                {
                    if( CCFBApi::GetUserAccessToken() != NULL )
                    {
                        m_parentWidget->setWindowTitle( m_parentWidgetTitle );
                    }
                    else
                    {
                        m_parentWidget->setWindowTitle( "Failed to Login.." );
                    }
                    shutdown();
                }
            }
        }
        else if( networkError != QNetworkReply::OperationCanceledError )
        {
            //ASSERT( true );
            m_parentWidget->setWindowTitle( "Failed to Login.." );
            shutdown();
        }
    }
    reply->deleteLater();
}


void CCFBWebView::finishLoading(bool)
{
    m_progress = 100;
    adjustTitle();

    if( m_webView != NULL )
    {
        QString code_email = "document.getElementById( 'email' ).value='";
        code_email.append( m_username );
        code_email.append( "'" );
        m_webView->page()->mainFrame()->evaluateJavaScript( code_email );

        QString code_password = "document.getElementById( 'pass' ).value='";
        code_password.append( m_password );
        code_password.append( "'" );
        m_webView->page()->mainFrame()->evaluateJavaScript( code_password );

        // Submit login
        QString code_login = "document.getElementById( 'login_form' ).submit();";
        //m_webView->page()->mainFrame()->evaluateJavaScript( code_login );
    }
}

 
void CCFBWebView::setProgress(int progress)
{
    m_progress = progress;
    adjustTitle();
}


void CCFBWebView::adjustTitle()
{
    if( m_parentWidget != NULL )
    {
        if( m_progress <= 0 || m_progress >= 100 )
        {
            QString title =  m_webView->title();
            m_parentWidget->setWindowTitle( title );
        }
        else
        {
            m_parentWidget->setWindowTitle( QString( "%1 (%2%)" ).arg( m_webView->title() ).arg( m_progress ) );
        }
    }
}


void CCFBWebView::contentsChanged()
{
    QWebElement emailElement = m_webView->page()->mainFrame()->findFirstElement( "input[id=email]" );
    QWebElement passwordElement = m_webView->page()->mainFrame()->findFirstElement( "input[id=pass]" );
    m_username = emailElement.evaluateJavaScript( "this.value" ).toString();
    m_password = passwordElement.evaluateJavaScript( "this.value" ).toString();
}


void CCFBWebView::ClearData()
{
    if( CCFileManager::doesCachedFileExist( userDataFile ) )
    {
        CCFileManager::deleteCachedFile( userDataFile );
    }
}
