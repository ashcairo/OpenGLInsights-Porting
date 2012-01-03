/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCMathTools.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"


const bool CCEqualFloat(const float a, const float b)
{
	const float difference = fabsf( a - b );
	return difference < 0.01f;
}


const float CCSignFloat(const float value)
{
    if( value > 0.0f )
    {
        return 1.0f;
    }
    else if( value < 0.0f )
    {
        return -1.0f;
    }
    return 0.0f;
}


const float CCRandomFloat()
{
	const int random = rand();
	const float randomFloat = ( random % 11 ) * 0.1f;
	return randomFloat;
}


const float CCRandomDualFloat()
{
    return ( CCRandomFloat() - 0.5f ) * 2.0f;
}


const int CCRandomDualInt()
{
    int random = rand() % 2;
    if( random == 0 )
    {
        random = -1;
    }
    return random;
}


void CCSwapFloat(float &a, float &b)
{
    const float temp = a;
    a = b;
    b = temp;
}


void CCClampFloat(float &value, const float min, const float max)
{
    value = MAX( min, value );
    value = MIN( max, value );
}


const bool CCToRotation(float &current, float target, const float amount)
{
    if( current != target )
	{
        float incrementRotation = target - current;
        CCClampRotation( incrementRotation );
		
        float decrementRotation = current - target;
        CCClampRotation( decrementRotation );
		
		if( decrementRotation < incrementRotation )
		{
            if( target > current )
			{
				target -= 360.0f;
			}
			
            current -= amount;
            current = current < target ? target : current;
		}
		else
		{
            if( target < current )
			{
				target += 360.0f;
			}
			
            current += amount;
            current = current > target ? target : current;
		}
		
		CCClampRotation( current );
		
		return true;
	}
	
	return false;
}


const float CCLengthSquared(const float a, const float b)
{
	return ( a * a ) + ( b * b );
}


const uint CCPowerOf2(const uint value)
{
	uint32_t powerOf2 = 2;
	while( powerOf2 < value )
	{
		powerOf2 *= 2;
	}
	
	return powerOf2;
}


const float CCDistance(const float first, const float second)
{
	return fabsf( first - second );
}


void CCClampDistance(float &current, const float target, const float offset)
{
    if( CCDistance( current, target ) > offset )
	{
        if( current < target )
		{
            current = target - offset;
		}
		else
		{
            current = target + offset;
		}
	}
}


const float CCDistanceBetweenPoints(const CCPoint &first, const CCPoint &second)
{
    const float deltaX = second.x - first.x;
    const float deltaY = second.y - first.y;
    return sqrtf( deltaX * deltaX + deltaY * deltaY );
}


const float CCDistanceBetweenAngles(const float first, const float second)
{
	float distance1 = first - second;
	float distance2 = second - first;
    CCClampRotation( distance1 );
    CCClampRotation( distance2 );
	
	return distance1 < distance2 ? distance1 : distance2;
}


const float CCAngleBetweenPoints(const CCPoint &first, const CCPoint &second)
{
    // atan( ( top - bottom ) / ( right - left ) )
    const float rads = atanf( ( second.y - first.y ) / ( first.x - second.x ) );
    return CC_RADIANS_TO_DEGREES( rads );
}


const float CCAngleBetweenLines(const CCPoint &line1Start, const CCPoint &line1End, const CCPoint &line2Start, const CCPoint &line2End)
{	
    const float a = line1End.x - line1Start.x;
    const float b = line1End.y - line1Start.y;
    const float c = line2End.x - line2Start.x;
    const float d = line2End.y - line2Start.y;
	
    const float rads = acosf( ( ( a*c ) + ( b*d ) ) / ( ( sqrtf( a*a + b*b ) ) * ( sqrtf( c*c + d*d ) ) ) );
	
    return CC_RADIANS_TO_DEGREES( rads );
}


void CCClampRotation(float &rotation)
{
    while( rotation >= 360.0f )
	{
        rotation -= 360.0f;
	}
	
    while( rotation < 0.0f )
	{
        rotation += 360.0f;
	}
}


void CCRotateAboutX(CCVector3 &rotatedPosition, const float rotation, const CCVector3 &from, const CCVector3 &about)
{
	// Move to origin
	const float y = from.y - about.y;
	const float z = from.z - about.z;
	
	const float rotationInRadians = CC_DEGREES_TO_RADIANS( rotation );
	const float cosAngle = cosf( rotationInRadians );
	const float sinAngle = sinf( rotationInRadians );
	
    rotatedPosition.y = ( y * cosAngle ) - ( z * sinAngle );
    rotatedPosition.z = ( y * sinAngle ) + ( z * cosAngle );
	
    rotatedPosition.y += about.y;
    rotatedPosition.z += about.z;
}


