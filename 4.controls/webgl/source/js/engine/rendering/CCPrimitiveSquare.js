/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2011 - 2011 France Telecom
 * This software is distributed under the Apache 2.0 license.
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *-----------------------------------------------------------
 * File Name   : CCPrimitiveSquare.js
 * Description : Square drawable component.
 *
 * Created     : 20/10/11
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

function CCPrimitiveSquare()
{
	this.construct();
    this.scale = vec3.create( [1,1,1] );
}
CopyPrototype( CCPrimitiveSquare, CCPrimitiveBase, "CCPrimitiveBase" );


CCPrimitiveSquare.prototype.setScale = function(width, height, depth)
{
	var scale = this.scale;
	scale[0] = width;
	scale[1] = height;
	scale[2] = depth;
    return this;
}


CCPrimitiveSquare.prototype.renderVertices = function(renderer)
{
    var gl = renderer.gl;
	
	GLPushMatrix();
	
    mat4.scale( mvMatrix, this.scale );
    
    //gl.bindBuffer( gl.ARRAY_BUFFER, renderer.squareVertexPositionBuffer );
    //gl.vertexAttribPointer( renderer.shaderProgram.vertexPositionAttribute, renderer.squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0 );
    
    renderer.setMatrixUniforms();
    gl.drawArrays( gl.TRIANGLE_STRIP, 0, renderer.squareVertexPositionBuffer.numItems );
	
    GLPopMatrix();
}


CCPrimitiveSquare.prototype.renderOutline = function(renderer)
{
	var gl = renderer.gl;
	GLPushMatrix();
    mat4.scale( mvMatrix, this.scale );
	renderer.setMatrixUniforms();
	
	gl.drawElements( gl.LINE_STRIP, renderer.squareVertexIndexBuffer.numItems, gl.UNSIGNED_SHORT, 0 );
	
	GLPopMatrix();
}