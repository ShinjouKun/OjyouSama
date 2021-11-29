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
BYTE Input::oldkey[256] = { 0 };//�O�̏��

ComPtr<IDirectInputDevice8>devgamepad = nullptr;
ComPtr<IDirectInput8> dInput = nullptr;//�C���v�b�g
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
		assert("�L�[�{�[�h�̏��������s");
#endif // DEBUG
	}

	if (FAILED(joyInit()))
	{
#ifdef _DEBUG
		assert("�W���C�X�e�B�b�N�̏��������s");
#endif // DEBUG
	}

	return;

	HRESULT result;

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	//�C���v�b�g������
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&dInput, nullptr);
	result = dInput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);

	//���̓f�[�^�`���̃Z�b�g
	result = devKeyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	//�r�����䃌�x���̃Z�b�g
	result = devKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);


}

void Input::Update()
{
	//1�t���[���O�̏����㏑��
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
	result = devKeyboard->Acquire();//�L�[�{�[�h���̂����Ƃ��J�n


	result = devKeyboard->Acquire();//�L�[�{�[�h���̂����Ƃ��J�n
	//1�t���[���O�̃L�[���X�V
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}

	//�S�L�[�̓��͏�Ԃ��l��
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
	//�ő�1�ŕԂ�����1000�Ŋ���
	if (Math::abs(static_cast<float>(mCurrentJoyState.lZ)) > deadZone)
	{
		return mCurrentJoyState.lZ / 1000.f;
	}
	return 0.f;
}

bool Input::getLeftTrigger(float deadZone)
{
	//�ő�1�ŕԂ�����1000�Ŋ���
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
	//�ő�1�ŕԂ�����1000�Ŋ���
	if (Math::abs(static_cast<float>(mCurrentJoyState.lX)) > deadZone)
	{
		return mCurrentJoyState.lX / 1000.f;
	}
	return 0.f;
}

float Input::joyVertical(float deadZone)
{
	//�Ȃ������]���Ă邩��
	if (Math::abs(static_cast<float>(mCurrentJoyState.lY)) > deadZone)
	{
		return -mCurrentJoyState.lY / 1000.f;
	}
	return 0.f;
}

float Input::joyRightHorizontal(float deadZone)
{
	//�ő�1�ŕԂ�����1000�Ŋ���
	if (Math::abs(static_cast<float>(mCurrentJoyState.lRx)) > deadZone)
	{
		return mCurrentJoyState.lRx / 1000.f;
	}
	return 0.f;
}

float Input::joyRightVertical(float deadZone)
{
	//�Ȃ������]���Ă邩��
	if (Math::abs(static_cast<float>(mCurrentJoyState.lRy)) > deadZone)
	{
		return -mCurrentJoyState.lRy / 1000.f;
	}
	return 0.f;
}

bool Input::KeyDown(int keyNum)
{
#ifdef _DEBUG
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
#endif // DEBUG
	return false;
	if (keyNum < 0x00)return false;//�G���[�΍�
	if (keyNum > 0xff)return false;//�G���[�΍�
	if (!oldkey[keyNum] && key[keyNum])
	{
		return true;
	}
	return false;
}

bool Input::KeyState(int keyNum)
{
#ifdef _DEBUG
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
#endif // DEBUG
	return false;
	if (keyNum < 0x00)return false;//�G���[�΍�
	if (keyNum > 0xff)return false;//�G���[�΍�
	if (key[keyNum])
	{
		return true;
	}
	return false;
}