void CCRotateAboutY(CCVector3 &rotatedPosition, const float rotation, const CCVector3 &from, const CCVector3 &about)
{
	// Move to origin
	const float x = from.x - about.x;
	const float z = from.z - about.z;
	
	const float rotationInRadians = CC_DEGREES_TO_RADIANS( rotation );
	const float cosAngle = cosf( rotationInRadians );
	const float sinAngle = sinf( rotationInRadians );
	
    rotatedPosition.x = ( x * cosAngle ) + ( z * sinAngle );
    rotatedPosition.z = -( x * sinAngle ) + ( z * cosAngle );
	
    rotatedPosition.x += about.x;
    rotatedPosition.z += about.z;
}


void CCRotateAboutXY(CCVector3 &rotatedPosition, const float rotationX, const float rotationY, const CCVector3 &from, const CCVector3 &about)
{
	// Move to origin
	const float x = from.x - about.x;
	const float y = from.y - about.y;
	const float z = from.z - about.z;
	
	const float rotationXInRadians = CC_DEGREES_TO_RADIANS( rotationX );
	const float cosAngleX = cosf( rotationXInRadians );
	const float sinAngleX = sinf( rotationXInRadians );
	
	const float rotationYInRadians = CC_DEGREES_TO_RADIANS( rotationY );
	const float cosAngleY = cosf( rotationYInRadians );
	const float sinAngleY = sinf( rotationYInRadians );
	
	rotatedPosition.y = ( y * cosAngleX ) - ( z * sinAngleX );
	rotatedPosition.z = ( y * sinAngleX ) + ( z * cosAngleX );
	
	rotatedPosition.x = ( x * cosAngleY ) + ( z * sinAngleY );
	rotatedPosition.z = -( x * sinAngleY ) + ( z * cosAngleY );
	
	rotatedPosition.x += about.x;
	rotatedPosition.y += about.y;
	rotatedPosition.z += about.z;
}


void CCRotatePoint(CCPoint &point, const float rotation)
{	
    const float x = point.x;
    const float y = point.y;
	
	const float rotationInRadians = CC_DEGREES_TO_RADIANS( rotation );
	const float cosAngle = cosf( rotationInRadians );
	const float sinAngle = sinf( rotationInRadians );
	
    point.x = ( x * cosAngle ) + ( y * sinAngle );
    point.y = -( x * sinAngle ) + ( y * cosAngle );
}


const bool CCOppositePoints(const CCPoint &pointA, const CCPoint &pointB)
{
	if( pointA.x > 0.0f )
	{
		if( pointB.x < 0.0f )
		{
			if( pointA.y > 0.0f )
			{
				if( pointB.y < 0.0f )
				{
					return true;
				}
			}
			else if( pointA.x < 0.0f )
			{
				if( pointB.y > 0.0f )
				{
					return true;
				}
			}
		}
	}
	else if( pointA.x < 0.0f )
	{
		if( pointB.x > 0.0f )
		{
			if( pointA.y > 0.0f )
			{
				if( pointB.y < 0.0f )
				{
					return true;
				}
			}
			else if( pointA.y < 0.0f )
			{
				if( pointB.y > 0.0f )
				{
					return true;
				}
			}
		}
	}
	
	return false;
}


const float CCAngleTowards(const float fromX, const float fromZ, const float toX, const float toZ)
{
	const float y = fromX - toX;
	const float x = fromZ - toZ;
	
	float angle = CC_RADIANS_TO_DEGREES( atan2f( y, x ) );
    CCClampRotation( angle );
	
	return angle;
}


const float CCAngleTowards(const CCVector3 &from, const CCVector3 &to)
{
	return CCAngleTowards( from.x, from.z, to.x, to.z );
}


void CCLockRotation(float &rotation, const float angle)
{
    const int numberOfAngles = rotation / angle;
	
	const float minRotationTarget = numberOfAngles * angle;
	const float maxRotationTarget = (numberOfAngles+1) * angle;
	
    const float differenceToMin = rotation - minRotationTarget;
    const float differenceToMax = maxRotationTarget - rotation;
	
	if( differenceToMin <= differenceToMax )
	{
        rotation = minRotationTarget;
	}
	else
	{
        rotation = maxRotationTarget;
	}
	
	CCClampRotation( rotation );
}
