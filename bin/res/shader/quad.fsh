// Set default precision to medium
precision mediump int;
precision mediump float;
      
varying vec2 v_texcoord;
uniform sampler2D ColorTexture;  


void main()
{
	gl_FragColor =  texture2D(ColorTexture, v_texcoord);
}
