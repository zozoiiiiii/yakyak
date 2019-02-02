#include "Anim.h"
#include <assert.h>
#include <math.h>
#include "core/inc/yy_file.h"

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "model_res.h"

NS_YY_BEGIN


// 
// void invert_simpler(Mat4f* result, const Mat4f* a)
// {
//     memcpy(result, a, sizeof(Mat4f));
// 
//     Swap(result->m[0][1], result->m[1][0]);
//     Swap(result->m[0][2], result->m[2][0]);
//     Swap(result->m[1][2], result->m[2][1]);
//     float m30    = -(
//         result->m[0][0] * result->m[3][0] + 
//         result->m[1][0] * result->m[3][1] + 
//         result->m[2][0] * result->m[3][2]
//     );
// 
//     float m31    = -(
//         result->m[0][1] * result->m[3][0] + 
//         result->m[1][1] * result->m[3][1] + 
//         result->m[2][1] * result->m[3][2]
//     );
// 
//     result->m[3][2]    = -(
//         result->m[0][2] * result->m[3][0] + 
//         result->m[1][2] * result->m[3][1] + 
//         result->m[2][2] * result->m[3][2]
//     );
// 
//     result->m[3][1]    = m31;
//     result->m[3][0]    = m30;
// }

void Anim::Load(YY::Skeleton* skel, const std::string& file)
{
    m_skel = skel;
    load(file);

    ResampleAnimationTracks(20);// 20 KeyFrames per second
    SetBindPose();                // store bind pose
}

void Anim::load(const std::string& file)
{
    std::string res_path = ModelRes::Instance()->GetResPath();
    std::string path = res_path + file;
    FILE *infile = fopen(path.c_str(), "rt");
    throw_assert(infile, "open file faile:"<<path);

    fscanf(infile, "time len:%f\n", &timeLength);
    fscanf(infile, "frame num:%d\n", &frameCount);

    int bone_num;
    fscanf(infile, "bone num:%d\n", &bone_num);
    Tracks.resize(bone_num);
    for(int i=0; i<bone_num; i++)
    {
        Track& tk = Tracks[i];
        int kf_num;
        fscanf(infile, "keyframe num: %d\n", &kf_num);
        for(int j=0; j<kf_num; j++)
        {
            KeyFrame kf;
            fscanf(infile, "%f (%f %f %f) (%f %f %f %f) (%f %f %f)\n",
                &kf.time,
                &kf.pos.x,&kf.pos.y,&kf.pos.z,
                &kf.angle,&kf.ax,&kf.ay,&kf.az,
                &kf.scale.x,&kf.scale.y,&kf.scale.z);

            // 前面几帧包含tpose信息，会导致一个明显的卡顿
            if(j>=2)
                tk.keys.push_back(kf);
        }
    }

    fclose(infile);

}



// 取得小数部分
#define frac(a) (a-floor(a))

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define clamp(a_,b_,c_) min(max(a_,b_),c_)

KeyFrame& Anim::GetInterpolatedKey(Track &t,int frame,float weight,bool normalize)
{
    // 获得当前帧和下一帧
    KeyFrame &k0=t.keys[(frame  ) % t.keys.size() ];
    KeyFrame &k1=t.keys[(frame+1) % t.keys.size() ];

    // 根据当前帧和下一帧，以及他们的权值，计算出当前的位置和角度
    static KeyFrame k;
    float weight1=1.0-weight;

    k.pos = k0.pos*weight1+k1.pos*weight;


    k.angle=k0.angle*weight1+k1.angle*weight;
    k.ax=k0.ax*weight1+k1.ax*weight;
    k.ay=k0.ay*weight1+k1.ay*weight;
    k.az=k0.az*weight1+k1.az*weight;

    if(normalize)
    {
        YY::Vec3f axis(k.ax,k.ay,k.az);
        axis.norm();
        k.ax=axis.x;k.ay=axis.y;k.az=axis.z;
    }
    return k;
}

void Anim::EvalSubtree(int id,int frame, float weight)
{
    Bone &b = m_skel->bones[id];
    YY::Mat4f a,m;

    // bind pose : default
    YY::Vec3f pos = b.pos;
    m.Set(b.angle, b.ax, b.ay, b.az);
    //Quaternion tmp;
    //tmp.fromAxisAngle(b.angle, b.ax, b.ay, b.az);
    //m.Set(tmp);


    //set(&m, b.angle, b.ax, b.ay, b.az);

    if(Tracks[id].keys.size()>frame) // add Animated pose if Track available
        if(frame>=0)
        {
            // 获得当前帧的骨骼数据
            KeyFrame &k=GetInterpolatedKey(Tracks[id],frame,weight);
            a.Set(k.angle,k.ax, k.ay,k.az);

            //Quaternion tmp;
            //tmp.fromAxisAngle(k.angle, k.ax, k.ay, k.az);
            //a.Set(tmp);

            pos=pos+k.pos;
            m=m * a;
        }

        // 移动到指定位置
        m.SetTranslate(pos.x, pos.y, pos.z);

        // store bone matrix
        if(b.parent>=0)
        {
            b.matrix = m_skel->bones[b.parent].matrix * m;
        }
        else
        {
            b.matrix=m;
        }

        // progress through tree
        for(int i=0; i<b.children.size(); i++)
        {
            EvalSubtree(b.children[i],frame,weight);
        }
}

