/*-----------------------------------------------------------
 * 2c - Cross Platform 3D Application Framework
 *-----------------------------------------------------------
 * Copyright © 2010 – 2011 France Telecom
 * This software is distributed under the Apache 2.0 license,
 * see the "license.txt" file for more details.
 *-----------------------------------------------------------
 * File Name   : basic.fx
 * Description : Basic unlit shader.
 *
 * Created     : 08/09/10
 * Author(s)   : Ashraf Samy Hegab
 *-----------------------------------------------------------
 */

#ifndef QT
precision mediump float;
#endif

// Globals
uniform mat4 u_modelViewProjectionMatrix;
uniform vec4 u_modelColour;

// PS Input
//varying vec4 ps_color;
varying vec2 ps_texCoord;


#ifdef VERTEX_SHADER

// VS Input
attribute vec4 vs_position;
//attribute vec4 vs_color;
attribute vec2 vs_texCoord;

void main()
{
    gl_Position = u_modelViewProjectionMatrix * vs_position;
//    ps_color = vs_color;
    ps_texCoord = vs_texCoord;
}

#endif


#ifdef PIXEL_SHADER

uniform sampler2D s_diffuseTexture;

void main()
{
    gl_FragColor = u_modelColour * texture2D( s_diffuseTexture, ps_texCoord ).rgba;
}

#endif
