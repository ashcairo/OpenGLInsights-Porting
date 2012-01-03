/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : CCControls.js
 * Description : Controls interface.
 *
 * Created     : 05/11/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */
 
var touch_pressed = 1;
var touch_movingHorizontal = 2;
var touch_movingVertical = 3;
var touch_moving = 4;
var touch_released = 5;
var touch_lost = 6;
var CC_TOUCH_TO_MOVEMENT_THRESHOLD = 0.05;

function CCControls()
{
}


CCControls.touchActionMoving = function(touchAction)
{
    if( touchAction >= touch_movingHorizontal && touchAction <= touch_moving )
    {
        return true;
    }
    return false;
}

// Converts our UI controls to relative space for our scenes
// TODO: Controls
function Touch()
{
    this.x = 0.0;
    this.y = 0.0;
    this.startX = 0.0;
    this.startY = 0.0;
    this.deltaX = 0.0;
    this.deltaY = 0.0;
    this.totalDeltaX = 0.0;
    this.totalDeltaY = 0.0;

    this.touching = false;
    this.timeHeld = 0.0;
}


function MouseDown(event)
{        
	return gUI.mouseDown( event );
}

function MouseUp( event )
{
	gUI.mouseUp( event );
}

function MouseOut(event)
{
	gUI.mouseOut( event );
}

function MouseMove(event)
{
	gUI.mouseMove( event );
}

function MouseWheel(event)
{
	var event = event;
    var delta = 0;
    
    // For IE.
    if( !event ) 
    {
        event = window.event;
    }
    
    // IE/Opera.
    if( event.wheelDelta ) 
    { 
        if( gBrowserTypesArray['chrome'] )
        {
            delta = event.wheelDelta / 360;
        }
        else
        {    
            delta = event.wheelDelta / 120;
        }
    } 
    else if( event.detail ) 
    { 
        // In Mozilla, sign of delta is different than in IE, also, delta is multiple of 3.
        delta = -event.detail/3;
    }

    var bReturn = true;    
    if( bReturn == false )
    {
        if( event.preventDefault )
        {
            event.preventDefault();
        }
        event.returnValue = false;
        return false;
    }
    
    return true;
}

function KeyUp(event)
{
	var event = event;
    if( !event ) 
    {
        event = window.event;
    }
	
	if( this.onChange )
	{
		this.onChange();
	}

    if( event.keyCode == 13 )
	{
		this.onSelect();
	}
}

function TouchMove(event)
{
	gUI.touchMove( event );
}

function TouchEnd(event)
{
	gUI.touchEnd( event );
}

function TouchCancel(event)
{
}

function GestureStart(event)
{
}

function GestureChange(event)
{
}

function GestureEnd(event)
{
}

function UI(engine)
{
	this.engine = engine;
	
	this.x = 0;
	this.y = 0;
	
	this.startX = 0;
	this.startY = 0;
	
	this.allowDefaults = false;
	
	this.documentScroller = document.documentElement.scrollTop ? document.documentElement : document.body;
	
	this.tolerance = 0;
	
	// Set the approperate mouse handlers
	if( gBrowserTypes['mobile'] )
	{
		this.tolerance = 20;
		
		document.addEventListener( 'touchstart', function(event) {
				gUI.touchStart( event );
			}, false );

		document.addEventListener( 'touchmove', TouchMove, false );
		document.addEventListener( 'touchend', TouchEnd, false );
		document.addEventListener( 'touchcancel', TouchCancel, false );
		
		document.addEventListener( 'gesturestart', GestureStart, true );
		document.addEventListener( 'gesturechange', GestureChange, true );
		document.addEventListener( 'gestureend', GestureEnd, true );
	}
	else
	{
		document.onmousemove = MouseMove;
		document.onmousedown = MouseDown;
		document.onmouseup = MouseUp;
		
		window.onmousewheel = document.onmousewheel = MouseWheel;
		if( window.addEventListener )
		{
			window.addEventListener( 'DOMMouseScroll', MouseWheel, true );
		}
	}
}