void Anim::SetPose(double time)
{
    // 获得一个时间
    double time01=time/double(timeLength);

    // 该时间肯定是正数，所以以下获得该实数的小数部分
    time01=time01-floor(time01);

    // 计算出属于哪一帧？
    float frame=(frameCount-2)*time01+1;

    for (int i = 0; i < m_skel->bones.size(); i++)
    {
        m_skel->bones[i].matrix.Ident();
    }

    for (int i = 0; i < m_skel->bones.size(); i++)
    {
        if (m_skel->bones[i].parent==-1)
        {
            EvalSubtree(i,int(frame),frac(frame));
        }
    }
}

void Anim::SetBindPose()
{
    // 初始化
    for(int i=0; i<m_skel->bones.size(); i++)
    {
        m_skel->bones[i].matrix.Ident();
    }

    // 从所有根骨头计算
    for(int i=0; i<m_skel->bones.size(); i++)
    {
        if (m_skel->bones[i].parent==-1)
        {
            EvalSubtree(i,-1,0);
        }
    }

    for(int i=0; i<m_skel->bones.size(); i++)
    {
        m_skel->bones[i].inverseBindPose = m_skel->bones[i].matrix;
    }

    for(int i=0; i<m_skel->bones.size(); i++)
    {
        m_skel->bones[i].inverseBindPose.Inverse();
        //invert_simpler(&m_skel->bones[i].inverseBindPose, &m_skel->bones[i].inverseBindPose);
    }
}

void Anim::ResampleAnimationTracks(double frames_per_second)
{
    for(int j=0; j<Tracks.size(); j++)
    {
        Track &src=Tracks[j];
        if(src.keys.size()<=0)
        {
            continue;
        }

        Track dst; 
        double length=timeLength;

        // 程序要求的帧数
        int newframecount=length*frames_per_second;
        int src_frame=0;
        //printf("src[%d]=%d frames\n",j,src.keys.size());

        // 遍历
        for(int k=0; k<newframecount; k++)
        {
            // 计算出具体帧所处的时间
            double time=k*length/double(newframecount-1);

            while(src_frame<src.keys.size() && time>src.keys[src_frame].time )
                src_frame++;

            int src_frame_1 = clamp ( src_frame-1 ,0,src.keys.size()-1);
            int src_frame_2 = clamp ( src_frame   ,0,src.keys.size()-1);

            float t1=src.keys[src_frame_1].time;
            float t2=src.keys[src_frame_2].time;
            float w=0.0f;
            if(t1!=t2)
            {
                w=(time-t1)/(t2-t1);
            }

            KeyFrame key=GetInterpolatedKey(src,src_frame_1,w,true);
            dst.keys.push_back(key);
        }
        Tracks[j]=dst;
        frameCount=newframecount;
        //printf("dst[%d]=%d frames\n\n",j,dst.keys.size());
    }
}

void Anim::render(IShader* sr)
{
    int ph = sr->GetProgramHandle();

    int u_hasAnimation=glGetUniformLocation(ph, "u_hasAnimation");
    throw_assert(u_hasAnimation >= 0, "no uniform called:u_hasAnimation in Shader:"<< ph);

    glUniform1i(u_hasAnimation, 1);

    // 获得程序启动之后的时间，单位是秒
    static sint64 startTime = YY::TimeStamp();
    sint64 timeInterval = YY::TimeStamp() - startTime;
    float fdetla = (float)timeInterval/1000.0f;
    fdetla /= 1000.0f;
    SetPose(fdetla);

    // skinning enabled
    YY::Mat4f bones[100];
    int bone_num = m_skel->bones.size();

	// android physical phone cannot support uniform 128
	//if (bone_num >= 62)
		//return;

    for(int i=0; i< bone_num; i++)
    {
        const Bone& jt = m_skel->bones[i];
        bones[i] = jt.matrix * jt.inverseBindPose;

        char str[100];
        sprintf(str,"u_bones[%d]",i);

        int g_bones=glGetUniformLocation(ph, str);
        throw_assert(g_bones >= 0, "no uniform called:u_hasAnimation in Shader:" << ph);
        glUniformMatrix4fv(g_bones, 1, GL_FALSE, (GLfloat*) &bones[i].m_data);
    }


}


NS_YY_END