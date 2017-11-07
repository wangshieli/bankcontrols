#include <bankshared.h>

#define ABCP_VERSION "abcpay.exe v0.0.1"
#define ABCP_HELP ("\n  农业银行交易密码输入控件：abcpay.exe v0.0.1\n" IO_HELP_INFO)

BOOL CALLBACK FindEdit(HWND hwnd, LPARAM lparam)
{
	HWND* p = (HWND*)lparam;
	RECT rect = { 0 };
	LPWSTR lp_window_text = new WCHAR[128];
	GetClassName(hwnd, lp_window_text, 128);

	if (wcsncmp(lp_window_text, L"Edit", 128) == 0)
	{
		*p = hwnd;

		BringWindowToTop(hwnd);
		SetForegroundWindow(hwnd);

		int l = 0;
		int r = 0;
		int t = 0;
		int b = 0;
		int px = 0;
		int py = 0;
		do
		{
			if (GetWindowRect(hwnd, &rect) == false) {
				delete lp_window_text;
				lp_window_text = NULL;
				return TRUE;
			}
			int screen_width = GetSystemMetrics(SM_CXSCREEN);
			int screen_height = GetSystemMetrics(SM_CYSCREEN);
			int button_dx = (rect.left + rect.right) / 2 * 65535 / screen_width;
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

		delete lp_window_text;
		lp_window_text = NULL;
		return FALSE;
	}

	delete lp_window_text;
	lp_window_text = NULL;
	return TRUE;
}

BOOL CALLBACK FindWeb(HWND hwnd, LPARAM lparam)
{
	LPWSTR lp_window_text = new WCHAR[128];
	GetWindowText(hwnd, lp_window_text, 128);

	if (wcsstr(lp_window_text, L"中国农业银行 - 网上支付中心") > 0)
	{
		if (IsIconic(hwnd))
			ShowWindow(hwnd, SW_RESTORE);
		else
			ShowWindow(hwnd, SW_SHOW);
		BringWindowToTop(hwnd);
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		SetForegroundWindow(hwnd);
		HWND hForeWnd = GetForegroundWindow();
		DWORD dwCurID = GetCurrentThreadId();
		DWORD dwForeID = GetWindowThreadProcessId(hForeWnd, NULL);
		AttachThreadInput(dwCurID, dwForeID, FALSE);

		HWND hFrameTab = FindWindowEx(hwnd, NULL, L"Frame Tab", NULL);
		HWND hWeNeedWnd = FindWindowEx(hFrameTab, NULL, L"TabWindowClass", NULL);
		HWND hwnd_child = NULL;
		hwnd_child = FindWindowEx(hWeNeedWnd, hwnd_child, L"Shell DocObject View", NULL);
		if (hwnd_child == NULL)
		{
			delete lp_window_text;
			lp_window_text = NULL;
			return TRUE;
		}

		HWND hwnd_child01 = FindWindowEx(hwnd_child, NULL, L"Internet Explorer_Server", NULL);
		if (hwnd_child01 == NULL)
		{
			delete lp_window_text;
			lp_window_text = NULL;
			return TRUE;
		}

		EnumChildWindows(hwnd_child01, FindEdit, lparam);

		delete lp_window_text;
		lp_window_text = NULL;
		return FALSE;
	}
	delete lp_window_text;
	lp_window_text = NULL;
	return TRUE;
}

int wmain(int argc, wchar_t* argv[])
{
	int ret = BC_NO_ERROR;
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
			fprintf(stderr, "%s\n", ABCP_VERSION);
			return ret;
		}
		break;
		case L'h':
		{
			fprintf(stderr, "%s\n", ABCP_HELP);
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

	HWND hEdit = NULL;
	EnumChildWindows(NULL, FindWeb, (LPARAM)&hEdit);
	if (NULL == hEdit)
	{
		ret = BC_EDITNDOWN_ERROR;
		goto error;
	}

	int nPassWordLen = lstrlenW(argv[1]);
	int nPos = 0;
	do
	{
		SendMessage(hEdit, EM_SETSEL, 0, -1); // 选择全部内容
		KEY_DOWN(46);
		KEY_UP(46);
		SendMessage(hEdit, EM_SETSEL, -2, -1);// 将光标放到最后

		int i1 = 0, j = 0, k = 0;
		i1 = SendMessage(hEdit, EM_GETSEL, 0, 0);
		j = HIWORD(i1);
		int ln = SendMessage(hEdit, EM_LINEFROMCHAR, j, 0) + 1;
		k = SendMessage(hEdit, EM_LINEINDEX, -1, 0);
		nPos = j - k + 1;
	} while (nPos > 1);

	for (int i = 0; i < nPassWordLen; i++)
	{
		int col2 = 0;
		int col = 0;
		do
		{
			int i1 = 0, j = 0, k = 0;
			i1 = SendMessage(hEdit, EM_GETSEL, 0, 0);
			j = HIWORD(i1);
			int ln = SendMessage(hEdit, EM_LINEFROMCHAR, j, 0) + 1;
			k = SendMessage(hEdit, EM_LINEINDEX, -1, 0);
			col = j - k + 1;
			short nKey = VkKeyScan(argv[1][i]);
			KEY_DOWN(nKey);
			KEY_UP(nKey);
			Sleep(50);
			i1 = SendMessage(hEdit, EM_GETSEL, 0, 0);
			j = HIWORD(i1);
			ln = SendMessage(hEdit, EM_LINEFROMCHAR, j, 0) + 1;
			k = SendMessage(hEdit, EM_LINEINDEX, -1, 0);
			col2 = j - k + 1;

			if (i < (nPassWordLen - 1) && col2 == (nPassWordLen + 1))
			{
				i = -1;
				int nPos = 0;
				do
				{
					SendMessage(hEdit, EM_SETSEL, 0, -1); // 选择全部内容
					KEY_DOWN(46);
					KEY_UP(46);
					SendMessage(hEdit, EM_SETSEL, -2, -1);// 将光标放到最后

					int i1 = 0, j = 0, k = 0;
					i1 = SendMessage(hEdit, EM_GETSEL, 0, 0);
					j = HIWORD(i1);
					int ln = SendMessage(hEdit, EM_LINEFROMCHAR, j, 0) + 1;
					k = SendMessage(hEdit, EM_LINEINDEX, -1, 0);
					nPos = j - k + 1;
				} while (nPos > 1);
				break;
			}
		} while (col == col2);
	}

	ShutdownWinIo();

error:
	if (0 != ret)
		fprintf(stderr, "%d\n", ret);

	return ret;
}