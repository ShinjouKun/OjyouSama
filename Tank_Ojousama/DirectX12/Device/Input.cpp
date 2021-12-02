#include "Input.h"
#include <cassert>

#include "../Math/Math.h"
#include <algorithm>

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);
BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);
BOOL CALLBACK enumFFDevicesCallback(const DIDEVICEINSTANCE*, VOID*);
BOOL CALLBACK enumAxesCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);

BYTE Input::mCurrentKeys[256] = { 0 };
BYTE Input::mPreviousKeys[256] = { 0 };
DIJOYSTATE2 Input::mCurrentJoyState;
DIJOYSTATE2 Input::mPreviousJoyState;
LPDIRECTINPUT8 Input::mDinput = nullptr;
LPDIRECTINPUTDEVICE8 Input::mKeyDevice = nullptr;
LPDIRECTINPUTDEVICE8 Input::mPadDevice = nullptr;
LPDIRECTINPUTEFFECT Input::mEffectDevice = nullptr;
DWORD Input::mEffectNumForceedbackAxis;

HWND Input::mHwnd;
int Input::frameCount = 0;
//

ComPtr<IDirectInputDevice8> Input::devKeyboard = nullptr;
BYTE Input::key[256] = { 0 };
BYTE Input::oldkey[256] = { 0 };//前の状態

ComPtr<IDirectInputDevice8>devgamepad = nullptr;
ComPtr<IDirectInput8> dInput = nullptr;//インプット
DIJOYSTATE Input::pad_data;
BYTE Input::buttonPre[32] = { 0 };
DIPROPDWORD Input::diprop;
DIPROPRANGE Input::diprg;

Input::Input()
{
}

Input::~Input()
{

	if (mDinput)
	{
		SAFE_RELEASE(mDinput);
	}

	if (mKeyDevice)
	{
		SAFE_RELEASE(mKeyDevice);
	}

	if (mEffectDevice)
	{
		SAFE_RELEASE(mEffectDevice);
	}

	if (mPadDevice)
	{
		SAFE_RELEASE(mPadDevice);
	}
}

void Input::Init(HWND hwnd)
{


	mHwnd = hwnd;

	if (FAILED(keyBodeInit()))
	{
#ifdef _DEBUG
		assert("キーボードの初期化失敗");
#endif // DEBUG
	}

	if (FAILED(joyInit()))
	{
#ifdef _DEBUG
		assert("ジョイスティックの初期化失敗");
#endif // DEBUG
	}

	return;

	HRESULT result;

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	//インプット初期化
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&dInput, nullptr);
	result = dInput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);

	//入力データ形式のセット
	result = devKeyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	//排他制御レベルのセット
	result = devKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);


}

void Input::Update()
{
	//1フレーム前の情報を上書き
	memcpy_s(mPreviousKeys, sizeof(mPreviousKeys), mCurrentKeys, sizeof(mCurrentKeys));
	mPreviousJoyState = mCurrentJoyState;


	HRESULT hr = mKeyDevice->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE)) {
		mKeyDevice->GetDeviceState(sizeof(mCurrentKeys), &mCurrentKeys);
	}

	if (mPadDevice) {
		hr = mPadDevice->Acquire();
		if ((hr == DI_OK) || (hr == S_FALSE)) {
			mPadDevice->GetDeviceState(sizeof(DIJOYSTATE2), &mCurrentJoyState);
		}
	}
	else
	{
		if (60 < frameCount++)
		{
			frameCount = 0;
			joyInit();
		}
	}

	return;
	HRESULT result;
	result = devKeyboard->Acquire();//キーボード情報のかくとく開始


	result = devKeyboard->Acquire();//キーボード情報のかくとく開始
	//1フレーム前のキーを更新
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}

	//全キーの入力状態を獲得
	result = devKeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::getKeyDown(KeyCode key)
{
	return (mCurrentKeys[static_cast<BYTE>(key)] & 0x80 && !(mPreviousKeys[static_cast<BYTE>(key)] & 0x80));
}

bool Input::getKey(KeyCode key)
{
	return mCurrentKeys[static_cast<BYTE>(key)] & 0x80;
}

bool Input::getKeyUp(KeyCode key)
{
	return (!(mCurrentKeys[static_cast<BYTE>(key)] & 0x80) && mPreviousKeys[static_cast<BYTE>(key)] & 0x80);
}

