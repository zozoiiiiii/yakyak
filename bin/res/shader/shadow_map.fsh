
// Set default precision to medium
precision mediump int;
precision mediump float;
      
varying vec2 v_texcoord;
uniform sampler2D ShadowMap;  


// 第一次渲染:以光源为视口，生成深度图，使用了vs; 第二次渲染：以摄像机为视口，显示深度图,使用了vs, fs
void main()
{
	// 在quard中显示深度图。所以根据quard的纹理坐标来采样深度图. 最近为0，最远为1.即越近越黑
    float Depth = texture2D(ShadowMap, v_texcoord).z;                   
	
	// 渲染结果不太清楚，所以做以下操作
    //Depth = 1.0 - (1.0 - Depth) * 25.0;                       
	gl_FragColor = vec4(Depth);
	return;
}



