#pragma once
#include <stdio.h>
#include <Windows.h>
#include <winio.h>

#pragma comment(lib, "../bankshared/winio.lib")

#define NIO_HELP_INFO "\n   �ؼ�ʹ����Ϣ\n"	\
				"     * �˿ؼ�����Ҫ���������豸֧��\n"	\
				"\n   ����ָ�\n"	\
				"     * -v/-V ��ȡ�ؼ��汾��Ϣ\n"	\
				"     * -h/-H ��ȡ������Ϣ\n"

#define IO_HELP_INFO "\n   �ؼ�ʹ����Ϣ\n"	\
				"     * �˿ؼ���Ҫ���������豸֧��\n"	\
				"     * ��ȷ�������Բ���ģʽ���У�\n"	\
				"        ִ�����bcdedit.exe /set testsigning on �������� �򿪲���ģʽ\n"	\
				"        ִ�����bcdedit.exe /set testsigning off �������� �رղ���ģʽ\n"	\
				"\n   ����ָ�\n"	\
				"     * -v/-V ��ȡ�ؼ��汾��Ϣ\n"	\
				"     * -h/-H ��ȡ������Ϣ\n"

#define ORDER_ERROR "-h/-H	ʹ�ô������ȡ������Ϣ"

typedef enum _bc_error
{
	BC_NO_ERROR,
	BC_PARAMETER_ERROR = 101,
	BC_IOINIT_ERROR,
	BC_NOKEYWINDOW_ERROR,
	BC_NOCONTROL_ERROR,
	BC_BUTTONDOWN_ERROR,
	BC_EDITNDOWN_ERROR,
}BC_ERROR;

#define key_cmd 0x64
#define key_dat 0x60

void KBCWait4IBE();
void KEY_DOWN(DWORD VirtualKey);
void KEY_UP(DWORD VirtualKey);

void keybd(HWND hwnd, LPWSTR lp_password);