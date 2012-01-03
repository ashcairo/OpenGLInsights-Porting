/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCSceneAppUI.js
 * Description : AppUI scene template.
 *
 * Created     : 20/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCSceneAppUI()
{
    this.construct();
}
CopyPrototype( CCSceneAppUI, CCSceneBase, "CCSceneBase" );


CCSceneAppUI.prototype.construct = function()
{
	this.CCSceneBase_construct();
	
	this.resizeCallbacks = new Array();
	this.resizing = false;
    
    this.scrollBar = false;
}


CCSceneAppUI.prototype.setup = function()
{
}


CCSceneAppUI.prototype.resize = function()
{   
	var resizeCallbacks = this.resizeCallbacks;
	var length = this.resizeCallbacks.length;
    for( var i=0; i<length; ++i )
    {
        resizeCallbacks[i].run();
    }
    
    this.resizing = true;
}


CCSceneAppUI.prototype.update = function(delta)
{
	this.CCSceneBase_update( delta );
}


CCSceneAppUI.prototype.render = function(renderer, pass, alpha)
{
	this.CCSceneBase_render( renderer, pass, alpha );
}