bool Input::getJoyDown(JoyCode joy)
{
	return (mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80 && !(mPreviousJoyState.rgbButtons[static_cast<int>(joy)] & 0x80));
}

bool Input::getJoy(JoyCode joy)
{
	return mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80;
}

bool Input::getJoyUp(JoyCode joy)
{
	return (!(mCurrentJoyState.rgbButtons[static_cast<int>(joy)] & 0x80) && mPreviousJoyState.rgbButtons[static_cast<int>(joy)] & 0x80);
}

float Input::getTrigger(float deadZone)
{
	//最大1で返すため1000で割る
	if (Math::abs(static_cast<float>(mCurrentJoyState.lZ)) > deadZone)
	{
		return mCurrentJoyState.lZ / 1000.f;
	}
	return 0.f;
}

bool Input::getLeftTrigger(float deadZone)
{
	//最大1で返すため1000で割る
	if (static_cast<float>(mCurrentJoyState.lZ) > deadZone)
	{
		return true;
	}
	return false;
}

bool Input::getLeftTriggerDown(float deadZone)
{
	return (static_cast<float>(mCurrentJoyState.lZ) > deadZone && !(static_cast<float>(mPreviousJoyState.lZ) > deadZone));
}

bool Input::getLeftTriggerUp(float deadZone)
{
	return (!(static_cast<float>(mCurrentJoyState.lZ) > deadZone) && static_cast<float>(mPreviousJoyState.lZ) > deadZone);
}

bool Input::getRightTrigger(float deadZone)
{
	if (static_cast<float>(mCurrentJoyState.lZ) < -deadZone)
	{
		return true;
	}
	return false;
}

bool Input::getRightTriggerDown(float deadZone)
{
	return (static_cast<float>(mCurrentJoyState.lZ) < -deadZone && !(static_cast<float>(mPreviousJoyState.lZ) < -deadZone));
}

bool Input::getRightTriggerUp(float deadZone)
{
	return (!(static_cast<float>(mCurrentJoyState.lZ) < -deadZone) && static_cast<float>(mPreviousJoyState.lZ) < -deadZone);
}

int Input::horizontal()
{
	if (getKey(KeyCode::A) /*|| getKey(KeyCode::LEFTARROW)*/) {
		return -1;
	}
	else if (getKey(KeyCode::D) /*|| getKey(KeyCode::RIGHTARROW)*/) {
		return 1;
	}
	else {
		return 0;
	}
}

int Input::ArrowHorizontal()
{
	if (getKey(KeyCode::LEFTARROW)) {
		return -1;
	}
	else if (getKey(KeyCode::RIGHTARROW)) {
		return 1;
	}
	else {
		return 0;
	}
}

int Input::vertical()
{
	if (getKey(KeyCode::W) /*|| getKey(KeyCode::UPARROW)*/) {
		return 1;
	}
	else if (getKey(KeyCode::S) /*|| getKey(KeyCode::DOWNARROW)*/) {
		return -1;
	}
	else {
		return 0;
	}
}

int Input::ArrowVertical()
{
	if (getKey(KeyCode::UPARROW)) {
		return 1;
	}
	else if (getKey(KeyCode::DOWNARROW)) {
		return -1;
	}
	else {
		return 0;
	}
}

float Input::joyHorizontal(float deadZone)
{
	//最大1で返すため1000で割る
	if (Math::abs(static_cast<float>(mCurrentJoyState.lX)) > deadZone)
	{
		return mCurrentJoyState.lX / 1000.f;
	}
	return 0.f;
}

float Input::joyVertical(float deadZone)
{
	//なぜか反転してるから
	if (Math::abs(static_cast<float>(mCurrentJoyState.lY)) > deadZone)
	{
		return -mCurrentJoyState.lY / 1000.f;
	}
	return 0.f;
}

float Input::joyRightHorizontal(float deadZone)
{
	//最大1で返すため1000で割る
	if (Math::abs(static_cast<float>(mCurrentJoyState.lRx)) > deadZone)
	{
		return mCurrentJoyState.lRx / 1000.f;
	}
	return 0.f;
}

float Input::joyRightVertical(float deadZone)
{
	//なぜか反転してるから
	if (Math::abs(static_cast<float>(mCurrentJoyState.lRy)) > deadZone)
	{
		return -mCurrentJoyState.lRy / 1000.f;
	}
	return 0.f;
}

