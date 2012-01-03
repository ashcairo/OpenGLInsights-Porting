/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCTile3DButton.js
 * Description : A dynamic button widget.
 *
 * Created     : 09/08/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCTile3DButton(scene)
{
    this.construct( scene );
}
CopyPrototype( CCTile3DButton, CCTile3D, "CCTile3D" );


CCTile3DButton.prototype.construct = function(scene)
{
	this.CCTile3D_construct();
	
	if( scene )
	{
		this.setScene( scene );
	}
	
	this.renderDepth = false;
	this.setTransparent( true );

    var model = this.model = new CCModelBase();
    
    var baseModel = this.baseModel = new CCModelBase();
    model.addModel( baseModel );
	this.colourInterpolator = new CCInterpolatorColour();
	this.colourInterpolator.setDuration( 0.5 );
	this.setBaseColour( gColour.white() );
	this.baseSquare = false;
	
	this.textModel = new CCModelText( this );
	
	this.allowTouchRotation( false );
	this.touchRotationMagnitude = 0.0;
	this.touchRotationSpeed = 1.0;
	
	//this.scaleInterpolator.setup( scale, 1.0 );
	
	this.allowTouchMovement( false );

    this.touchDepressPosition = vec3.create();
    this.touchDepressInterpolator = new CCInterpolatorsV3();
    this.touchDepressInterpolator.speed = 3.0;
	this.setTouchDepressRange( 1.0 );
    this.touchDepressDepth = 2.0;
}


CCTile3DButton.prototype.setup = function(width, height, text)
{
	this.setupBase( width, height );
    
    if( text )
    {
        this.textModel.setText( text, height );
    }
	
	return this;
}

// Create tile with width and textures aspect ratio
CCTile3DButton.prototype.setupTexture = function(width, textureSrc)
{
	var TextureLoadCallback = function(tile, src) 
	{
		this.tile = tile;
		this.src = src;
	}
    TextureLoadCallback.prototype.callback = function(texture)
    {
        var aspectRatio = texture.width / texture.height;
		var height = width / aspectRatio;
		this.tile.setupBase( width, height );
		
		this.tile.setBaseTexture( this.src );
    }
	
	gEngine.textureManager.getTexture( textureSrc, new TextureLoadCallback( this, textureSrc ) );
	return this;
}

// Create tile with the text height
CCTile3DButton.prototype.setupText = function(text, height, centered)
{
	var textModel = this.textModel;
	textModel.setText( text, height );
    
    var width = textModel.getWidth();
    this.setHCollisionBounds( width * 0.5, height * 0.5, CC_SMALLFLOAT );
    
    textModel.setCentered( centered );
    
    if( centered == false )
    {
        this.translate( collisionBounds.x, 0.0, 0.0 );
    }
	
	return this;
}


CCTile3DButton.prototype.destruct = function(width, height)
{
}


// CCRenderable
CCTile3DButton.prototype.refreshModelMatrix = function()
{
	if( this.updateModelMatrix )
    {
        this.updateModelMatrix = false;
        
        var modelMatrix = this.modelMatrix;
        mat4.identity( modelMatrix );
		
        mat4.translate( modelMatrix, this.position );
        mat4.translate( modelMatrix, this.touchDepressPosition );

        mat4.scale( modelMatrix, this.scale );

        var rotation = this.rotation;
        mat4.rotate( modelMatrix, rotation[0], [1.0, 0.0, 0.0] );
        //mat4.rotate( modelMatrix, rotation[1], [0.0, 1.0, 0.0] );
        gTools.CCMatrixRotate( modelMatrix, rotation[1], 0.0, 1.0, 0.0 );
    }
}


CCTile3DButton.prototype.update = function(delta)
{
    if( this.touchDepressRange > 0.0 )
    {
        if( this.touchDepressInterpolator.update( delta ) )
        {
            this.updateModelMatrix = true;
        }
    }

    if( this.touching )
    {
        this.touchingTime += delta;
    }
    else if( this.touchReleased )
    {
        if( this.touchDepressInterpolator.finished() )
        {
            this.handleTouchRelease();
        }
    }
	
	this.colourInterpolator.update( delta );
	
	if( this.textModel != false )
    {
        this.textModel.colourInterpolator.update( delta );
    }
}


CCTile3DButton.prototype.setupBase = function(width, height)
{
    if( this.baseSquare == false )
    {
        var baseSquare = this.baseSquare = new CCPrimitiveSquare();
        this.baseModel.addPrimitive( baseSquare );
    }

    var hWidth = width * 0.5;
    var hHeight = height * 0.5;
    this.setHCollisionBounds( hWidth, hHeight, CC_SMALLFLOAT );
	this.baseSquare.setScale( width, height, 1.0 );
    
    this.refreshCollisions();
}


CCTile3DButton.prototype.setBaseTexture = function(src)
{
	this.baseSquare.setTexture( src );
}


