#include <bankshared.h>

#define CCB_VERSION "ccb.exe v0.0.1"
#define CCB_HELP ("\n  建设银行U盾密码输入控件：ccb.exe v0.0.1\n" IO_HELP_INFO)

int FindWeNeed(HWND& _button, HWND& _edit, wchar_t* _main_name, HWND _begin)
{
	int ret = 0;
	_begin = FindWindowEx(NULL, _begin, L"#32770", _main_name);
	if (NULL == _begin)
	{
		ret = BC_NOCONTROL_ERROR;
		return ret;
	}
	_button = FindWindowEx(_begin, NULL, L"Button", L"确  定");
	if (NULL == _button)
		_button = FindWindowEx(_begin, NULL, L"Button", L"OK");
	_edit = FindWindowEx(_begin, NULL, L"Edit", NULL);
	if (NULL == _button || NULL == _edit)
	{
		ret = FindWeNeed(_button, _edit, _main_name, _begin);
	}

	return ret;
}

int find(HWND& _button, HWND& _edit)
{
	int ret = 0;
	wchar_t* main_name = L"";
	_button = NULL;
	_edit = NULL;

	HWND main = FindWindow(L"#32770", main_name);
	if (main == NULL) {
		ret = BC_NOKEYWINDOW_ERROR;
		return ret;
	}
	_button = FindWindowEx(main, NULL, L"Button", L"确  定");
	if (NULL == _button)
		_button = FindWindowEx(main, NULL, L"Button", L"OK");
	_edit = FindWindowEx(main, NULL, L"Edit", NULL);
	if (NULL == _button || NULL == _edit)
	{
		main_name = L"";
		ret = FindWeNeed(_button, _edit, main_name, main);
	}

	return ret;
}

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
			fprintf(stderr, "%s\n", CCB_VERSION);
			return ret;
		}
		break;
		case L'h':
		{
			fprintf(stderr, "%s\n", CCB_HELP);
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

	if (!InitializeWinIo())
	{
		ret = BC_IOINIT_ERROR;
		goto error;
	}

	BOOL isnew = FALSE;
	wchar_t* main_name = L"中国建设银行网银盾";
	HWND button = NULL;
	HWND edit = NULL;
	HWND main = FindWindow(NULL, main_name);
	if (NULL == main)
	{
		main_name = L"zhongguojiansheyinhangwangyindun";
		main = FindWindow(L"#32770", main_name);
		if (NULL != main)
			isnew = TRUE;
	}

	if (NULL == main)
	{
		if (0 != (ret = find(button, edit)))
			goto error;
		isnew = TRUE;
	}
	else
	{
		button = FindWindowEx(main, NULL, L"Button", L"确定");
		if (NULL == button)
			button = FindWindowEx(main, NULL, L"Button", L"确  定");
		edit = FindWindowEx(main, NULL, L"Edit", NULL);
		if (NULL == button || NULL == edit)
		{
			if (0 != (ret = find(button, edit)))
				goto error;
			isnew = TRUE;
		}
	}

	int l = 0;
	int r = 0;
	int t = 0;
	int b = 0;
	int px = 0;
	int py = 0;
	do
	{
		SetFocus(edit);
		if (GetWindowRect(edit, &rect) == false) {
			ret = BC_EDITNDOWN_ERROR;
			goto error;
		}
		int screen_width = GetSystemMetrics(SM_CXSCREEN);
		int screen_height = GetSystemMetrics(SM_CYSCREEN);
		int len = (rect.right - rect.left) * 4 / 5;
		int button_dx = (rect.left + len) * 65535 / screen_width;
		int button_dy = (rect.top + rect.bottom) / 2 * 65535 / screen_height;
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, button_dx, button_dy, 0, NULL);

		POINT pt;
		GetCursorPos(&pt);
		l = rect.left * 65535 / screen_width;
		r = rect.right * 65535 / screen_width;
		t = rect.top * 65535 / screen_height;
		b = rect.bottom * 65535 / screen_height;

		px = pt.x * 65535 / screen_width;
		py = pt.y * 65535 / screen_height;
	} while (px < l || px > r || py < t || py > b);

	int nPassWordLen = lstrlenW(argv[1]);
	for (int i = 0; i < nPassWordLen; i++)
	{
		KEY_DOWN(8);
		KEY_UP(8);
	}

	for (int i = 0; i < nPassWordLen; i++)
	{
		short nKey = VkKeyScan(argv[1][i]);
		KEY_DOWN(nKey);
		KEY_UP(nKey);
	}

	if (isnew)
		::SendMessage(button, BM_CLICK, 0, 0);
	else
	{
		if (GetWindowRect(button, &rect) == false) {
			ret = BC_BUTTONDOWN_ERROR;
			goto error;
		}
		int screen_width = GetSystemMetrics(SM_CXSCREEN);
		int screen_height = GetSystemMetrics(SM_CYSCREEN);
		int button_dx = (rect.left + rect.right) / 2 * 65535 / screen_width;
		int button_dy = (rect.top + rect.bottom) / 2 * 65535 / screen_height;
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, button_dx, button_dy, 0, NULL);
	}

	ShutdownWinIo();

error:
	if (0 != ret)
		fprintf(stderr, "%d\n", ret);

	return ret;
}