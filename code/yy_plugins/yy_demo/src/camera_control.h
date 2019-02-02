/************************************************************************/
/* 
@author:    junliang
@brief:     camera control
@time:      2017-7-3
*/
/************************************************************************/
#pragma once

#include "yy_core.h"
#include "yy_world/inc/i_game_obj.h"

class CameraControl: public YY::BaseObject
{
    YY_BEGIN(CameraControl, "BaseObject");
    YY_END
public:
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void OnExcute(float sec);
    virtual bool OnMsg(uint32 msg_id, uint32 p1, uint32 p2);
    void SetCamera(YY_OBJECTID camera_id);

private:
	bool OnKeyDown(size_t vk);
	bool OnKeyUp(size_t vk);
	bool OnMouseMove(size_t param1, size_t param2);
	bool OnMouseWheel(size_t param1, size_t param2);
	bool OnLButtonDown(size_t param1, size_t param2);
	bool OnLButtonUp(size_t param1, size_t param2);
	bool OnMButtonDown(size_t param1, size_t param2);
	bool OnMButtonUp(size_t param1, size_t param2);

public:
	bool MoveFront(float delta);
	
	// �ƶ�һ�ξ���
	bool MoveLength(float len);
	// ����ƽ��
	bool MoveShift(float delta);
	// �������½�
	bool MoveUp(float delta);

	// ��ת�Ƕ�
	bool YawAngle(float angle);
	// ǰ����
	bool PitchAngle(float angle);
	// ����ƫת
	bool RollAngle(float angle);

    void CameraMoveBy(float x, float y, float z);
private:
	int m_nClickX;
	int m_nClickY;

	float m_fDirection;				// ǰ���ƶ�����
	float m_fUpDirection;			// �����ƶ�����
	float m_fShiftDirection;		// �����ƶ�����

    IGameObj* m_pCamera;

    float m_move_speed;

};


