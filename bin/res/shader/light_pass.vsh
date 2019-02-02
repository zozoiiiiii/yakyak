precision mediump float;
precision mediump int;


attribute vec3 i_pos;
attribute vec2 i_texcoord;
varying vec2 v_texcoord;

void main()
{
	v_texcoord = i_texcoord;
    gl_Position = vec4(i_pos,1.0);
}