bool Input::KeyDown(int keyNum)
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG
	return false;
	if (keyNum < 0x00)return false;//エラー対策
	if (keyNum > 0xff)return false;//エラー対策
	if (!oldkey[keyNum] && key[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::KeyState(int keyNum)
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG
	return false;
	if (keyNum < 0x00)return false;//エラー対策
	if (keyNum > 0xff)return false;//エラー対策
	if (key[keyNum])
	{
		return true;
	}
	return false;
}


/*　ゲームパッド　*/

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	HRESULT result;

	result = dInput->CreateDevice(ipddi->guidInstance,
		&devgamepad, NULL);
	if (FAILED(result)) {

		return DIENUM_CONTINUE;
	}


	return DIENUM_STOP;
}


bool Input::InitGamepad(HWND hwnd)
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG
	return true;
	HRESULT result;


	/* ゲームパッド */
	dInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
		DeviceFindCallBack, NULL, DIEDFL_ATTACHEDONLY);
	if (!devgamepad) {
		return false;
	}

	result = devgamepad->SetDataFormat(&c_dfDIJoystick); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//　排他制御
	result = devgamepad->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//　軸モード設定
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;

	result = devgamepad->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}



	//　軸値範囲設定
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	//　X軸の値範囲設定
	diprg.diph.dwObj = DIJOFS_X;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	//　Y軸の値範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	//　Z軸の値範囲設定
	diprg.diph.dwObj = DIJOFS_Z;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//　Z軸の値範囲設定
	diprg.diph.dwObj = DIJOFS_RZ;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	result = devgamepad->Acquire();

	return true;
}

void Input::UpdateGamepad()
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG

	return;
	HRESULT result;
	memcpy(buttonPre, pad_data.rgbButtons, sizeof(pad_data.rgbButtons));

	if (devgamepad) {
		devgamepad->Poll();
		result = devgamepad->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
		if (result == DIERR_INPUTLOST) {
			result = devgamepad->Acquire();

		}
	}

}

