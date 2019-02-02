
// Set default precision to medium
precision mediump int;
precision mediump float;

uniform mat4 view;
uniform mat4 projection;
attribute vec3 i_pos;
varying vec3 v_texcoord;

void main()
{
	vec4 pos  = projection * view * vec4(i_pos, 1.0);
	gl_Position = pos .xyww; // 此处让z=w 则对应的深度值变为depth = w / w = 1.0
	v_texcoord = i_pos; // 当立方体中央处于原点时 立方体上位置即等价于向量
}