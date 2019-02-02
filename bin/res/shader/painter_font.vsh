precision mediump float;
precision mediump int;

attribute vec3 iPos;
attribute vec2 i_texcoord;

uniform mat4 c_topLeftMatrix;
varying vec2 v_texcoord;

void main(void)
{
	gl_Position = c_topLeftMatrix * vec4(iPos, 1.0);

    v_texcoord =  vec2(i_texcoord.x, i_texcoord.y);
}

