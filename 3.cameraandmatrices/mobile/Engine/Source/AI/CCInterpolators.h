/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCInterpolators.h
 * Description : Interpolators for various curves.
 *
 * Created     : 30/04/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef __CCINTERPOLATORS_H__
#define __CCINTERPOLATORS_H__


class CCInterpolator : public CCUpdater
{
public:
    CCInterpolator()
    {
        speed = 1.0f;
    }

    virtual ~CCInterpolator()
    {
#ifdef DEBUGON
        destructCalled = true;
#endif

        onInterpolated.deleteObjectsAndList();
    }
    
    void setDuration(const float duration);

public:
    LAMBDA_SIGNAL onInterpolated;
    
protected:
    float speed;
};



class CCInterpolatorCurve : public CCInterpolator
{
public:
    CCInterpolatorCurve()
    {  
        current = NULL;
        amount = 1.0f;
    }
    
    CCInterpolatorCurve(float *inCurrent, const float inTarget)
    {
        setup( inCurrent, inTarget);
    }
    
    const bool equals(float *inCurrent, const float inTarget);
    void setup(float *inCurrent, const float inTarget);

    void setup(float *inCurrent);
    void setTarget(float inTarget);
    
    // Call when ready to start the interpolation
    void ready();
    
    virtual const bool update(const float delta);
    
    inline const float* getCurrent() const { return current; }
    inline const float getAmount() const { return amount; }
    inline const float getTarget() const { return target; }
    
protected:
    float *current;
    float target;
    float start;
    float length;
    float amount;
};



class CCInterpolatorX2Curve : public CCInterpolatorCurve
{
public:
    const bool update(const float delta);
};


class CCInterpolatorX3Curve : public CCInterpolatorCurve
{
public:
    const bool update(const float delta);
};


class CCInterpolatorSinCurve : public CCInterpolatorCurve
{
public:
    const bool update(const float delta);
};



class CCInterpolatorCurveXY : public CCInterpolator
{
public:
    void setup(float *inX, float *inY, const float target)
    {
        x.setup( inX, target );
        y.setup( inY, target );
    }
    
    void setup(float *inX, float *inY, const float targetX, const float targetY)
    {
        x.setup( inX, targetX );
        y.setup( inY, targetY );
    }
    
    const bool update(const float delta)
    {
        float deltaSpeed = delta * speed;
        bool updating = x.update( deltaSpeed );
        updating |= y.update( deltaSpeed );
        return updating;
    }
    
protected:
    CCInterpolatorCurve x,y;
};



template <typename T> class CCInterpolatorCurveV3 : public CCInterpolator
{
public:
    CCInterpolatorCurveV3() {}
    
    CCInterpolatorCurveV3(CCVector3 *inCurrent, const CCVector3 target)
    {
        setup( inCurrent, target );
    }
    
    const bool equals(CCVector3 *inCurrent, const CCVector3 target)
    {
        if( x.equals( &inCurrent->x, target.x ) &&
            y.equals( &inCurrent->y, target.y ) &&
            z.equals( &inCurrent->z, target.z ) )
        {
            return true;
        }
        return false;
    }
    
    void clear()
    {
        x.setup( NULL );
        y.setup( NULL );
        z.setup( NULL );
    }

    
    void setup(CCVector3 *inCurrent, const CCVector3 target, CCLambdaCallback *inCallback=NULL)
    {
        x.setup( &inCurrent->x, target.x );
        y.setup( &inCurrent->y, target.y );
        z.setup( &inCurrent->z, target.z );
        
        onInterpolated.deleteObjects();
        if( inCallback != NULL )
        {
            onInterpolated.add( inCallback );
        }
    }
    
    void setup(CCVector3 *inCurrent, const float target)
    {
        x.setup( &inCurrent->x, target );
        y.setup( &inCurrent->y, target );
        z.setup( &inCurrent->z, target );
    }
    
    void ready()
    {
        x.ready();
        y.ready();
        z.ready();
    }
    
    const bool update(const float delta)
    {
        float deltaSpeed = delta * speed;
        bool updating = x.update( deltaSpeed );
        updating |= y.update( deltaSpeed );
        updating |= z.update( deltaSpeed );
        
        if( onInterpolated.length > 0 )
        {
            if( updating == false )
            {
                LAMBDA_EMIT_ONCE( onInterpolated );
            }
        }
        
        return updating;
    }
    
    const CCVector3 getAmount() const { return CCVector3( x.getAmount(), y.getAmount(), z.getAmount() ); }
    const CCVector3 getTarget() const { return CCVector3( x.getTarget(), y.getTarget(), z.getTarget() ); }
    
protected:
    T x,y,z;
};



class CCInterpolatorCurvesV3 : public CCInterpolator
{
public:
    ~CCInterpolatorCurvesV3()
    {
        interpolators.deleteObjectsAndList();
    }
    
