
// Set default precision to medium
precision mediump int;
precision mediump float;


varying vec3 v_texcoord;
uniform samplerCube tex_sky;       
void main()
{
	gl_FragColor = textureCube(tex_sky, v_texcoord);
}

