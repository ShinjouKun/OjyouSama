
#pragma once
#include"Windows.h"
#include"dinput.h"//�C���v�b�g�n
#include"wrl.h"

#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��

using namespace Microsoft::WRL;

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//�L�[�R�[�h
enum class KeyCode
{
	A = DIK_A,
	B = DIK_B,
	C = DIK_C,
	D = DIK_D,
	E = DIK_E,
	F = DIK_F,
	G = DIK_G,
	H = DIK_H,
	I = DIK_I,
	J = DIK_J,
	K = DIK_K,
	L = DIK_L,
	M = DIK_M,
	N = DIK_N,
	O = DIK_O,
	P = DIK_P,
	Q = DIK_Q,
	R = DIK_R,
	S = DIK_S,
	T = DIK_T,
	U = DIK_U,
	V = DIK_V,
	W = DIK_W,
	X = DIK_X,
	Y = DIK_Y,
	Z = DIK_Z,
	SPACE = DIK_SPACE,
	LEFTSHIFT = DIK_LSHIFT,
	RIGHTSHIFT = DIK_RSHIFT,
	LEFTCONTROL = DIK_LCONTROL,
	RIGHTCONTROL = DIK_RCONTROL,
	LEFTALT = DIK_LALT,
	RIGHTALT = DIK_RALT,
	TAB = DIK_TAB,
	ESCAPE = DIK_ESCAPE,
	UPARROW = DIK_UPARROW,
	DOWNARROW = DIK_DOWNARROW,
	LEFTARROW = DIK_LEFTARROW,
	RIGHTARROW = DIK_RIGHTARROW,
	Alpha1 = DIK_1,
	Alpha2 = DIK_2,
	Alpha3 = DIK_3,
	Alpha4 = DIK_4,
	Alpha5 = DIK_5,
	Alpha6 = DIK_6,
	Alpha7 = DIK_7,
	Alpha8 = DIK_8,
	Alpha9 = DIK_9,
	Alpha0 = DIK_0,
	F1 = DIK_F1,
	F2 = DIK_F2,
	F3 = DIK_F3,
	F4 = DIK_F4,
	F5 = DIK_F5,
	F6 = DIK_F6,
	F7 = DIK_F7,
	F8 = DIK_F8,
	F9 = DIK_F9,
	F10 = DIK_F10,
	F11 = DIK_F11,
	F12 = DIK_F12,
	Enter = DIK_RETURN,
	Enter2 = DIK_NUMPADENTER,
	BACKSPACE = DIK_BACKSPACE,
	NONE = 512,
};

//�R���g���[���[
enum class JoyCode
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LeftButton = 4,//L�{�^��
	RightButton = 5,//R�{�^��
	ViewButton = 6,//SELECT?BACK?
	MenuButton = 7,//�X�^�[�g�{�^��
	LeftStickButton = 8,//���X�e�B�b�N��������
	RightStickButton = 9,//�E�X�e�B�b�N��������
};

class Input
{
public:
	Input();
	~Input();

	static void Init(HWND hwnd);//������
	static void Update();
	
	//�L�[�������ꂽ�u��
	static bool getKeyDown(KeyCode key);
	//�L�[�������ꑱ���Ă��邩
	static bool getKey(KeyCode key);
	//�L�[�����ꂽ�u��
	static bool getKeyUp(KeyCode key);

	//�W���C�p�b�h�������ꂽ�u��
	static bool getJoyDown(JoyCode joy);
	//�W���C�p�b�h�������ꑱ���Ă��邩
	static bool getJoy(JoyCode joy);
	//�W���C�p�b�h�����ꂽ�u��
	static bool getJoyUp(JoyCode joy);

	//�c
	static int horizontal();
	static int ArrowHorizontal();
	//��
	static int vertical();
	static int ArrowVertical();
	
	///<summary>
	///�W���C�X�e�B�b�N���A�c����
	///<param name="num">�͈͓��͖�������l0.f�`1.f�̊�</param>
	///<returns>�ǂ̂��炢�X���Ă��邩</returns>
	///</summary>
	static float joyHorizontal(float num = 0.f);

	///<summary>
	///�W���C�X�e�B�b�N���A������
	///<param name="num">�͈͓��͖�������l0.f�`1.f�̊�</param>
	///<returns>�ǂ̂��炢�X���Ă��邩</returns>
	///</summary>
	static float joyVertical(float num = 0.f);

	///<summary>
	///�W���C�X�e�B�b�N�E�A�c����
	///<param name="num">�͈͓��͖�������l0.f�`1.f�̊�</param>
	///<returns>�ǂ̂��炢�X���Ă��邩</returns>
	///</summary>
	static float joyRightHorizontal(float num = 0.f);

	///<summary>
	///�W���C�X�e�B�b�N�E�A������
	///<param name="num">�͈͓��͖�������l0.f�`1.f�̊�</param>
	///<returns>�ǂ̂��炢�X���Ă��邩</returns>
	///</summary>
	static float joyRightVertical(float num = 0.f);



