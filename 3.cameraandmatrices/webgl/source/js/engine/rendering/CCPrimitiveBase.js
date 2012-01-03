/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCPrimitiveBase.js
 * Description : Base drawable component.
 *
 * Created     : 23/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCPrimitiveBase()
{
}


CCPrimitiveBase.prototype.construct = function()
{
    this.textureIndex = 0;
}


CCPrimitiveBase.prototype.setTexture = function(src)
{
    this.textureIndex = gEngine.textureManager.assignTextureIndex( src );
}


CCPrimitiveBase.prototype.render = function(renderer)
{
    gEngine.textureManager.setTextureIndex( this.textureIndex );
    this.renderVertices( renderer );
}


CCPrimitiveBase.prototype.renderOutline = function(renderer)
{
}