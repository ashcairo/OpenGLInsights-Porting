/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCBaseTypes.js
 * Description : Contains base functions.
 *
 * Created     : 30/11/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function DetectBrowser() 
{ 
    var BO = new Object(); 
    BO['ie']        	= navigator.appName == 'Microsoft Internet Explorer'; 
    BO['ie6']       	= BO['ie'] && (document.implementation != null) && (document.implementation.hasFeature != null); 
    /*@cc_on
    BO['ie7']       	= @_jscript_version == '5.7';
    @*/ 
    BO['ns4']       	= !BO['ie'] &&  (document.layers != null) &&  (window.confirm != null) && (document.createElement == null); 
    BO['opera']     	= (self.opera != null); 
    BO['gecko']     	= (document.getBoxObjectFor != null); 
    BO['khtml']     	= (navigator.vendor == 'KDE'); 
    BO['konq']      	= ((navigator.vendor == 'KDE') || (document.childNodes) && (!document.all) && (!navigator.taintEnabled)); 
    BO['chrome']     	= navigator.userAgent.toLowerCase().indexOf('chrome') > -1;
    BO['safari']    	= (document.childNodes) && (!document.all) && (!navigator.taintEnabled) && (!navigator.accentColorName) && !BO['chrome']; 
    BO['safari1.2'] 	= (parseInt(0).toFixed == null) && (BO['safari'] && (window.XMLHttpRequest != null)); 
    BO['safari2.0'] 	= (parseInt(0).toFixed != null) && BO['safari'] && !BO['safari1.2']; 
    BO['safari1.1'] 	= BO['safari'] && !BO['safari1.2'] && !BO['safari2.0']; 
    BO['firefox']    	= navigator.userAgent.indexOf('Firefox') > -1;
	
	BO['iphone']    	= navigator.userAgent.toLowerCase().indexOf('iphone') > -1;
    BO['ipad']    		= navigator.userAgent.toLowerCase().indexOf('ipad') > -1;
    BO['android']    	= navigator.userAgent.toLowerCase().indexOf('android') > -1;
	
    BO['mobile']    	= BO['iphone'] || BO['android'] || BO['iPad'];
    
    return BO; 
} 
var gBrowserTypes = new DetectBrowser();
var gUI = false;
var gTools = false;
var gEngine = false;