	static bool KeyDown(int keyNum);//�����������H
	static bool KeyState(int keyNum);//�������ςȂ����H


	static bool InitGamepad(HWND hwnd);

	static void UpdateGamepad();


	/// <summary>
/// �{�^�������`�F�b�N
/// </summary>
/// <param name="buttonNumber">�X�e�B�b�N�{�^���ԍ�</param>
/// <returns>������Ă��邩</returns>
	static bool PushButton(BYTE buttonNumber);

	/// <summary>
	/// �{�^�������`�F�b�N
	/// </summary>
	/// <param name="buttonNumber">�X�e�B�b�N�{�^���ԍ�</param>
	/// <returns>������Ă��邩</returns>
	static bool TriggerButton(BYTE buttonNumber);

	/// <summary>
	/// ���X�e�B�b�N�̌X�����`�F�b�N
	/// </summary>
	/// <param name="stickNumber">���X�e�B�b�N�������</param>
	/// <returns>�X���Ă��邩</returns>
	static bool LeftStickTilt(LONG stickNumber);

	/// <summary>
	/// �E�X�e�B�b�N�̌X�����`�F�b�N
	/// </summary>
	/// <param name="stickNumber">���X�e�B�b�N�������</param>
	/// <returns>�X���Ă��邩</returns>
	static bool RightStickTilt(LONG stickNumber);

	/// <summary>
	/// �\���{�^�����`�F�b�N
	/// </summary>
	/// <param name="buttonData">�\���{�^�����</param>
	/// <returns>������Ă��邩</returns>
	static bool CrossButton(LONG buttonData);

private:
	//�L�[�{�[�h������
	static HRESULT keyBodeInit();
	//�R���g���[���[������
	static HRESULT joyInit();

public:
	static LPDIRECTINPUT8 mDinput;//�L�[�{�[�h�f�o�C�X
	static LPDIRECTINPUTDEVICE8 mPadDevice;//�Q�[���p�b�h�f�o�C�X
	static LPDIRECTINPUTEFFECT mEffectDevice;//�G�t�F�N�g�I�u�W�F�N�g
	static DWORD mEffectNumForceedbackAxis;

private:
	static BYTE mCurrentKeys[256];//���݂̃L�[
	static BYTE mPreviousKeys[256];//1�t���[���O�̃L�[
	static DIJOYSTATE2 mCurrentJoyState;
	static DIJOYSTATE2 mPreviousJoyState;
	static LPDIRECTINPUTDEVICE8 mKeyDevice;

	static HWND mHwnd;

private:

	static ComPtr<IDirectInputDevice8> devKeyboard;
	//static ComPtr<IDirectInputDevice8> devgamepad;

	static BYTE key[256];
	static BYTE oldkey[256];//�O�̏��


	static BYTE buttonPre[32];

	static DIPROPDWORD diprop; //�����[�h�ݒ�
	static DIPROPRANGE diprg; //���̒l�͈͐ݒ�


public:

	static DIJOYSTATE pad_data;

};


/*�@�Q�[���p�b�h�R�[�h�@*/
#define LSTICK_UP     0x00000001l //(pad_data.IY < 0)
#define LSTICK_DOWN   0x00000002l //(pad_data.IY > 0)
#define LSTICK_LEFT   0x00000004l //(pad_data.IX < 0)
#define LSTICK_RIGHT  0x00000008l //(pad_data.IX > 0)

#define RSTICK_UP     0x00000010l //(pad_data.IZ < 0)
#define RSTICK_DOWN   0x00000020l //(pad_data.IZ > 0)
#define RSTICK_LEFT   0x00000040l //(pad_data.IRz < 0)
#define RSTICK_RIGHT  0x00000080l //(pad_data.IRz > 0)

#define BUTTON_UP     0x00000001l //(pad_data.rgdwPOV.IY < 0)
#define BUTTON_DOWN   0x00000002l //(pad_data.rgdwPOV.IY > 0)
#define BUTTON_LEFT   0x00000004l //(pad_data.rgdwPOV.IX < 0)
#define BUTTON_RIGHT  0x00000008l //(pad_data.rgdwPOV.IX > 0)

#define BUTTON_X     (0) //(pad_data.rgbButtons[0])
#define BUTTON_Y     (1) //(pad_data.rgbButtons[1])
#define BUTTON_A     (2) //(pad_data.rgbButtons[2])
#define BUTTON_B     (3) //(pad_data.rgbButtons[3])
#define BUTTON_L1    (4) //(pad_data.rgbButtons[4])
#define BUTTON_R1    (5) //(pad_data.rgbButtons[5])
#define BUTTON_L2    (6) //(pad_data.rgbButtons[6])
#define BUTTON_R2    (7) //(pad_data.rgbButtons[7])
#define BUTTON_L3    (8) //(pad_data.rgbButtons[8])
#define BUTTON_R3    (9) //(pad_data.rgbButtons[9])
#define BUTTON_BACK  (10) //(pad_data.rgbButtons[10])
#define BUTTON_START (11) //(pad_data.rgbButtons[11])