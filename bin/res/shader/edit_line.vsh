precision mediump float;
precision mediump int;

attribute vec3 iPos;
attribute vec4 iDiffuse;

uniform mat4 c_mtxWVP;
varying vec4 oDiffuse;

void main(void)
{
	oDiffuse = iDiffuse.zyxw;
	gl_Position = c_mtxWVP * vec4(iPos, 1.0);
}