function CopyPrototype(descendant, parent, copyFunctionsWithPrefix) 
{  
	var sConstructor = parent.toString();  
	var aMatch = sConstructor.match( /\s*function (.*)\(/ );  
									
	if( aMatch != null )
	{ 
		descendant.prototype[aMatch[1]] = parent; 
	}  
	
    for( var m in parent.prototype ) 
    {  
        if( copyFunctionsWithPrefix )
        {
            var combined = copyFunctionsWithPrefix + '_' + m;
            descendant.prototype[combined] = parent.prototype[m];
        }
        descendant.prototype[m] = parent.prototype[m];  
    }
};


function ObjectDimensions(object)
{
	this.object = object;
									
	this.x = 0;
	this.y = 0;
	this.width = 1;
	this.height = 1;
	this.totalWidth = 1;
	this.totalHeight = 1;
	
	this.targetX = 0;
	this.targetY = 0;
	this.targetWidth = 1;
	this.targetHeight = 1;
	
	this.minSize = 1;
	
	this.borderSize = 0;
}

									
ObjectDimensions.prototype.refreshBorder = function()
{
	this.borderSize = this.object.border ? parseInt( this.object.border, 10 ) : 0;
}

									
ObjectDimensions.prototype.refreshPosition = function()
{
	var dimensions = this;
	var tools = gTools;
	dimensions.x = tools.getX( this.object );
	dimensions.y = tools.getY( this.object );
}


ObjectDimensions.prototype.refreshSize = function()
{
	var dimensions = this;
	
	var width = this.object.clientWidth;
	var height = this.object.clientHeight;

	if( width < 1 )
	{
		width = 1;
	}
	if( height < 1 )
	{
		height = 1;
	}
	dimensions.width = width;
	dimensions.height = height;

	dimensions.totalWidth = width + dimensions.borderSize * 2;
	dimensions.totalHeight = height + dimensions.borderSize * 2;
}


ObjectDimensions.prototype.refresh = function()
{
	this.refreshBorder();
	this.refreshPosition();
	this.refreshSize();
}


function CCTools()
{
	this.resize();
	
	this.monthNames = new Array( 12 );
	this.monthNames[0] = "January";
	this.monthNames[1] = "February";
	this.monthNames[2] = "March";
	this.monthNames[3] = "April";
	this.monthNames[4] = "May";
	this.monthNames[5] = "June";
	this.monthNames[6] = "July";
	this.monthNames[7] = "August";
	this.monthNames[8] = "September";
	this.monthNames[9] = "October";
	this.monthNames[10] = "November";
	this.monthNames[11] = "December";
									
	this.ie = gBrowserTypes['ie'];
}


CCTools.prototype.resize = function()
{
	var width = this.getScreenWidth();
    var height = this.getScreenHeight();	
	this.screenWidth = width;
	this.screenHeight = height;
	
	this.screenEnd = height + this.getScreenScroll();
}


CCTools.prototype.getScreenWidth = function()
{
    if( document.body && document.body.clientWidth )
    {
        return document.body.clientWidth;
    }
    if( document.width )
    {
        return parseInt( document.width, 10 );
    }
    if( window.innerWidth )
    {
        return window.innerWidth;
    }
    return 500;
}


CCTools.prototype.getScreenHeight = function()
{
    if( window && window.innerHeight )
    {        
        return window.innerHeight;
    }
    if( document.height )
    {
        return parseInt( document.height, 10 );
    }
    if( document.body && document.body.clientHeight )
    {
        return document.body.clientHeight;
    }
    return 500;
}


CCTools.prototype.getScreenScroll = function()
{
	return document.body.scrollTop;
}


CCTools.prototype.getX = function(object)
{
	var parentObject = object;
    var returnValue = object.offsetLeft;
    while( ( object = object.offsetParent ) != null )
    {
        if( object.tagName != 'HTML' )
            returnValue += object.offsetLeft;
    }
	parentObject.x = returnValue;
    return returnValue;
}


CCTools.prototype.getY = function(object)
{            
	var parentObject = object;
    var returnValue = object.offsetTop;
    while( ( object = object.offsetParent ) != null )
    {
        if( object.tagName != 'HTML' )
            returnValue += object.offsetTop;
    }
	parentObject.y = returnValue;
    return returnValue;
}

CCTools.prototype.zeroPosition = function(object)
{
    object.style.left = 0;
    object.style.top = 0;
    
	object.dimensions.x = 0;
	object.dimensions.y = 0;
}


CCTools.prototype.setPosition = function(object, x, y)
{
	var dimensions = object.dimensions;
	
	if( x != false && x != dimensions.x )
	{
		object.style.left = x + 'px';
		object.dimensions.x = x;
	}
	
	if( y != false && y != dimensions.y )
	{
		object.style.top = y + 'px';
		object.dimensions.y = y;
	}
}

CCTools.prototype.setOpacity = function(object, value)
{
	if( this.ie )
    {
        object.style.filter ='progid:DXImageTransform.Microsoft.Alpha(opacity=' +  value * 100 + ')';
    }
    else
    {
        object.style.opacity = value;
    }
}


CCTools.prototype.setGradient = function(style, from, to)
{
	var browserTypes = gBrowserTypes;
	if( from == to )
	{
		style.background = to;
	}
	else if( browserTypes['firefox'] )
	{
		style.background = "-moz-linear-gradient( top, " + from + ", " + to + " )";
	}
	else if( browserTypes['ie'] )
	{
		style.filter = "progid:DXImageTransform.Microsoft.Gradient( StartColorStr='" + from + "', EndColorStr='" + to + "', GradientType=0 )";
	}
	else
	{
		try
		{
			style.background = "-webkit-gradient( linear, left top, left bottom, from( " + from + " ), to( " + to + " ) )";
		}
		catch(e) 
		{
			style.background = to;
		}
		
		if( !style.background )
		{
			style.background = to;
		}
	}
}


CCTools.prototype.toTarget = function(current, target, speed)
{
    if( current < target )
    {
        current += speed;
        if( current > target )
        {
            current = target;
        }
    }
    else
    {
        current -= speed;
        if( current < target )
        {
            current = target;
        }
    }
    return current;
}
                                    
                                    
CCTools.prototype.stringReplaceChar = function(source, search, replace)
{
    var split = source.split( search );
    var result = "";
    for( var i=0; i<split.length; ++i )
    {
        if( i > 0 )
        {
            result += replace;
        }
        result += split[i];
    }
    return result;
}


CCTools.prototype.stringSplitBetween = function(source, from, to)
{
	var splitFrom = source.split( from );
	if( splitFrom.length > 1 )
	{
		var splitTo = splitFrom[1].split( to );
		return splitTo[0];
	}
	return false;
}
                                    
                                    
CCTools.prototype.stringSplitAfter = function(source, after)
{
    var split = source.split( after );
    if( split.length > 1 )
    {
        var result = split[1];
        for( var i=2; i<split.length; ++i )
        {
            result += after;
            result += split[i];
        }
        return result;
    }
    return false;
}


CCTools.prototype.stringLastWord = function(source)
{
	var split = source.split( " " );
	return split[split.length-1];
}
                                    

CCTools.prototype.stringRemoveBetween = function(source, before, after)
{
    var result = "";
    var splitBefore = source.split( before );
    for( var i=0; i<splitBefore.length-1; ++i )
    {
        result += before;
        result += splitBefore[i];
    }

    var splitAfter = source.split( after );
    for( var i=1; i<splitAfter.length; ++i )
    {            
        result += after;
        result += splitAfter[i];
    }

    return result;
}
                                    
CCTools.prototype.stringRemoveBetweenIncluding = function(source, before, after)
{
    var result = "";
    var splitBefore = source.split( before );
    for( var i=0; i<splitBefore.length-1; ++i )
    {
        if( i != 0 )
        {
            result += before;
        }
        result += splitBefore[i];
    }

    var splitAfter = source.split( after );
    for( var i=1; i<splitAfter.length; ++i )
    {
        if( i != 1 )
        {
            result += after;
        }
        result += splitAfter[i];
    }

    return result;
}


CCTools.prototype.equalDates = function(date1, date2)
{
	if( date1.getDate() == date2.getDate() && date1.getMonth() == date2.getMonth() && date1.getFullYear() == date2.getFullYear() )
	{
		return true;
	}
	return false;
}


CCTools.prototype.betweenDates = function(date, dateStart, dateEnd)
{
    if( this.equalDates( date, dateStart ) )
    {
        return true;
    }

    if( this.equalDates( date, dateEnd ) )
    {
        return true;
    }

    if( date > dateStart && date < dateEnd )
    {
        return true;
    }

    return false;
}


CCTools.prototype.daysBetween = function(date1, date2)
{
	// The number of milliseconds in one day
	var ONE_DAY = 1000 * 60 * 60 * 24;
		
	// Convert both dates to milliseconds
	var date1_ms = date1.getTime();
	var date2_ms = date2.getTime();
		
	// Calculate the difference in milliseconds
	var difference_ms = this.CCDistance( date1_ms, date2_ms );
		
	// Convert back to days and return
	return Math.round( difference_ms / ONE_DAY );
}


CCTools.prototype.dateToString = function(date)
{
	return date.getFullYear() + this.monthNames[date.getMonth()] + date.getDate();
}


CCTools.prototype.getLocationBarData = function(key, fallback)
{
	var stringSplit = window.location.href.split( key + '?' );
    if( stringSplit.length > 1 )
    {
        var operationIndicator = stringSplit[1].split( ';' );
        return operationIndicator[0];
    }
    return fallback;
}


CCTools.prototype.loadData = function(key)
{
    var value = false;
	if( window.localStorage )
	{
		if( window.localStorage.getItem( key ) )
		{
			value = window.localStorage.getItem( key );
		}
	}
	return value;
}


CCTools.prototype.saveData = function(key, value)
{
	if( window.localStorage )
	{
		window.localStorage[key] = value;
	}
	return value;
}


CCTools.prototype.isAlphabetic = function(string)
{
    return /^[a-zA-Z]+$/.test( string );
}


CCTools.prototype.isAlphaNumeric = function(string)
{
    return /^[a-zA-Z0-9]*$/.test( string );
}