/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCVideoView.cpp
 *-----------------------------------------------------------
 */

#include <QtGui>
#include "CCDefines.h"

#include "CCVideoView.h"


CCVideoView::CCVideoView(QWidget *parent) :
    QWidget( parent ),
    m_AudioOutput( Phonon::VideoCategory )
{
    m_videoWidget = new Phonon::VideoWidget( this );
    QVBoxLayout *videoLayout = new QVBoxLayout( this );
    videoLayout->addWidget( m_videoWidget );
    videoLayout->setContentsMargins( 0, 0, 0, 0);
    setLayout( videoLayout );

    m_audioOutputPath = Phonon::createPath( &m_MediaObject, &m_AudioOutput );
    Phonon::createPath( &m_MediaObject, m_videoWidget );
}


CCVideoView::~CCVideoView()
{
}


void CCVideoView::playVideo(const char *filePath)
{
    m_MediaObject.setCurrentSource( Phonon::MediaSource( filePath ) );
    m_MediaObject.play();
}


void CCVideoView::resizeView(int x1, int y1, int x2, int y2)
{
    int width = x2 - x1;
    int height = y2 - y1;
    setMaximumSize( width, height );
    setGeometry( x1, y1, x2, y2 );
}
