/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCRenderer.js
 * Description : WebGL renderer.
 *
 * Created     : 24/07/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

var shaderVS = "                                                        \
attribute vec3 aVertexPosition;                                         \
attribute vec4 aVertexColor;                                            \
attribute vec2 aTextureCoord;                                           \
uniform mat4 uMVMatrix;                                                 \
uniform mat4 uPMatrix;                                                  \
varying vec4 vColor;                                                    \
varying vec2 vTextureCoord;                                             \
void main(void)                                                         \
{                                                                       \
    gl_Position = uPMatrix * uMVMatrix * vec4( aVertexPosition, 1.0 );  \
    vColor = aVertexColor;                                              \
    vTextureCoord = aTextureCoord;                                      \
}";

var shaderFS = "            \
#ifdef GL_ES                \n\
    precision highp float;  \n\
#endif                      \n\
varying vec4 vColor;        \
varying vec2 vTextureCoord; \
uniform sampler2D uSampler; \
void main(void)             \
{                           \
    vec4 texColor = texture2D( uSampler, vec2( vTextureCoord.s, vTextureCoord.t ) ); \
    gl_FragColor = texColor * vColor;  \
}                           \
";

function initGL(canvas) 
{
    var names = ["webgl", "experimental-webgl", "webkit-3d", "moz-webgl"];
    var context = null;
    for( var i=0; i<names.length; ++i)
    {
        try
        {
            context = canvas.getContext( names[i] );
        }
        catch( e ) {}

        if( context )
        {
            break;
        }
    }

    if( !context )
    {
        alert( "Could not initialise WebGL, sorry :-(" );
    }
    return context;
}


function getShader(gl, source, shaderType) 
{
    var shader = gl.createShader( shaderType );
    gl.shaderSource( shader, source );
    gl.compileShader( shader );

    if( !gl.getShaderParameter( shader, gl.COMPILE_STATUS ) ) 
    {
        alert( gl.getShaderInfoLog( shader ) );
        return null;
    }

    return shader;
}


var mvMatrix = mat4.create();
var mvMatrixStack = [];

function GLPushMatrix() 
{
    var copy = mat4.create();
    mat4.set( mvMatrix, copy );
    mvMatrixStack.push( copy );
}

function GLPopMatrix() 
{
    if( mvMatrixStack.length == 0 ) 
    {
        throw "Invalid popMatrix!";
    }
    mvMatrix = mvMatrixStack.pop();
}

function GLMultMatrix(inMatrix)
{
    //mat4.multiply( inMatrix, mvMatrix, mvMatrix );
	gTools.CCMatrixMultiply( mvMatrix, inMatrix, mvMatrix );
}



function CCRenderer(parent)
{	
    this.parent = parent;
    this.canvas = document.createElement( 'canvas' );	
	if( gBrowserTypes['mobile'] )
	{
		this.canvas.width = 320 * 0.5;
		this.canvas.height = 480 * 0.5;
	}
	else
	{
		this.canvas.width = 480;
		this.canvas.height = 720;
	}
    parent.appendChild( this.canvas );

    this.webGLStart();
    
    this.pendingRender = true;
    
    // Enums
    this.render_background = 0;
    this.render_main = 1;
    this.render_finished = 2;
}


CCRenderer.prototype.zero = function()
{
    this.canvas.style.width = 1;
    this.canvas.style.height = 1;
}


CCRenderer.prototype.resize = function()
{	
	var width = this.parent.clientWidth;
	var height = this.parent.clientHeight;
	
    //this.canvas.width = this.parent.clientWidth;
    //this.canvas.height = this.parent.clientHeight;
	
	var aspect = this.canvas.width / this.canvas.height;
	
    this.canvas.style.width = height * aspect;
    this.canvas.style.height = height;

    this.pendingRender = true;
}


