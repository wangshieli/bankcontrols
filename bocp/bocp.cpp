#include <bankshared.h>

#define BOCP_VERSION "bocp.exe v0.0.1"
#define BOCP_HELP ("\n  中国银行U盾密码输入控件：bocp.exe v0.0.1\n" NIO_HELP_INFO)

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
			fprintf(stderr, "%s\n", BOCP_VERSION);
			return ret;
		}
		break;
		case L'h':
		{
			fprintf(stderr, "%s\n", BOCP_HELP);
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

	BOOL bFlag = FALSE;

	HWND main = FindWindow(NULL, L"验证二代USBkey密码");
	if (main == NULL) {
		main = FindWindow(NULL, L"检验用户密码");
		if (main == NULL)
		{
			main = FindWindow(NULL, L"USBKey密码");
			if (main == NULL)
			{
				ret = BC_NOKEYWINDOW_ERROR;
				goto error;
			}
			bFlag = TRUE;
		}
	}
	BringWindowToTop(main);
	SetWindowPos(main, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetWindowPos(main, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetForegroundWindow(main);
	HWND hForeWnd = GetForegroundWindow();
	DWORD dwCurID = GetCurrentThreadId();
	DWORD dwForeID = GetWindowThreadProcessId(hForeWnd, NULL);
	AttachThreadInput(dwCurID, dwForeID, FALSE);

	HWND edit = NULL;
	HWND button = NULL;
	edit = FindWindowEx(main, NULL, L"Edit", NULL);
	if (!bFlag)
	{
		button = FindWindowEx(main, NULL, L"Button", L"确定");
	}
	else
	{
		button = FindWindowEx(main, NULL, L"Button", L"确  定");
	}

	if (edit == NULL || button == NULL) {
		ret = BC_NOCONTROL_ERROR;
		goto error;
	}

	keybd(edit, argv[1]);

	Sleep(500);
	::SendMessage(button, BM_CLICK, 0, 0);

error:
	if (0 != ret)
		fprintf(stderr, "%d\n", ret);

	return ret;
}