UI.prototype.leftClick = function(event)
{
	var event = event;
    var leftClick = false;
    if( !event ) 
    {
        event = window.event;
    }
    
    if( !event )
	{
        return false;
	}
    
    if( event.which )
    {
        leftClick = ( event.which == 1 );
    }
    else if( event.button )
    {
        leftClick = ( event.button == 1 ) || ( event.button == 0 );
    }
    
    return leftClick;
}

UI.prototype.mouseUpdate = function(event)
{
	var event = event;
	if( !event )
    {
        event = window.event;
    }
    
    var x, y;
    if( event.pageX )
    {
        x = event.pageX;
    }
    else
    {   
        x = event.clientX;
        x += this.documentScroller.scrollLeft;
    }    
    
    if( event.mouseY )
    {
        y = event.pageY;
    }
    else
    {   
        y = event.clientY;
        y += this.documentScroller.scrollTop;
    }
    this.x = x;
    this.y = y;
}

UI.prototype.touchUpdate = function(event)
{
	var touch = event.touches[0];
	if( touch )
	{            
		this.x = touch.clientX;
		this.y = touch.clientY;
	}
}

UI.prototype.isMouseOver = function(testObject, mouseObject)
{
    var i=0;
    while( mouseObject )
    {
        if( testObject == mouseObject )
        {
            return true;
        }
        
        mouseObject = mouseObject.parentNode;
        
        if( ++i>1000 )
        {
            alert( 'UH OH ' + mouseObject + ' ' + mouseObject.innerHTML );
            return false;
        }
    }
    
    return false;
}

UI.prototype.isMouseOverObject = function(target)
{	
	return this.isMouseOverObjectTolerated( target, this.tolerance, this.tolerance );
}

UI.prototype.isMouseOverObjectTolerated = function(target, xTolerance, yTolerance)
{	
	var target = target;
	var dimensions = target.dimensions;
	var table = target.table;
	var x = this.x;
	var y = this.y;
	
	var tools = gTools;
    var x1 = dimensions.x;
    var y1 = dimensions.y;
    var x2 = x1 + dimensions.totalWidth;
    var y2 = y1 + dimensions.totalHeight;
	
	x1 -= xTolerance;
	y1 -= yTolerance;
	x2 += xTolerance;
	y2 += yTolerance;
	
	//alert( x + ' ' + y + ' ' + x1 + ' ' + y1 + ' ' + x2 + ' ' + y2 );
    if( x > x1 && x < x2 && y > y1 && y < y2 )
	{
		this.mouseOverObject = target;
		return true;
	}
	
	return false;
}

UI.prototype.mouseDown = function(event)
{
	var event = event;
	if( !event ) 
    {
        event = window.event;
    }
    if( !this.leftClick( event ) )
	{
        return true;
	}
	this.clicking = true;

	this.mouseUpdate( event );
    this.startX = this.x;
    this.startY = this.y;
	
	if( this.engine.handleControls( this, touch_pressed ) )
    {
        if( event )
        {            
            if( event.preventDefault )
            {
                event.preventDefault();
            }
            event.returnValue = false;
        }
        return false;
    }
	else if( window.focusedInput )
	{
		var target = event.target || event.srcElement;
		if( target != window.focusedInput )
		{
			window.focusedInput.blur();
			window.focusedInput = false;
		}
	}
	
    return true;
}

UI.prototype.touchStart = function(event)
{
	this.touchUpdate( event );
    this.startX = this.x;
    this.startY = this.y;
	
	if( this.engine.handleControls( this, touch_pressed ) )
	{
		event.preventDefault();
	}
}

UI.prototype.mouseUp = function(event)
{
    if( !this.leftClick( event ) )
        return;
    
    this.engine.handleControls( this, touch_released );
    this.clicking = false;
}

UI.prototype.touchEnd = function(event)
{
	if( this.engine.handleControls( this, touch_released ) )
	{
		event.preventDefault();
	}
}

UI.prototype.mouseMove = function(event)
{
	if( !this.clicking )
        return;

	this.mouseUpdate( event );
	this.engine.handleControls( this, touch_moving );
	
	//this.engine.updateOnce();
}

UI.prototype.touchMove = function(event)
{
	this.touchUpdate( event );
	if( this.engine.handleControls( this, touch_moving ) )
	{
		event.preventDefault();
	}
}