/*�@�Q�[���p�b�h�@*/

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
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
#endif // DEBUG
	return true;
	HRESULT result;


	/* �Q�[���p�b�h */
	dInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
		DeviceFindCallBack, NULL, DIEDFL_ATTACHEDONLY);
	if (!devgamepad) {
		return false;
	}

	result = devgamepad->SetDataFormat(&c_dfDIJoystick); // �W���`��
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�@�r������
	result = devgamepad->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�@�����[�h�ݒ�
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



	//�@���l�͈͐ݒ�
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = -1000;
	diprg.lMax = 1000;

	//�@X���̒l�͈͐ݒ�
	diprg.diph.dwObj = DIJOFS_X;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	//�@Y���̒l�͈͐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	//�@Z���̒l�͈͐ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	//�@Z���̒l�͈͐ݒ�
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
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
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
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
#endif // DEBUG
	return false;
	// �ُ�Ȉ��������o
	//assert(0 <= buttonNumber && buttonNumber <= 32);

	// 0�łȂ���Ή����Ă���
	if (pad_data.rgbButtons[buttonNumber]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerButton(BYTE buttonNumber)
{
#ifdef _DEBUG
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
#endif // DEBUG
	return false;
	// �ُ�Ȉ��������o
	//assert(0 <= buttonNumber && buttonNumber <= 32);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!buttonPre[buttonNumber] && pad_data.rgbButtons[buttonNumber]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::LeftStickTilt(LONG stickNumber)
{
#ifdef _DEBUG
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
#endif // DEBUG
	return false;
	//�@������

	//�@�������͈�
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
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
#endif // DEBUG
	return false;
	//�@�������͈�
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
	assert("���ݎg���Ȃ��̂ŁA�ς��Ă����Ă�");
#endif // DEBUG
	return false;
	//�@������

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
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&mDinput, NULL))) {
		return E_FAIL;
	}

	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(mDinput->CreateDevice(GUID_SysKeyboard, &mKeyDevice, NULL))) {
		return E_FAIL;
	}
	// �f�o�C�X���L�[�{�[�h�ɐݒ�
	if (FAILED(mKeyDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}
	// �������x���̐ݒ�
	if (FAILED(mKeyDevice->SetCooperativeLevel(mHwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
		return E_FAIL;
	}
	// �f�o�C�X���u�擾�v����
	mKeyDevice->Acquire();
	return S_OK;
}

HRESULT Input::joyInit()
{
	//���p�\�ȃQ�[���R���g���[���[�̗񋓊֐������s
	if (FAILED(mDinput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY))) {
		return E_FAIL;
	}
	if (!mPadDevice) { //�p�b�h�̐ڑ��͂����ɏI��
		return S_OK;
	}
	// �f�o�C�X���W���C�X�e�B�b�N�ɐݒ�
	if (FAILED(mPadDevice->SetDataFormat(&c_dfDIJoystick2))) {
		return E_FAIL;
	}
	// �������x���̐ݒ�
	if (FAILED(mPadDevice->SetCooperativeLevel(mHwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
		return E_FAIL;
	}
	//�A�v���P�[�V�����Ŏg�p����R���g���[���[�̃v���p�e�B��񋓂��Đݒ肷��
	//if (FAILED(mPadDevice->EnumObjects(enumObjectsCallback, NULL, DIDFT_ALL))) {
	//	return E_FAIL;
	//}
	if (FAILED(mPadDevice->EnumObjects(enumObjectsCallback, (void*)mEffectNumForceedbackAxis, DIDFT_AXIS))) {
		return E_FAIL;
	}

	//�G�t�F�N�g�̎��ƕ������Z�b�g
	DWORD rgdwAxes[2] = { DIJOFS_X,DIJOFS_Y };
	LONG rglDirection[2] = { 1,1 };//���̒l��ω�������ƃo�����X�������ł���

	//�\����
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

	// �f�o�C�X���u�擾�v����
	mPadDevice->Acquire();

	return S_OK;
}

//�R�[���o�b�N

BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	//�����񋓂����ꍇ�A���[�U�[�ɑI���E�m�F������
	WCHAR szConfirm[MAX_PATH];
	//���Ŏg�p�\�p�b�h��񋓂��邪�A���͍ŏ��̈�ŏ�����
	//wsprintf(szConfirm, L"���̕����f�o�C�X�Ńf�o�C�X�I�u�W�F�N�g���쐬���܂����H\n���i��%s\n�C���X�^���X��%s", pdidInstance->tszProductName, pdidInstance->tszInstanceName);
	//if (MessageBox(0, szConfirm, L"�m�F", MB_YESNO) == IDNO) {
	//    return DIENUM_CONTINUE;
	//}
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
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