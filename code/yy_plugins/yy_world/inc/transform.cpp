#include "transform.h"



void Transform::GetRotateMatrix(YY::Mat4f* rotateM)
{
    rotateM->InitRotateTransform(angle.x, angle.y, angle.z);
    //pMatrix->Ident();
    //pMatrix->Rotate(GetAngleY(), 0,1,0); // yaw
    //pMatrix->Rotate(GetAngleX(), 1,0,0); // pitch
    //pMatrix->Rotate(GetAngleZ(), 0,0,1); // roll
    //return;

//      YY::Mat4f rotX,rotY,rotZ;
//      rotX.RotateX(GetAngleX());
//      rotY.RotateY(GetAngleY());
//      rotZ.RotateZ(GetAngleZ());
//     *pMatrix = rotZ * rotY * rotX;
    
    //pMatrix->Rotate(GetAngleX(), GetAngleY(), GetAngleZ());
}

void Transform::GetTranslateMatrix(YY::Mat4f* pMatrix)
{
    pMatrix->Translate(pos.x, pos.y, pos.z);
}

void Transform::GetScaleMatrix(YY::Mat4f* pMatrix)
{
    pMatrix->Scale(scale.x, scale.y, scale.z);
}


void Transform::GetTMMatrix(YY::Mat4f* pMatrix)
{
    YY::Mat4f translate,scale,rotate;
    GetRotateMatrix(&rotate);
    GetScaleMatrix(&scale);
    GetTranslateMatrix(&translate);

    // 顺序：顶点先缩放，在旋转，再平移。
    // TransformedVector = TranslationMatrix * RotationMatrix * ScaleMatrix * OriginalVector; ```
    *pMatrix = translate*rotate*scale;
    //*pMatrix = scale * rotate * translate;
}