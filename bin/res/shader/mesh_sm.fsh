precision mediump float;
precision mediump int;



uniform sampler2D ShadowMap;

varying vec2 v_texcoord;

void main()
{                                               
    float Depth = texture(ShadowMap, v_texcoord).x;                               
    Depth = 1.0 - (1.0 - Depth) * 25.0;                                             
    gl_FragColor = vec4(Depth); 
}