bool Input::PushButton(BYTE buttonNumber)
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG
	return false;
	// 異常な引数を検出
	//assert(0 <= buttonNumber && buttonNumber <= 32);

	// 0でなければ押している
	if (pad_data.rgbButtons[buttonNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerButton(BYTE buttonNumber)
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG
	return false;
	// 異常な引数を検出
	//assert(0 <= buttonNumber && buttonNumber <= 32);

	// 前回が0で、今回が0でなければトリガー
	if (!buttonPre[buttonNumber] && pad_data.rgbButtons[buttonNumber]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::LeftStickTilt(LONG stickNumber)
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG
	return false;
	//　未実装

	//　無反応範囲
	DWORD unresponsive_range = 0;

	switch (stickNumber)
	{
	case LSTICK_UP:

		return pad_data.lY < unresponsive_range;
	case LSTICK_DOWN:

		return pad_data.lY < unresponsive_range;
	case LSTICK_RIGHT:

		return pad_data.lX > unresponsive_range;
	case LSTICK_LEFT:

		return pad_data.lX < unresponsive_range;

	}



	return false;
}

bool Input::RightStickTilt(LONG stickNumber)
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG
	return false;
	//　無反応範囲
	DWORD unresponsive_range = 0;

	switch (stickNumber)
	{
	case RSTICK_UP:

		return pad_data.lZ < unresponsive_range;
	case RSTICK_DOWN:

		return pad_data.lZ < unresponsive_range;
	case RSTICK_RIGHT:

		return pad_data.lRz > unresponsive_range;
	case RSTICK_LEFT:

		return pad_data.lRz < unresponsive_range;

	}


	return false;
}

bool Input::CrossButton(LONG buttonData)
{
#ifdef _DEBUG
	assert("現在使えないので、変えておいてね");
#endif // DEBUG
	return false;
	//　未実装

	if (pad_data.rgdwPOV[0] != buttonData)
	{
		/*switch (pad_data.rgdwPOV[0])
		{
		case 0:
			break;
		}*/
	}


	return false;
}

HRESULT Input::keyBodeInit()
{
	// 「DirectInput」オブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&mDinput, NULL))) {
		return E_FAIL;
	}

	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(mDinput->CreateDevice(GUID_SysKeyboard, &mKeyDevice, NULL))) {
		return E_FAIL;
	}
	// デバイスをキーボードに設定
	if (FAILED(mKeyDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}
	// 協調レベルの設定
	if (FAILED(mKeyDevice->SetCooperativeLevel(mHwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
		return E_FAIL;
	}
	// デバイスを「取得」する
	mKeyDevice->Acquire();
	return S_OK;
}

HRESULT Input::joyInit()
{
	//利用可能なゲームコントローラーの列挙関数を実行
	if (FAILED(mDinput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY))) {
		return E_FAIL;
	}
	if (!mPadDevice) { //パッドの接続はせずに終了
		return S_OK;
	}
	// デバイスをジョイスティックに設定
	if (FAILED(mPadDevice->SetDataFormat(&c_dfDIJoystick2))) {
		return E_FAIL;
	}
	// 協調レベルの設定
	if (FAILED(mPadDevice->SetCooperativeLevel(mHwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
		return E_FAIL;
	}
	//アプリケーションで使用するコントローラーのプロパティを列挙して設定する
	//if (FAILED(mPadDevice->EnumObjects(enumObjectsCallback, NULL, DIDFT_ALL))) {
	//	return E_FAIL;
	//}
	if (FAILED(mPadDevice->EnumObjects(enumObjectsCallback, (void*)mEffectNumForceedbackAxis, DIDFT_AXIS))) {
		return E_FAIL;
	}

	//エフェクトの軸と方向をセット
	DWORD rgdwAxes[2] = { DIJOFS_X,DIJOFS_Y };
	LONG rglDirection[2] = { 1,1 };//この値を変化させるとバランス調整ができる

	//構造体
	DICONSTANTFORCE cf;
	DIEFFECT eff;

	ZeroMemory(&eff, sizeof(eff));
	eff.dwSize = sizeof(DIEFFECT);
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.dwDuration = INFINITE;
	eff.dwSamplePeriod = 0;
	eff.dwGain = DI_FFNOMINALMAX;
	eff.dwTriggerButton = DIEB_NOTRIGGER;
	eff.dwTriggerRepeatInterval = 0;
	eff.cAxes = mEffectNumForceedbackAxis;
	eff.rgdwAxes = rgdwAxes;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;//
	eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	eff.lpvTypeSpecificParams = &cf;
	eff.dwStartDelay = 0;


	if (FAILED(mPadDevice->CreateEffect(GUID_ConstantForce, &eff, &mEffectDevice, NULL)))
	{
		return E_FAIL;
	}

	// デバイスを「取得」する
	mPadDevice->Acquire();

	return S_OK;
}

//コールバック

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	//複数列挙される場合、ユーザーに選択・確認させる
	WCHAR szConfirm[MAX_PATH];
	//↓で使用可能パッドを列挙するが、今は最初の一つで初期化
	//wsprintf(szConfirm, L"この物理デバイスでデバイスオブジェクトを作成しますか？\n商品名%s\nインスタンス名%s", pdidInstance->tszProductName, pdidInstance->tszInstanceName);
	//if (MessageBox(0, szConfirm, L"確認", MB_YESNO) == IDNO) {
	//    return DIENUM_CONTINUE;
	//}
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(Input::mDinput->CreateDevice(pdidInstance->guidInstance, &Input::mPadDevice, NULL))) {
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

BOOL CALLBACK enumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
	if (pdidoi->dwType & DIDFT_AXIS) {
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = -1000;
		diprg.lMax = 1000;

		if (FAILED(Input::mPadDevice->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

BOOL CALLBACK enumFFDevicesCallback(const DIDEVICEINSTANCE* pInput, VOID* pContext)
{
	HRESULT hr;

	hr = Input::mDinput->CreateDevice(pInput->guidInstance, &Input::mPadDevice, NULL);
	if (FAILED(hr))return DIENUM_CONTINUE;

	return DIENUM_STOP;
}

BOOL CALLBACK enumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
	HRESULT hr;
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	hr = Input::mPadDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr))return DIENUM_STOP;

	DWORD* pdwNumForceFeedbackAxis = (DWORD*)pContext;
	if ((pdidoi->dwFlags&DIDOI_FFACTUATOR) != 0)(*pdwNumForceFeedbackAxis)++;

	return DIENUM_CONTINUE;
}