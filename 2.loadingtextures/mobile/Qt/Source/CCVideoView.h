/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCVideoView.h
 * Description : Video player view.
 *
 * Created     : 04/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCVIDEOVIEW_H__
#define __CCVIDEOVIEW_H__


#include <QtGui/QWidget>

#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/videowidget.h>

#include "CCMainWindow.h"

class CCVideoView : public QWidget, public MainWindowChild
{
    Q_OBJECT
	
public:
    explicit CCVideoView(QWidget *parent);
    ~CCVideoView();

    // CCMainWindow
    void resizeView(int x1, int y1, int x2, int y2);

    void playVideo(const char *filePath);

protected:
    QWidget m_videoWindow;
    Phonon::MediaObject m_MediaObject;
    Phonon::AudioOutput m_AudioOutput;
    Phonon::VideoWidget *m_videoWidget;
    Phonon::Path m_audioOutputPath;
};


#endif // __CCVIDEOVIEW_H__
