precision mediump float;
precision mediump int;

attribute vec3 iPos;
attribute vec4 iDiffuse;

uniform mat4 c_topLeftMatrix;
varying vec4 oDiffuse;

void main(void)
{
	oDiffuse = (iDiffuse/255.0).zyxw;
	gl_Position = c_topLeftMatrix * vec4(iPos, 1.0);
	
}

