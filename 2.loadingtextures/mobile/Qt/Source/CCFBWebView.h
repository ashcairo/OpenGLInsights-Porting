/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCFBWebView.h
 * Description : Launches web view to login to Facebook.
 *               Inspired by http://code.google.com/p/facebook-cpp-graph-api/
 *
 * Created     : 08/07/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCFBWEBVIEW_H__
#define __CCFBWEBVIEW_H__


#include "CCMainWindow.h"
#include "CCGLView.h"

#include <QtGui>
#include <QWebView>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class CCFBWebView : public QObject, public MainWindowChild
{
    Q_OBJECT

public:
    CCFBWebView(QWidget *parentWidget);
    ~CCFBWebView();

    // CCMainWindow
    void resizeView(int x1, int y1, int x2, int y2);

private:
    void setNetManager(const QString &webURL);
    void shutdown();

public slots:
    void sslErrorHandler(QNetworkReply*, const QList<QSslError>&);
    void clickedLink(const QUrl& url);
    void finishedSlot(QNetworkReply *reply);
    void finishLoading(bool);
    void setProgress(int progress);
    void adjustTitle();

    void contentsChanged();

public:
    static void ClearData();

private:
    QWidget *m_parentWidget;
    QString m_parentWidgetTitle;

    QWebView *m_webView;
    QNetworkAccessManager *m_networkManager;
    int m_progress;

    QString m_username;
    QString m_password;

    bool m_deleting;
};


#endif  // __CCFBWEBVIEW_H__
