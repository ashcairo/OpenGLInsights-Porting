/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCDeviceControls.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCDeviceControls.h"


CCDeviceControls::CCDeviceControls()
{
}


void CCDeviceControls::touchBegin(UITouch *touch)
{
    touchHandle( touch );

    inUse = false;
    for( uint i=0; i<numberOfTouches; ++i )
    {
        inUse |= screenTouches[i].usingTouch != NULL;
    }
}


void CCDeviceControls::touchMove(UITouch *touch)
{
    if( inUse )
    {
        touchHandle( touch );
    }
}


void CCDeviceControls::touchEnd(UITouch *touch)
{
    unTouch( touch );
    inUse = false;
}


void CCDeviceControls::touchHandle(UITouch *touch)
{
    const CCSize &screenSizeMultiple = gEngine->renderer->screenSizeMultiple;

    CCScreenTouches& screenTouch = screenTouches[0];

    CCPoint position = touch->pos;
    position.x *= screenSizeMultiple.width;
    position.y *= screenSizeMultiple.height;

    if( gEngine->renderer->orientation.target == 270.0f )
    {
        CCSwapFloat( position.x, position.y );
        position.y = 1.0f - position.y;
    }
    else if( gEngine->renderer->orientation.target == 90.0f )
    {
        CCSwapFloat( position.x, position.y );
        position.x = 1.0f - position.x;
    }
    else if( gEngine->renderer->orientation.target == 180.0f )
    {
        position.x = 1.0f - position.x;
        position.y = 1.0f - position.y;
    }

    if( screenTouch.usingTouch != NULL )
    {
        screenTouch.delta.x += position.x - screenTouch.position.x;
        screenTouch.delta.y += position.y - screenTouch.position.y;
        screenTouch.totalDelta.x += screenTouch.delta.x;
        screenTouch.totalDelta.y += screenTouch.delta.y;
    }
    else
    {
        // Restart our counters
        screenTouch.totalDelta.x = 0.0f;
        screenTouch.totalDelta.y = 0.0f;
        screenTouch.timeHeld = 0.0f;
        screenTouch.startPosition = position;

        for( uint i=0; i<CCScreenTouches::max_last_deltas; ++i )
        {
            screenTouch.lastDeltas[i].clear();
        }
        screenTouch.usingTouch = touch;
    }
    screenTouch.position = position;
}



void CCDeviceControls::touchBegin(QList<QTouchEvent::TouchPoint> touches)
{
    touchHandle( touches );

    inUse = false;
    for( uint touchIndex=0; touchIndex<numberOfTouches; ++touchIndex )
    {
        inUse |= screenTouches[touchIndex].usingTouch != NULL;
    }
}


void CCDeviceControls::touchMove(QList<QTouchEvent::TouchPoint> touches)
{
    if( inUse )
    {
        touchHandle( touches );
    }
}


void CCDeviceControls::touchEnd(QList<QTouchEvent::TouchPoint> touches)
{
    for( int touchIndex=0; touchIndex<touches.size(); ++touchIndex )
    {
        QTouchEvent::TouchPoint touchPoint = touches.at( touchIndex );
        const int touchID = touchPoint.id();

        // Fill in our screen touches
        for( uint i=0; i<numberOfTouches; ++i )
        {
            CCScreenTouches &screenTouch = screenTouches[i];
            if( screenTouch.usingTouch != NULL )
            {
                UITouch *uiTouch = (UITouch*)screenTouch.usingTouch;
                if( uiTouch->touchID == touchID )
                {
                    unTouch( screenTouch.usingTouch );
                }
            }
        }
    }

    // Check if nothing else is using the touch
    for( int i=0; i<numberOfTouches; ++i )
    {
        CCScreenTouches &screenTouch = screenTouches[i];
        if( screenTouch.usingTouch != NULL )
        {
            return;
        }
    }
    inUse = false;
}


void CCDeviceControls::touchHandle(QList<QTouchEvent::TouchPoint> touches)
{
    const CCSize &screenSizeMultiple = gEngine->renderer->screenSizeMultiple;

    for( int touchIndex=0; touchIndex<touches.size(); ++touchIndex )
    {
        QTouchEvent::TouchPoint touchPoint = touches.at( touchIndex );
        const int touchID = touchPoint.id();

        // Fill in our screen touches
        for( uint i=0; i<numberOfTouches; ++i )
        {
            CCScreenTouches &screenTouch = screenTouches[i];
            UITouch *uiTouch = (UITouch*)screenTouch.usingTouch;
            if( uiTouch == NULL || uiTouch->touchID == touchID )
            {
                CCEngineThreadLock();

                QPointF qPosition = touchPoint.pos();
                CCPoint position( qPosition.x(), qPosition.y() );

                // Range between 0.0f - 1.0f
                position.x *= screenSizeMultiple.width;
                position.y *= screenSizeMultiple.height;

                if( gEngine->renderer->orientation.target == 270.0f )
                {
                    CCSwapFloat( position.x, position.y );
                    position.y = 1.0f - position.y;
                }
                else if( gEngine->renderer->orientation.target == 90.0f )
                {
                    CCSwapFloat( position.x, position.y );
                    position.x = 1.0f - position.x;
                }
                else if( gEngine->renderer->orientation.target == 180.0f )
                {
                    position.x = 1.0f - position.x;
                    position.y = 1.0f - position.y;
                }

                // Restart our counters
                if( screenTouch.usingTouch == NULL )
                {
                    screenTouch.totalDelta.x = 0.0f;
                    screenTouch.totalDelta.y = 0.0f;
                    screenTouch.timeHeld = 0.0f;
                    screenTouch.startPosition = position;
                    for( uint i=0; i<CCScreenTouches::max_last_deltas; ++i )
                    {
                        screenTouch.lastDeltas[i].clear();
                    }

                    UITouch *touch = getUnusedUITouch();
                    touch->pos = position;
                    touch->touchID = touchID;
                    screenTouch.usingTouch = touch;
                }

                // if( screenTouch.usingTouch->touchID == touchID )
                else
                {
                    screenTouch.delta.x += position.x - screenTouch.position.x;
                    screenTouch.delta.y += position.y - screenTouch.position.y;
                    screenTouch.totalDelta.x += screenTouch.delta.x;
                    screenTouch.totalDelta.y += screenTouch.delta.y;
                }

                screenTouch.position = position;

                CCEngineThreadUnlock();
                break;
            }
        }
    }
}


UITouch* CCDeviceControls::getUnusedUITouch()
{
    for( int touchIndex=0; touchIndex<numberOfTouches; ++touchIndex )
    {
        UITouch *uiTouch = &uiTouches[touchIndex];
        bool used = false;
        for( int i=0; i<numberOfTouches; ++i )
        {
            CCScreenTouches &screenTouch = screenTouches[i];
            if( screenTouch.usingTouch == uiTouch )
            {
                used = true;
                break;
            }
        }

        if( used == false )
        {
            return uiTouch;
        }
    }
    ASSERT( false );
    return NULL;
}