CCRenderer.prototype.webGLStart = function() 
{
    var gl = this.gl = initGL( this.canvas );
    this.initShaders()
    this.initBuffers();
        
    gl.clearColor( 0.0, 0.0, 0.0, 0.0 );
	this.CCSetColour( gColour.white() );
    
	//gl.enable( gl.DEPTH_TEST );
	
    gl.blendFunc( gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA );
	gl.enable( gl.BLEND );
	
	// As of 29/10/11 Windows XP Firefox/Chrome doesn't support lineWidth
    gl.lineWidth( 2.0 );
    
    return gl;
}


CCRenderer.prototype.initShaders = function()
{
    var gl = this.gl;
    var fragmentShader = getShader( gl, shaderFS, gl.FRAGMENT_SHADER );
    var vertexShader = getShader( gl, shaderVS, gl.VERTEX_SHADER );
    
    var shaderProgram = this.shaderProgram = gl.createProgram();
    gl.attachShader( shaderProgram, vertexShader );
    gl.attachShader( shaderProgram, fragmentShader );
    gl.linkProgram( shaderProgram );
    
    if( !gl.getProgramParameter( shaderProgram, gl.LINK_STATUS ) ) 
    {
        alert( "Could not initialise shaders" );
    }
    
    gl.useProgram( shaderProgram );
    
    shaderProgram.vertexPositionAttribute = gl.getAttribLocation( shaderProgram, "aVertexPosition" );
    gl.enableVertexAttribArray( shaderProgram.vertexPositionAttribute );
    
    shaderProgram.vertexColorAttribute = gl.getAttribLocation( shaderProgram, "aVertexColor" );
    gl.enableVertexAttribArray( shaderProgram.vertexColorAttribute );

    shaderProgram.textureCoordAttribute = gl.getAttribLocation( shaderProgram, "aTextureCoord" );
    gl.enableVertexAttribArray( shaderProgram.textureCoordAttribute );
    
    shaderProgram.pMatrixUniform = gl.getUniformLocation( shaderProgram, "uPMatrix" );
    shaderProgram.mvMatrixUniform = gl.getUniformLocation( shaderProgram, "uMVMatrix" );
    shaderProgram.samplerUniform = gl.getUniformLocation( shaderProgram, "uSampler" );
}


CCRenderer.prototype.initBuffers = function()
{
    var gl = this.gl;
    
    // Square buffer
    var numberOfVerts = 4;
    {
        {
            var squareVertexPositionBuffer = this.squareVertexPositionBuffer = gl.createBuffer();
            squareVertexPositionBuffer.itemSize = 3;
            squareVertexPositionBuffer.numItems = numberOfVerts;
            
            this.squareVertexPositionData = new Float32Array( 3 * 4 );
            
            this.CCDefaultSquareVertexPointer();
        }
        
        {
            var squareVertexColorBuffer = this.squareVertexColorBuffer = gl.createBuffer();
            squareVertexColorBuffer.itemSize = 4;
            squareVertexColorBuffer.numItems = numberOfVerts;
            
            this.squareVertexColorData = new Float32Array( 4 * numberOfVerts );
        }

        {
            var squareVertexTextureCoordBuffer = this.squareVertexTextureCoordBuffer = gl.createBuffer();
            squareVertexTextureCoordBuffer.itemSize = 2;
            squareVertexTextureCoordBuffer.numItems = numberOfVerts;
            
            this.squareVertexTextureCoordData = new Float32Array( 2 * numberOfVerts );
            
            this.CCDefaultTexCoords();
        }
		
		// Indices
		{
			var squareVertexIndexBuffer = this.squareVertexIndexBuffer = gl.createBuffer();
			squareVertexIndexBuffer.itemSize = 1;
			
			var faces = [0, 1, 3, 2, 0,];
			squareVertexIndexBuffer.numItems = faces.length;
			var squareVertexIndexData = this.squareVertexIndexData = new Uint16Array( faces );
			
			gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, squareVertexIndexBuffer );
			gl.bufferData( gl.ELEMENT_ARRAY_BUFFER, squareVertexIndexData, gl.STATIC_DRAW );
		}
    }
}


