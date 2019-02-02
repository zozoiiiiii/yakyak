precision mediump float;
precision mediump int;

//matrix
uniform mat4 u_mvp;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;

const int MAX_bones = 100;
uniform mat4 u_bones[MAX_bones];
uniform int  u_hasAnimation;


//input
attribute vec3 i_pos;
attribute vec2 i_texcoord;

//bone
attribute vec4 BONE_ID;
attribute vec4 BONE_WEIGHT;


//output
varying vec2 v_texcoord;

void main() {	
 
	vec4 actual_pos;
	
	// skinning
	if(u_hasAnimation==1)
	{
		int boneID0=int(BONE_ID[0]);
		int boneID1=int(BONE_ID[1]);
		int boneID2=int(BONE_ID[2]);
		int boneID3=int(BONE_ID[3]);
		
		mat4 BoneTransform;
		BoneTransform      = BONE_WEIGHT[0] * u_bones[boneID0];
		BoneTransform     += BONE_WEIGHT[1] * u_bones[boneID1];
		BoneTransform     += BONE_WEIGHT[2] * u_bones[boneID2];
		BoneTransform     += BONE_WEIGHT[3] * u_bones[boneID3];
		actual_pos = BoneTransform * vec4(i_pos,1.0);
	}
	else
	{
		actual_pos = vec4(i_pos,1.0);
	}

	//texture
    v_texcoord = i_texcoord;
	gl_Position = u_proj*u_view*u_model * actual_pos;
	//gl_Position = u_mvp*actual_pos;
}