    const bool pushV3(CCVector3 *inCurrent, const CCVector3 target, const bool replace=false);    
    const bool update(const float delta);
    
    const bool finished() { return interpolators.length == 0; }
    
public:
    CCList< CCInterpolatorCurveV3<CCInterpolatorCurve> > interpolators;
};



class CCInterpolatorLinearV3 : public CCInterpolator
{
public:
    CCInterpolatorLinearV3() {}
    
    CCInterpolatorLinearV3(CCVector3 *inCurrent, const CCVector3 inTarget, CCLambdaCallback *inCallback=NULL)
    {
        setup( inCurrent, inTarget );

        onInterpolated.deleteObjects();
        if( inCallback != NULL )
        {
            onInterpolated.add( inCallback );
        }
    }
    
    const bool equals(CCVector3 *inCurrent, const CCVector3 inTarget)
    { 
        // Ignore if we're already doing this
        if( current != inCurrent || target != inTarget )
        {
            return false;
        }
        return true;
    }
    
    void setup(CCVector3 *inCurrent, const CCVector3 inTarget)
    {
        current = inCurrent;
        target = inTarget;
    }
    
    const bool update(const float delta);
    
    inline const CCVector3& getTarget() const { return target; }
    
protected:
    CCVector3 *current;
    CCVector3 target;
};


class CCInterpolatorLinearColour : public CCInterpolator
{
public:
    CCInterpolatorLinearColour()
    {
        current = NULL;
        updating = false;
    }
    
    CCInterpolatorLinearColour(CCColour *inCurrent, const CCColour inTarget)
    {
        setup( inCurrent, inTarget );
    }
    
    const bool equals(CCColour *inCurrent, const CCColour inTarget)
    { 
        // Ignore if we're already doing this
        if( current != inCurrent || target != inTarget )
        {
            return false;
        }
        return true;
    }
    
    void setup(CCColour *inCurrent, const CCColour inTarget)
    {
        current = inCurrent;
        target = inTarget;
        updating = true;
    }
    
    const bool update(const float delta);
    
    void setTarget(const CCColour inTarget, CCLambdaCallback *inCallback=NULL)
    {
        setup( current, inTarget );
        if( onInterpolated.length > 0 )
        {
            onInterpolated.deleteObjects();
        }
        if( inCallback != NULL )
        {
            onInterpolated.add( inCallback );
        }
    }

    void setTarget(const CCColour *inTarget, CCLambdaCallback *inCallback=NULL)
    {
        setTarget( *inTarget, inCallback );
    }
    
    void setTargetColour(const float grey)
    {
        target.red = grey;
        target.green = grey;
        target.blue = grey;
        updating = true;
    }

    void setTargetAlpha(const float inTargetAlpha, CCLambdaCallback *inCallback=NULL)
    {
        if( target.alpha != inTargetAlpha )
        {
            target.alpha = inTargetAlpha;
            setTarget( target, inCallback );
        }
        else
        {
            if( inCallback != NULL )
            {
                onInterpolated.add( inCallback );
            }
        }
    }

    inline const CCColour& getTarget() const { return target; }
    
protected:
    bool updating;
    CCColour *current;
    CCColour target;
};



class CCInterpolatorLinearsV3 : public CCInterpolator
{
public:
    ~CCInterpolatorLinearsV3()
    {
        interpolators.deleteObjectsAndList();
    }

    void clear()
    {
        while( interpolators.length > 0 )
        {
            CCInterpolatorLinearV3 *interpolator = interpolators.list[0];
            interpolators.remove( interpolator );
            delete interpolator;
        }
    }
    
    // replace: Deletes all the other pending interpolations and pushes this target to the front
    const bool pushV3(CCVector3 *inCurrent, const CCVector3 target, const bool replace=false, CCLambdaCallback *inCallback=NULL)
    {
        if( interpolators.length > 0 )
        {
            if( replace )
            {
                bool found = false;
                for( int i=0; i<interpolators.length; ++i )
                {
                    CCInterpolatorLinearV3 *interpolator = interpolators.list[0];
                    if( interpolator->equals( inCurrent, target ) )
                    {
                        found = true;
                    }
                    else
                    {
                        interpolators.remove( interpolator );
                        delete interpolator;
                    }
                }
                
                if( found )
                {
                    return false ;
                }
            }
        }
        
        interpolators.add( new CCInterpolatorLinearV3( inCurrent, target, inCallback ) );
        return true;
    }
    
    const bool update(const float delta)
    {
        if( interpolators.length > 0 )
        {
            CCInterpolatorLinearV3 *interpolator = interpolators.list[0];
            if( interpolator->update( delta * speed ) == false )
            {
                interpolators.remove( interpolator );
                delete interpolator;
                
                return interpolators.length > 0;
            }
            return true;
        }
        return false;
    }
    
    const bool finished() { return interpolators.length == 0; }
    
    CCList<CCInterpolatorLinearV3> interpolators;
};


#endif // __CCInterpolatorS_H__
