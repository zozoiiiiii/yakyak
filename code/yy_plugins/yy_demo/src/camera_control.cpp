#include "camera_control.h"
#include "game.h"
#include <math.h>
//#include <windows.h>
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL   (WM_MOUSELAST+1)  // message that will be supported
#endif


void CameraControl::OnCreate()
{
//     GetMgr()->AddMsgProc(this, WM_KEYDOWN);
//     GetMgr()->AddMsgProc(this, WM_KEYUP);
//     GetMgr()->AddMsgProc(this, WM_MOUSEMOVE);
//     GetMgr()->AddMsgProc(this, WM_MOUSEWHEEL);
//     GetMgr()->AddMsgProc(this, WM_LBUTTONDOWN);
//     GetMgr()->AddMsgProc(this, WM_LBUTTONUP);
//     GetMgr()->AddMsgProc(this, WM_MBUTTONDOWN);
//     GetMgr()->AddMsgProc(this, WM_MBUTTONUP);
//     GetMgr()->AddMsgProc(this, WM_RBUTTONDOWN);
//     GetMgr()->AddMsgProc(this, WM_RBUTTONUP);
    GetMgr()->AddExcute(this);


    m_move_speed = 15.0f;

    m_fDirection = 0.0f;
    m_fUpDirection = 0.0f;
    m_fShiftDirection = 0.0f;
}

void CameraControl::SetCamera(YY_OBJECTID camera_id)
{
    m_pCamera = (IGameObj*)GetMgr()->Get(camera_id);
}

void CameraControl::OnDestroy()
{
    GetMgr()->RemoveExcute(this);
//     GetMgr()->RemoveMsgProc(this, WM_KEYDOWN);
//     GetMgr()->RemoveMsgProc(this, WM_KEYUP);
//     GetMgr()->RemoveMsgProc(this, WM_CHAR);
//     GetMgr()->RemoveMsgProc(this, WM_MOUSEMOVE);
//     GetMgr()->RemoveMsgProc(this, WM_MOUSEWHEEL);
//     GetMgr()->RemoveMsgProc(this, WM_LBUTTONDOWN);
//     GetMgr()->RemoveMsgProc(this, WM_LBUTTONUP);
//     GetMgr()->RemoveMsgProc(this, WM_MBUTTONDOWN);
//     GetMgr()->RemoveMsgProc(this, WM_MBUTTONUP);
//     GetMgr()->RemoveMsgProc(this, WM_RBUTTONDOWN);
//     GetMgr()->RemoveMsgProc(this, WM_RBUTTONUP);
}

void CameraControl::OnExcute(float sec)
{
    float move_speed = m_move_speed;

    // 前后移动 
    float len = sec * move_speed * m_fDirection;

    if (len != 0)
    {
        // 前后平移
        MoveFront(-len);
    }

    // 上下移动
    float len1 = sec * move_speed * m_fUpDirection;

    if (len1 != 0)
    {
        MoveUp(len1);
    }

    // 左右平移 
    float delta = sec * move_speed * m_fShiftDirection;

    if (delta != 0)
    {
        MoveShift(delta);
    }

}

bool CameraControl::OnMsg(uint32 msg_id, uint32 param1, uint32 param2)
{
	switch(msg_id)
	{
	/*case WM_KEYDOWN:
		{
			OnKeyDown(param1);
		}break;
	case WM_KEYUP:
		{
			OnKeyUp(param1);
		}break;
	case WM_MOUSEMOVE:
		{
			OnMouseMove(param1,param2);
		}break;
	case WM_MOUSEWHEEL:
		{
			OnMouseWheel(param1,param2);
		}break;
	case WM_MBUTTONDOWN:
		{
		}break;
		*/
	default:
		break;
	}

    return true;
}


static inline int get_x_lparam(size_t lparam)
{
	//return (short)(LOWORD(lparam));
	return 0;
}

static inline int get_y_lparam(size_t lparam)
{
	//return (short)(HIWORD(lparam));
	return 0;
}

bool CameraControl::MoveLength(float len)
{
    ITransform* pTransfrm = m_pCamera->GetTransform();

    // 垂直方向运动
    float v_delta = len * sinf(pTransfrm->GetAngle().x);

//     if (m_bToChangeHeight)
//     {
//         m_vPosition.y -= v_delta;
//     }
    // 水平方向运动
    float h_delta = len * cosf(pTransfrm->GetAngle().x);

    pTransfrm->AddPosX(h_delta * sinf(pTransfrm->GetAngle().y) );
    pTransfrm->AddPosZ( h_delta * cosf(pTransfrm->GetAngle().y) );

    return true;
}

bool CameraControl::MoveShift(float delta)
{
    ITransform* pTransfrm = m_pCamera->GetTransform();
    float angle = pTransfrm->GetAngle().y + YY_PI * 0.5F;

    pTransfrm->AddPosX( delta * sinf(angle) );
    pTransfrm->AddPosZ( delta * cosf(angle) );
    return true;
}

