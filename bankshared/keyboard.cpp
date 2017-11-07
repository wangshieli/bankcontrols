#include "bankshared.h"

void KBCWait4IBE()
{
	DWORD ch = 0;
	do
	{
		GetPortVal(key_cmd, &ch, 1);
	} while (ch & 0x2);
}

void KEY_DOWN(DWORD VirtualKey)
{
	Sleep(10);
	KBCWait4IBE();
	SetPortVal(0x64, 0xD2, 1);
	Sleep(10);
	KBCWait4IBE();
	SetPortVal(0x60, MapVirtualKey(VirtualKey, 0), 1);
}

void KEY_UP(DWORD VirtualKey)
{
	Sleep(10);
	KBCWait4IBE();
	SetPortVal(0x64, 0xD2, 1);
	Sleep(10);
	KBCWait4IBE();
	SetPortVal(0x60, (MapVirtualKey(VirtualKey, 0) | 0x80), 1);
}

void keybd(HWND hwnd, LPWSTR lp_password)
{
	size_t i;
	Sleep(50);
	bool is_capital = GetKeyState(VK_CAPITAL) & 0x0001;
	if (is_capital)
	{
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}
	for (i = 0; i < 20; i++)
	{
		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
	}
	Sleep(50);
	for (i = 0; i < wcslen(lp_password); i++)
	{
		short key = VkKeyScan(lp_password[i]);
		byte bh = (byte)(key >> 8);
		byte bl = (byte)key;

		if (bh == 1)
		{
			keybd_event(VK_SHIFT, 0, 0, 0);
			Sleep(1);
		}
		keybd_event(bl, 0, 0, 0);
		Sleep(1);
		keybd_event(bl, 0, KEYEVENTF_KEYUP, 0);
		Sleep(1);
		if (bh == 1)
		{
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
			Sleep(1);
		}
	}
	Sleep(50);

	if (is_capital)
	{
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}
}