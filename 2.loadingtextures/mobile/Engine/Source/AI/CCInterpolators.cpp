/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCInterpolators.cpp
 * Description : Interpolators for various curves.
 *
 * Created     : 30/04/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#include "CCDefines.h"


void CCInterpolator::setDuration(const float duration)
{
    speed = 1.0f/duration;
}



const bool CCInterpolatorCurve::equals(float *inCurrent, const float inTarget)
{ 
    // Ignore if we're already doing this
    if( current == inCurrent && target == inTarget )
    {
        return true;
    }
    return false;
}


void CCInterpolatorCurve::setup(float *inCurrent, const float inTarget)
{
    setup( inCurrent );
    setTarget( inTarget );
}


void CCInterpolatorCurve::setup(float *inCurrent)
{
    current = inCurrent;
}


void CCInterpolatorCurve::setTarget(float inTarget)
{
    target = inTarget;
    ready();
}


void CCInterpolatorCurve::ready()
{
    if( current != NULL )
    {
        start = *current;
        length = target - start;
        amount = 0.0f;
    }
}


const bool CCInterpolatorCurve::update(const float delta)
{
    if( current != NULL )
    {
        if( CCToTarget( amount, 1.0f, delta * speed ) )
        {
            static const float interpolationCurveMultiple = 1.0f / sinf( 2.0f );
            const float percentage = sinf( amount * 2.0f ) * interpolationCurveMultiple;
            const float movement = length * percentage;
            *current = movement + start;
            return true;
        }
        else if( *current != target )
        {
            *current = target;
            current = NULL;
            return true;
        }
    }
    else if( onInterpolated.length > 0 )
    {
        LAMBDA_SIGNAL pendingCallbacks;
        for( int i=0; i<onInterpolated.length; ++i )
        {
            pendingCallbacks.add( onInterpolated.list[i] );
        }
        onInterpolated.length = 0;
        LAMBDA_EMIT_ONCE( pendingCallbacks );
    }

    return false;
}



const bool CCInterpolatorX2Curve::update(const float delta)
{
    if( CCToTarget( amount, 1.0f, delta * speed ) )
    {
        const float percentage = amount * amount;
        const float movement = length * percentage;
        *current = movement + start;
        return true;
    }
    else if( current && *current != target )
    {
        *current = target;
        return true;
    }
    return false;
}


const bool CCInterpolatorX3Curve::update(const float delta)
{
    if( CCToTarget( amount, 1.0f, delta * speed ) )
    {
        const float sudoAmount = amount - 1.0f;
        const float percentage = 1.0f + ( sudoAmount * sudoAmount * sudoAmount );
        const float movement = length * percentage;
        *current = movement + start;
        return true;
    }
    else if( current && *current != target )
    {
        *current = target;
        return true;
    }
    return false;
}



const bool CCInterpolatorSinCurve::update(const float delta)
{
    if( CCToTarget( amount, 1.0f, delta * speed ) )
    {
        static const float interpolationCurveMultiple = 1.0f / sinf( CC_HPI );
        const float percentage = sinf( amount * CC_HPI ) * interpolationCurveMultiple;
        const float movement = length * percentage;
        *current = movement + start;
        return true;
    }
    else if( current && *current != target )
    {
        *current = target;
        return true;
    }
    return false;
}



const bool CCInterpolatorCurvesV3::pushV3(CCVector3 *inCurrent, const CCVector3 target, const bool replace)
{
    if( interpolators.length > 0 )
    {
        if( replace )
        {
            bool found = false;
            for( int i=0; i<interpolators.length; ++i )
            {
                CCInterpolatorCurveV3<CCInterpolatorCurve> *interpolator = interpolators.list[i];
                if( interpolator->equals( inCurrent, target ) )
                {
                    found = true;
                    if( i != 0 )
                    {
                        interpolator->ready();
                    }
                }
                else
                {
                    interpolators.remove( interpolator );
                    delete interpolator;
                }
            }
            
            if( found )
            {
                return false;
            }
        }
        else
        {
            for( int i=0; i<interpolators.length; ++i )
            {
                CCInterpolatorCurveV3<CCInterpolatorCurve> *interpolator = interpolators.list[i];
                if( interpolator->equals( inCurrent, target ) )
                {
                    return false;
                }
            }
        }
    }
    
    if( *inCurrent != target )
    {
        interpolators.add( new CCInterpolatorCurveV3<CCInterpolatorCurve>( inCurrent, target ) );
    }
    return true;
}


const bool CCInterpolatorCurvesV3::update(const float delta)
{
    if( interpolators.length > 0 )
    {
        CCInterpolatorCurveV3<CCInterpolatorCurve> *interpolator = interpolators.list[0];
        if( interpolator->update( delta * speed ) == false )
        {
            interpolators.remove( interpolator );
            DELETE_OBJECT( interpolator );
            
            // If there's another interpolation planned, tell it to ready itself to interpolate
            if( interpolators.length > 0 )
            {
                interpolators.list[0]->ready();
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;
}


const bool CCInterpolatorLinearColour::update(const float delta)
{
    if( current != NULL )
    {
        if( updating )
        {
            if( current->toTarget( target, delta * speed ) )
            {
                return true;
            }
            else
            {
                updating = false;
            }
        }
        else 
        {
            ASSERT( current->equals( target ) );
            if( onInterpolated.length > 0 )
            {
                LAMBDA_SIGNAL pendingCallbacks;
                for( int i=0; i<onInterpolated.length; ++i )
                {
                    pendingCallbacks.add( onInterpolated.list[i] );
                }
                onInterpolated.length = 0;
                LAMBDA_EMIT_ONCE( pendingCallbacks );
            }
        }
    }
    return false;
}


const bool CCInterpolatorLinearV3::update(const float delta)
{
    if( current->toTarget( target, delta * speed ) )
    {
        return true;
    }
    else if( onInterpolated.length > 0 )
    {
        LAMBDA_SIGNAL pendingCallbacks;
        for( int i=0; i<onInterpolated.length; ++i )
        {
            pendingCallbacks.add( onInterpolated.list[i] );
        }
        onInterpolated.length = 0;
        LAMBDA_EMIT_ONCE( pendingCallbacks );
    }
    return false;
}
