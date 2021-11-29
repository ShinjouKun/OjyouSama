
#pragma once
#include"Windows.h"
#include"dinput.h"//インプット系
#include"wrl.h"

#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定

using namespace Microsoft::WRL;

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//キーコード
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

//コントローラー
enum class JoyCode
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LeftButton = 4,//Lボタン
	RightButton = 5,//Rボタン
	ViewButton = 6,//SELECT?BACK?
	MenuButton = 7,//スタートボタン
	LeftStickButton = 8,//左スティック押し込み
	RightStickButton = 9,//右スティック押し込み
};

class Input
{
public:
	Input();
	~Input();

	static void Init(HWND hwnd);//初期化
	static void Update();
	
	//キーが押された瞬間
	static bool getKeyDown(KeyCode key);
	//キーが押され続けているか
	static bool getKey(KeyCode key);
	//キーが離れた瞬間
	static bool getKeyUp(KeyCode key);

	//ジョイパッドが押された瞬間
	static bool getJoyDown(JoyCode joy);
	//ジョイパッドが押され続けているか
	static bool getJoy(JoyCode joy);
	//ジョイパッドが離れた瞬間
	static bool getJoyUp(JoyCode joy);

	//縦
	static int horizontal();
	static int ArrowHorizontal();
	//横
	static int vertical();
	static int ArrowVertical();
	
	///<summary>
	///ジョイスティック左、縦方向
	///<param name="num">範囲内は無視する値0.f～1.fの間</param>
	///<returns>どのくらい傾いているか</returns>
	///</summary>
	static float joyHorizontal(float num = 0.f);

	///<summary>
	///ジョイスティック左、横方向
	///<param name="num">範囲内は無視する値0.f～1.fの間</param>
	///<returns>どのくらい傾いているか</returns>
	///</summary>
	static float joyVertical(float num = 0.f);

	///<summary>
	///ジョイスティック右、縦方向
	///<param name="num">範囲内は無視する値0.f～1.fの間</param>
	///<returns>どのくらい傾いているか</returns>
	///</summary>
	static float joyRightHorizontal(float num = 0.f);

	///<summary>
	///ジョイスティック右、横方向
	///<param name="num">範囲内は無視する値0.f～1.fの間</param>
	///<returns>どのくらい傾いているか</returns>
	///</summary>
	static float joyRightVertical(float num = 0.f);



	static bool KeyDown(int keyNum);//今押したか？
	static bool KeyState(int keyNum);//押しっぱなしか？


	static bool InitGamepad(HWND hwnd);

	static void UpdateGamepad();


	/// <summary>
/// ボタン押下チェック
/// </summary>
/// <param name="buttonNumber">スティックボタン番号</param>
/// <returns>押されているか</returns>
	static bool PushButton(BYTE buttonNumber);

	/// <summary>
	/// ボタン押下チェック
	/// </summary>
	/// <param name="buttonNumber">スティックボタン番号</param>
	/// <returns>押されているか</returns>
	static bool TriggerButton(BYTE buttonNumber);

	/// <summary>
	/// 左スティックの傾きをチェック
	/// </summary>
	/// <param name="stickNumber">左スティック向き情報</param>
	/// <returns>傾いているか</returns>
	static bool LeftStickTilt(LONG stickNumber);

	/// <summary>
	/// 右スティックの傾きをチェック
	/// </summary>
	/// <param name="stickNumber">左スティック向き情報</param>
	/// <returns>傾いているか</returns>
	static bool RightStickTilt(LONG stickNumber);

	/// <summary>
	/// 十字ボタンをチェック
	/// </summary>
	/// <param name="buttonData">十字ボタン情報</param>
	/// <returns>押されているか</returns>
	static bool CrossButton(LONG buttonData);

private:
	//キーボード初期化
	static HRESULT keyBodeInit();
	//コントローラー初期化
	static HRESULT joyInit();

public:
	static LPDIRECTINPUT8 mDinput;//キーボードデバイス
	static LPDIRECTINPUTDEVICE8 mPadDevice;//ゲームパッドデバイス
	static LPDIRECTINPUTEFFECT mEffectDevice;//エフェクトオブジェクト
	static DWORD mEffectNumForceedbackAxis;

private:
	static BYTE mCurrentKeys[256];//現在のキー
	static BYTE mPreviousKeys[256];//1フレーム前のキー
	static DIJOYSTATE2 mCurrentJoyState;
	static DIJOYSTATE2 mPreviousJoyState;
	static LPDIRECTINPUTDEVICE8 mKeyDevice;

	static HWND mHwnd;

private:

	static ComPtr<IDirectInputDevice8> devKeyboard;
	//static ComPtr<IDirectInputDevice8> devgamepad;

	static BYTE key[256];
	static BYTE oldkey[256];//前の状態


	static BYTE buttonPre[32];

	static DIPROPDWORD diprop; //軸モード設定
	static DIPROPRANGE diprg; //軸の値範囲設定


public:

	static DIJOYSTATE pad_data;

};


/*　ゲームパッドコード　*/
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