CCTile3DButton.prototype.setBaseColour = function(inColour)
{
	this.baseModel.setColour( inColour );
	this.colourInterpolator.setup( this.baseModel.colour );
}


CCTile3DButton.prototype.handleProjectedTouch = function(hitObject, hitPosition, 
                                                 touch, touchAction)
{
    if( this.enabled == false )
    {
        return false;
    }

    if( hitObject == this &&
        ( touchAction == touch_pressed || ( this.touchMovementAllowed && CCControls.touchActionMoving( touchAction ) ) ) )
    {
        if( this.touching == false )
        {
            this.touching = true;
            this.touchingTime = 0.0;
            this.onTouchPress();
        }
    }
    
    if( this.touching )
    {
        var position = this.position;
        var collisionBounds = this.collisionBounds;
        var maxTimeHeld = 0.0;
        
        if( touchAction == touch_pressed )
        {
            if( touch.timeHeld >= maxTimeHeld )
            {
                var relativeHitPosition = vec3.create();
                relativeHitPosition[0] = hitPosition[0] - position[0];
                relativeHitPosition[1] = hitPosition[1] - position[1];
                var x = relativeHitPosition[0] / collisionBounds[0];
                var y = relativeHitPosition[1] / collisionBounds[1];
                this.touchActionPressed( x, y, touchAction );
            }
        }
        else if( this.touchMovementAllowed &&  CCControls.touchActionMoving( touchAction ) )
        {
            if( hitObject != this )
            {
                return this.handleProjectedTouch( hitObject, hitPosition, touch, touch_lost );
            }
            else
            {
                var relativeHitPosition = vec3.create();
                relativeHitPosition[0] = hitPosition[0] - position[0];
                relativeHitPosition[1] = hitPosition[1] - position[1];
                var x = relativeHitPosition[0] / collisionBounds[0];
                var y = relativeHitPosition[1] / collisionBounds[1];
                this.touchActionPressed( x, y, touchAction );
            }
        }
        else
        {   
            // Ensure we have a good touch release
            if( touchAction == touch_released )
            {
				if( this.touchMovementAllowed == false )
                {
					var absDeltaX = Math.abs( touch.totalDeltaX );
					var absDeltaY = Math.abs( touch.totalDeltaY );
					if( absDeltaX > CC_TOUCH_TO_MOVEMENT_THRESHOLD || absDeltaY > CC_TOUCH_TO_MOVEMENT_THRESHOLD )
					{
						return this.handleProjectedTouch( hitObject, hitPosition, touch, touch_lost );
					}
				}
                
                // If we have a good first touch, register it.
                var relativeHitPosition = vec3.create();
                relativeHitPosition[0] = hitPosition[0] - position[0];
                relativeHitPosition[1] = hitPosition[1] - position[1];
                var x = relativeHitPosition[0] / collisionBounds[0];
                var y = relativeHitPosition[1] / collisionBounds[1];
                this.touchActionPressed( x, y, touch_released );
            }
            
            this.touchActionRelease( touchAction );
            this.touching = false;
            
            if( touchAction == touch_released )
            {
                return 2;
            }
        }
    }
    
    return 0;
}

CCTile3DButton.prototype.touchActionPressed = function(x, y, touchAction)
{   
    // If we can press down on the tile
    if( this.touchDepressRange > 0.0 )
    {
        this.touchDepressInterpolator.pushV3( this.touchDepressPosition, vec3.create( [0.0, 0.0, -this.touchDepressDepth] ), true );
    }

    if( touchAction > touch_pressed && touchAction < touch_released )
    {
        this.onTouchMove();
    }
}

CCTile3DButton.prototype.touchActionRelease = function(touchAction)
{
    if( touchAction == touch_released )
    {
        this.touchReleased = true;
    }
    else 
    {
        this.handleTouchRelease();
		if( touchAction == touch_lost )
        {
            this.onTouchLoss();
        }
    }
}

CCTile3DButton.prototype.handleTouchRelease = function()
{
    // If the touch has been released successfully we fire our callback
    if( this.touchReleased )
    {
        this.touchReleased = false;
        this.onTouchRelease();
    }

	// Touch depress: On releasse
    if( this.touchDepressRange > 0.0 )
    {
        this.touchDepressInterpolator.pushV3( this.touchDepressPosition, vec3.create(), true );
    }
}


CCTile3DButton.prototype.allowTouchRotation = function(allow) 
{ 
	this.touchRotationAllowed = allow;
}


CCTile3DButton.prototype.allowTouchMovement = function(allow)
{
	this.touchMovementAllowed = allow;
}
    
	
CCTile3DButton.prototype.setTouchRotationSpeed = function(speed)
{
	this.touchRotationSpeed = speed;
}
    
	
CCTile3DButton.prototype.setTouchDepressRange = function(range)
{
	this.touchDepressRange = range;
}