CCRenderer.prototype.beginRender = function()
{
    //if( this.pendingRender )
    {
        this.pendingRender = false;

        var gl = this.gl;
        gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );		
        return true;
    }
    return false;
}


CCRenderer.prototype.setMatrixUniforms = function()
{
    var gl = this.gl;
    var shaderProgram = this.shaderProgram;
    
    gl.uniformMatrix4fv( shaderProgram.mvMatrixUniform, false, mvMatrix );
}


CCRenderer.prototype.CCSetColour = function(colour)
{
    var r = colour.r;
    var g = colour.g;
    var b = colour.b;
    var a = colour.a;
    
    var gl = this.gl;
    var data = this.squareVertexColorData;
    
    if( data[0] != r || 
        data[1] != g || 
        data[2] != b ||
        data[3] != a )
    {
        gl.bindBuffer( gl.ARRAY_BUFFER, this.squareVertexColorBuffer );
        var length = data.length;
        for( var i=0; i<length; i+=4 )
        {
            data[i+0] = r;
            data[i+1] = g;
            data[i+2] = b;
            data[i+3] = a;
        }
        
        gl.bufferData( gl.ARRAY_BUFFER, data, gl.STATIC_DRAW );
        gl.vertexAttribPointer( this.shaderProgram.vertexColorAttribute, this.squareVertexColorBuffer.itemSize, gl.FLOAT, false, 0, 0 );
    }
	
	this.colour = colour;
}

                                            
CCRenderer.prototype.CCSetTexCoords = function(textureCoords)
{
    var gl = this.gl;
    var data = this.squareVertexTextureCoordData;
    {
        gl.bindBuffer( gl.ARRAY_BUFFER, this.squareVertexTextureCoordBuffer );
        var length = data.length;
        for( var i=0; i<length; ++i )
        {
            data[i] = textureCoords[i];
        }
        
        gl.bufferData( gl.ARRAY_BUFFER, data, gl.STATIC_DRAW );
        gl.vertexAttribPointer( this.shaderProgram.textureCoordAttribute, this.squareVertexTextureCoordBuffer.itemSize, gl.FLOAT, false, 0, 0 );
    }
}


CCRenderer.prototype.CCDefaultTexCoords = function()
{
    var textureCoords = [1.0, 1.0,
                         0.0, 1.0,
                         1.0, 0.0,
                         0.0, 0.0,];
    this.CCSetTexCoords( textureCoords );
}


CCRenderer.prototype.CCSetSquareVertexPointer = function(vertices)
{
    var gl = this.gl;
    var data = this.squareVertexPositionData;
    {
        gl.bindBuffer( gl.ARRAY_BUFFER, this.squareVertexPositionBuffer );
        var length = data.length;
        for( var i=0; i<length; ++i )
        {
            data[i] = vertices[i];
        }
        
        gl.bufferData( gl.ARRAY_BUFFER, data, gl.STATIC_DRAW );
        gl.vertexAttribPointer( this.shaderProgram.vertexPositionAttribute, this.squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0 );
    }
}


CCRenderer.prototype.CCDefaultSquareVertexPointer = function()
{
    var vertices = [0.5,  0.5,  0.0,        // Top right
                    -0.5,  0.5,  0.0,       // Top left
                    0.5, -0.5,  0.0,        // Bottom left
                    -0.5, -0.5,  0.0,];     // Bottom right
    this.CCSetSquareVertexPointer( vertices );
}



CCRenderer.prototype.CCRenderHorizontalSquare = function(start, end)
{
    var vertices = 
	[
		start[0],   start[1],    start[2],      // Top left
		end[0],     start[1],    start[2],      // Top right
		start[0],   end[1],      end[2],		// Bottom left
		end[0],     end[1],      end[2],		// Bottom right
    ];
    this.CCSetSquareVertexPointer( vertices );
	
    var gl = this.gl;
    gl.drawArrays( gl.TRIANGLE_STRIP, 0, this.squareVertexPositionBuffer.numItems );
}

