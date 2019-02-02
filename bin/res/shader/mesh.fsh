precision mediump float;
precision mediump int;

      
// 只有直射光的话，会导致阴影黑色。加个环境光，使黑色阴影的模型也能显示
struct Ambient
{
	vec3 color;
	float intensity;
};

struct  DirectionalLight
{
	vec3 direction;
	vec3 color;
	float intensity;
};

uniform Ambient ambient;
uniform DirectionalLight directionLight;
uniform sampler2D texture;
uniform sampler2D ShadowMap;


varying vec4 LightSpace_postion;
varying vec2 v_texcoord;
varying vec3 v_normal_line;


vec4 calculateAmbient(Ambient light , vec3 normal)
{
    if(length(normal) <= 0.5)
    {   
        return vec4(1.0,1.0,1.0,1.0);
    }else
    {
        return vec4(light.color,1.0) * light.intensity;
    }
}


// 计算在光源空间下的像素，在深度图中是否属于阴影
float CalcShadowFactor(sampler2D the_shadow_map, vec4 LightSpacePos)                                                  
{            
	float bias = 0.005;
    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;  

	// 变换到0,1的范围
    vec2 UVCoords;                                                                          
    UVCoords.x = 0.5*ProjCoords.x +0.5;                                                  
    UVCoords.y = 0.5*ProjCoords.y+0.5;                                                  
	
	// simple
	// 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    //float closestDepth = texture2D(the_shadow_map, UVCoords).x; 
	
    // 取得当前片元在光源视角下的深度
    //float currentDepth = 0.5*ProjCoords.z+0.5;
	
    // 检查当前片元是否在阴影中
    //float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

	// 该片元有多大成分不在阴影中
    //return 1.0-shadow;
	
	
	
	
    float z =  0.5*ProjCoords.z+0.5;

	float xOffset = 1.0/2000.0;
    float yOffset = 1.0/2000.0;

    float Factor = 0.0;

    for (int y = -1 ; y <= 1 ; y++) {
        for (int x = -1 ; x <= 1 ; x++) {
			float x1 = float(x);
			float y1 = float(y);
            vec2 Offsets = vec2(x1 * xOffset, y1 * yOffset);
            vec2 UVC = vec2(UVCoords + Offsets);
            float  result = texture2D(the_shadow_map, UVC).x;
			if(result < z -0.00001)
			{
			Factor +=0.0;
			}
			else{
			Factor +=1.0;	
			}
        }
    }                                                                    
	return Factor/9.0;
}


//计算漫反射光
vec4 calculateDiffuse(vec3 normal_line , vec3 light_direction , vec3 color , float intensity)
{
	// 计算光和法线夹角
	float diffuse_factor = dot(normalize(normal_line), normalize(-light_direction));
	
	vec4 diffuse_color;
    if (diffuse_factor > 0.0) {
        diffuse_color = vec4(color,1.0) * diffuse_factor * intensity;
    }
	else
	{
		diffuse_color = vec4(0,0,0,1.0);
	}
	return diffuse_color;
}

vec4 caclculateDirectionLight(vec3 normal_line,DirectionalLight light)
{
	float shadow_factor =CalcShadowFactor(ShadowMap,LightSpace_postion);
	vec4 pointColor = calculateDiffuse(normal_line , light.direction , light.color , light.intensity);
	return shadow_factor*pointColor;
}


void main()
{
	vec3 normal;
		normal = v_normal_line;

	vec4 totalLight = calculateAmbient(ambient,normal);
	totalLight += caclculateDirectionLight(normal,directionLight);
	gl_FragColor = texture2D(texture,v_texcoord)*totalLight;
}