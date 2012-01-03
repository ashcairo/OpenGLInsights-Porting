/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCAppEngine.cpp
 *-----------------------------------------------------------
 */

#include "CCDefines.h"
#include "Samples.h"


void CCAppEngine::start()
{
    addScene( new SceneSample() );
    
    //AddFlag( gEngine->renderFlags, collision_box );
}