bool CameraControl::MoveUp(float delta)
{
    m_pCamera->GetTransform()->AddPosY( delta );
    return true;
}

bool CameraControl::YawAngle(float angle)
{
    m_pCamera->GetTransform()->AddAngleY(angle);
    return true;
}

bool CameraControl::PitchAngle(float angle)
{
    m_pCamera->GetTransform()->AddAngleX(angle);
    return true;
}

bool CameraControl::RollAngle(float angle)
{
    m_pCamera->GetTransform()->AddAngleZ(angle);
    return true;
}

bool CameraControl::MoveFront(float delta)
{
    YY::Vec3f angle = m_pCamera->GetTransform()->GetAngle();
    YY::Vec3f pos = m_pCamera->GetTransform()->GetPos();
	pos.x += delta * sin(angle.y);
	pos.z += delta * cos(angle.y);

	m_pCamera->GetTransform()->SetPos(pos.x, pos.y, pos.z);
	return true;
}

bool CameraControl::OnKeyDown(size_t vk)
{
	switch (vk)
	{
	case 'A':
		// 左转
		m_fShiftDirection = -1.0f;
		break;
	case 'D':
		// 右转
		m_fShiftDirection = 1.0f;
		break;
	case 'W':
		// 前进
		m_fDirection = 1.0f;
		break;
	case 'S':
		// 后退
		m_fDirection = -1.0f;
		break;
	default:
		return false;
	}

	return true;
}

bool CameraControl::OnKeyUp(size_t vk)
{
	switch (vk)
	{
	case 'A':
		// 左转
		m_fShiftDirection = -0.0f;
		break;
	case 'D':
		// 右转
		m_fShiftDirection = 0.0f;
		break;
	case 'W':
		// 前进
		m_fDirection = 0.0f;
		break;
	case 'S':
		// 后退
		m_fDirection = 0.0f;
		break;
	default:
		return false;
	}

	return true;
}

bool CameraControl::OnMouseMove(size_t param1, size_t param2)
{
	/*
	int x = get_x_lparam(param2);
	int y = get_y_lparam(param2);

	int delta_x = x - m_nClickX;
	int delta_y = y - m_nClickY;

	if ((param1 & MK_RBUTTON) != 0)
	{
 		size_t win_width = GetSystemMetrics(SM_CXSCREEN);
 		size_t win_height = GetSystemMetrics(SM_CYSCREEN);
 
 		// 左右转, 弧度
 		float angle = float(delta_x) / win_width * YY_PI;
 
 		YawAngle(-angle);
         float tmp_angle_y = m_pCamera->GetTransform()->GetAngle().y;
 
 		// 俯仰
 		angle = float(delta_y) / win_height * YY_PI;
 		PitchAngle(-angle);
	}

	m_nClickX = x;
	m_nClickY = y;
	*/
	return true;
}

bool CameraControl::OnMouseWheel(size_t param1, size_t param2)
{
	/*
	// 前进或后退
	int delta = short(HIWORD(param1));

#define WHEEL_DELTA      120      // Default value for rolling one notch
	delta = delta / WHEEL_DELTA;

	float len = float(delta) * 0.2f * m_move_speed; 

	if (len != 0)
	{
		//MoveLength(len);
		MoveUp(-len);
	}
	*/
	return false;
}

bool CameraControl::OnLButtonDown(size_t param1, size_t param2)
{
	m_nClickX = get_x_lparam(param2);
	m_nClickY = get_y_lparam(param2);

	return false;
}

bool CameraControl::OnLButtonUp(size_t param1, size_t param2)
{
	return false;
}

bool CameraControl::OnMButtonDown(size_t param1, size_t param2)
{
	m_nClickX = get_x_lparam(param2);
	m_nClickY = get_y_lparam(param2);

	return false;
}

bool CameraControl::OnMButtonUp(size_t param1, size_t param2)
{
	return false;
}


void CameraControl::CameraMoveBy(float x, float y, float z)
{
    ITransform* pTransform = m_pCamera->GetTransform();

    YY::Mat4f rotateM;
    pTransform->GetRotateMatrix(&rotateM);

    //YY::Vec4f v(x,y,z,1);
    //v = rotateM*v;

    YY::Mat4f translateM;
    translateM.Translate(x,y,z);
    translateM = rotateM*translateM;

    YY::Vec3f pos;
    pos.x = translateM.m_data[12];
    pos.y = translateM.m_data[13];
    pos.z = translateM.m_data[14];

    //pTransform->SetPos(pos);
    pTransform->AddPosX(pos.x);
    pTransform->AddPosY(pos.y);
    pTransform->AddPosZ(pos.z);
}

