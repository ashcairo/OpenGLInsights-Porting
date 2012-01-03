/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCRenerable.h
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "CCRenderable.h"


CCRenderable::CCRenderable()
{
    shouldRender = true;
}


void CCRenderable::destruct()
{
}