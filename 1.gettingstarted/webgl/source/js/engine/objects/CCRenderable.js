/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCRenderable.js
 * Description : A renderable component.
 *
 * Created     : 23/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCRenderable()
{
	this.construct();
}


CCRenderable.prototype.construct = function()
{	
	this.shouldRender = true;
}


CCRenderable.prototype.destruct = function()
{
}


CCRenderable.prototype.refreshCollisions = function()
{
}
