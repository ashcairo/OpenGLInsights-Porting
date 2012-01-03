/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : main.cpp
 * Description : Qt application entry point.
 *
 * Created     : 05/06/10
 * Author(s)   : Jerome Portier, Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#include <QApplication>
#include "CCMainWindow.h"
#include <QDesktopWidget>

#ifdef Q_OS_SYMBIAN
    #include <eikenv.h>
    #include <eikappui.h>
    #include <aknenv.h>
    #include <aknappui.h>
#endif

class ApplicationClass : public QApplication
{
public:
    ApplicationClass(int _argc, char **_argv) : QApplication(_argc, _argv) {}

    bool notify(QObject *receiver, QEvent *event)
    {
        // cDebug() << "Called Application::notify()" << endl;
       try
       {
           return QApplication::notify( receiver, event );
       }
       catch( char const *str )
       {
           //cDebug() << "EXCEPTION: " << str << endl;
           return false;
       }
       catch( ... )
       {
           //cDebug() << "Unknown exception!" << endl;
           abort();
       }
    }
};

int main(int argc, char *argv[])
{
    ApplicationClass a( argc, argv );

#ifdef Q_OS_SYMBIAN
    // Lock orientation to landscape
    CAknAppUi* appUi = dynamic_cast<CAknAppUi*>( CEikonEnv::Static()->AppUi() );
    TRAPD( error,
        if( appUi )
        {
            appUi->SetOrientationL( CAknAppUi::EAppUiOrientationPortrait );
        }
    );
#endif

    CCMainWindow w;

    QDesktopWidget *desktop = QApplication::desktop();
    if( desktop )
    {
        {
            const QRect screenSize = desktop->screenGeometry();
            const float screenWidth = screenSize.width();
            const float screenHeight = screenSize.height();
            const float width = 480;
            const float height = 720;
            const float x = ( screenWidth - width ) * 0.5f;
            const float y = ( screenHeight - height ) * 0.5f;
            w.setGeometry( x, y, width, height );
        }

        if( false && desktop->numScreens() > 1 )
        {
#ifdef Q_OS_MACX
            w.move( QPoint( -1024, 0 ) );
#endif
            //w.showFullScreen();
        }
    }
    else
    {
        w.resize( QSize( 480, 320 ) );
    }

    //w.showFullScreen();
    w.show();

    return a.exec();
}
