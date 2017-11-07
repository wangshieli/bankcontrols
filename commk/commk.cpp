#include <bankshared.h>

#define COMMK_VERSION "bcmkey.exe v0.0.1"
#define COMMK_HELP ("\n  交通银行U盾密码输入控件：bcmkey.exe v0.0.1\n" NIO_HELP_INFO)

int wmain(int argc, wchar_t* argv[])
{
	int ret = BC_NO_ERROR;
	RECT rect = { 0 };
	if (argc != 2)
	{
		ret = BC_PARAMETER_ERROR;
		goto error;
	}

	if (argv[1][0] == L'-')
	{
		switch (towlower(argv[1][1]))
		{
		case L'v':
		{
			fprintf(stderr, "%s\n", COMMK_VERSION);
			return ret;
		}
		break;
		case L'h':
		{
			fprintf(stderr, "%s\n", COMMK_HELP);
			return ret;
		}
		break;
		default:
		{
			fprintf(stderr, "%s\n", ORDER_ERROR);
			return ret;
		}
		break;
		}
	}

	wchar_t* main_name = L"验证二代USBkey密码";

	HWND main = FindWindow(NULL, main_name);
	if (main == NULL) {
		ret = BC_NOKEYWINDOW_ERROR;
		goto error;
	}

	BringWindowToTop(main);
	SetWindowPos(main, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetWindowPos(main, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetForegroundWindow(main);
	HWND hForeWnd = GetForegroundWindow();
	DWORD dwCurID = GetCurrentThreadId();
	DWORD dwForeID = GetWindowThreadProcessId(hForeWnd, NULL);
	AttachThreadInput(dwCurID, dwForeID, FALSE);

	HWND edit = FindWindowEx(main, NULL, L"Edit", NULL);
	HWND button = FindWindowEx(main, NULL, L"Button", L"确定");

	if (edit == NULL || button == NULL) {
		ret = BC_NOCONTROL_ERROR;
		goto error;
	}

	if (GetWindowRect(edit, &rect) == false) {
		ret = BC_EDITNDOWN_ERROR;
		goto error;
	}
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	int button_dx = (rect.left + rect.right) / 2 * 65535 / screen_width;
	int button_dy = (rect.top + rect.bottom) / 2 * 65535 / screen_height;
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, button_dx, button_dy, 0, NULL);
	Sleep(100);
	keybd(edit, argv[1]);

	if (GetWindowRect(button, &rect) == false) {
		ret = BC_BUTTONDOWN_ERROR;
		goto error;
	}
	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);
	button_dx = (rect.left + rect.right) / 2 * 65535 / screen_width;
	button_dy = (rect.top + rect.bottom) / 2 * 65535 / screen_height;
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, button_dx, button_dy, 0, NULL);

error:
	if (0 != ret)
		fprintf(stderr, "%d\n", ret);

	return ret;
}