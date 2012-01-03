/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCControls.h
 * Description : Cross platform controls interface.
 *
 * Created     : 01/03/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCCONTROLS_H__
#define __CCCONTROLS_H__


#define CC_DOUBLE_TAP_THRESHOLD 0.2f
#define CC_TOUCH_TO_MOVEMENT_THRESHOLD 0.025f
#define CC_MIN_MOVEMENT_THRESHOLD 0.1f


enum CCTouchAction
{
	touch_pressed,
    touch_movingHorizontal,
    touch_movingVertical,
	touch_moving,
	touch_released,
	touch_lost
};


struct CCScreenTouches
{
	CCScreenTouches()
	{
		lastTouch = usingTouch = NULL;	
		timeHeld = lastTimeReleased = 0.0f;
	}
	
    const CCPoint averageLastDeltas() const;
	
	void *usingTouch, *lastTouch;
    CCPoint startPosition, position, delta, totalDelta, lastTotalDelta;
	float timeHeld, lastTimeReleased;
	
    enum { max_last_deltas = 100 };
    struct TimedDelta
    {
        TimedDelta()
        {
            time = 0.0f;
        }

        void clear()
        {
            time = 0.0f;
            delta = CCPoint();
        }

        float time;
        CCPoint delta;
    };
    TimedDelta lastDeltas[max_last_deltas];
};



class CCControls
{
public:
	CCControls();
	
	void render();
	
	// Synchronizes the controls
	void update(const CCTime &gameTime);
    
protected:
   	void unTouch(void *touch);
    
public:
	const bool detectZoomTouch();
    static const bool touchActionMoving(const CCTouchAction touchAction);
	
public:
	bool inUse;
	enum { numberOfTouches = 2 };
	CCScreenTouches screenTouches[numberOfTouches];
};


#endif // __CONTROLS_H_
