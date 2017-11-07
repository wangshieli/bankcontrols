#pragma once
#include <stdio.h>
#include <Windows.h>
#include <winio.h>

#pragma comment(lib, "../bankshared/winio.lib")

#define NIO_HELP_INFO "\n   控件使用信息\n"	\
				"     * 此控件不需要键盘驱动设备支持\n"	\
				"\n   操作指令：\n"	\
				"     * -v/-V 获取控件版本信息\n"	\
				"     * -h/-H 获取帮助信息\n"

#define IO_HELP_INFO "\n   控件使用信息\n"	\
				"     * 此控件需要键盘驱动设备支持\n"	\
				"     * 请确保电脑以测试模式运行：\n"	\
				"        执行命令：bcdedit.exe /set testsigning on 并重启， 打开测试模式\n"	\
				"        执行命令：bcdedit.exe /set testsigning off 并重启， 关闭测试模式\n"	\
				"\n   操作指令：\n"	\
				"     * -v/-V 获取控件版本信息\n"	\
				"     * -h/-H 获取帮助信息\n"

#define ORDER_ERROR "-h/-H	使用此命令获取帮助信息"

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