// Tetris.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "Tetris.h"
#include <vector>
#include <cstdlib> // 包含 srand 和 rand 函数
#include <ctime>   // 包含 time 函数
using namespace std;
#define MAX_LOADSTRING 100

// 位图资源：
HBITMAP bmp_StartButton;     // 开始按钮图像资源
HBITMAP bmp_Green;            // 绿砖
HBITMAP bmp_Red;           // 红砖
HBITMAP bmp_Yellow;          //黄砖          
HBITMAP bmp_Line;          //边线  
//HBITMAP bmp_WhiteBackground; // 生成的纯白色背景图像
//HBITMAP bmp_GameBackground;  // 生成的游戏背景图像
HBITMAP bmp_Bg;  //游戏背景图像

// 游戏状态：
Stage* currentStage = NULL; // 当前场景状态
vector<Button*> buttons;   // 按钮
Brick* brick = NULL;        // 砖块
Block* block = nullptr;
std::vector<Brick*> base;
//创建场景数据数组
vector<vector<int>> Data(BG_ROWS + 10, vector<int>(BG_COLUMNS + 10, 0));

//recheck
int recheck_times = 0;
//移动锁
int left_lock = 0;
int right_lock = 0;
int up_lock = 0;
int down_lock = 0;
//长按计时器
int left_long_press = 0;
int right_long_press = 0;
int up_long_press = 0;
int down_long_press = 0;
//自然掉落间隔
int time_space = 20;
//固化判断
bool flag = 0;
//方块
//T
int shape_T[4][4][4] = {
	{{0,1,0,0},
	{1,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},

	{{0,1,0,0},
	{0,1,1,0},
	{0,1,0,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{1,1,1,0},
	{0,1,0,0},
	{0,0,0,0}},

	{{0,1,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,0,0,0}},
};
//1
int shape_1[4][4][4] = {
	{{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}},

	{{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0}},
	{{0,0,0,0},
	{1,1,1,1},
	{0,0,0,0},
	{0,0,0,0}},

	{{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,1,0,0}}
};
//O
int shape_O[4][4][4] = {
	{{1,1,0,0},
	{1,1,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{0,0,0,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0}},
	{{0,0,0,0},
	{1,1,0,0},
	{1,1,0,0},
	{0,0,0,0}},
};
//L
int shape_L[4][4][4] = {
	{{0,1,0,0},
	{0,1,0,0},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{1,1,1,0},
	{1,0,0,0},
	{0,0,0,0}},

	{{1,1,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,0,0,0}},

	{{0,0,1,0},
	{1,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},
};

//L-Reflected
int shape_LR[4][4][4] = {
	{{0,1,0,0},
	{0,1,0,0},
	{1,1,0,0},
	{0,0,0,0}},

	{{1,0,0,0},
	{1,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},

	{{0,1,1,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{1,1,1,0},
	{0,0,1,0},
	{0,0,0,0}},
};

//Z
int shape_Z[4][4][4] = {
	{{1,1,0,0},
	{0,1,1,0},
	{0,0,0,0},
	{0,0,0,0}},

	{{0,0,1,0},
	{0,1,1,0},
	{0,1,0,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{1,1,0,0},
	{0,1,1,0},
	{0,0,0,0}},

	{{0,1,0,0},
	{1,1,0,0},
	{1,0,0,0},
	{0,0,0,0}},
};
//Z-Reflected
int shape_ZR[4][4][4] = {
	{{0,1,1,0},
	{1,1,0,0},
	{0,0,0,0},
	{0,0,0,0}},

	{{0,1,0,0},
	{0,1,1,0},
	{0,0,1,0},
	{0,0,0,0}},

	{{0,0,0,0},
	{0,1,1,0},
	{1,1,0,0},
	{0,0,0,0}},

	{{1,0,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,0,0,0}},
};
//形状
int shapeSet[7][4][4][4] = {};

// 键盘：
bool keyUpDown = false;    // 上
bool keyDownDown = false;  // 下
bool keyLeftDown = false;  // 左
bool keyRightDown = false; // 右

// 鼠标：
int mouseX = 0;         // 鼠标X坐标
int mouseY = 0;         // 鼠标Y坐标
bool mouseDown = false; // 鼠标按下

int Cnt = 0;

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TETRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TETRIS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(
		szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// 初始化游戏窗体
		InitGame(hWnd, wParam, lParam);
	}
	break;
	case WM_KEYDOWN:
	{
		// 键盘按下事件
		KeyDown(hWnd, wParam, lParam);
	}
	break;
	case WM_KEYUP:
	{
		// 键盘松开事件
		KeyUp(hWnd, wParam, lParam);
	}
	break;
	case WM_MOUSEMOVE:
	{
		// 鼠标移动事件
		MouseMove(hWnd, wParam, lParam);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		// 鼠标左键按下事件
		LButtonDown(hWnd, wParam, lParam);
	}
	break;
	case WM_LBUTTONUP:
	{
		// 鼠标左键松开事件
		LButtonUp(hWnd, wParam, lParam);
	}
	break;
	case WM_TIMER:
	{
		if (wParam == TIMER_GAMETIMER) {
			// 判断游戏是否暂停
			if (currentStage != NULL && !currentStage->pause)
			{
				TimerUpdate(hWnd, wParam, lParam);
			}
		}
		else if (wParam == TIMER_PAINTTIMER) {
			if (currentStage != NULL && !currentStage->pause)
			{
				PaintUpdate(hWnd, wParam, lParam);
			}
		}
		else if (wParam == TIMER_CONTROLTIMER)
		{
			if (currentStage != NULL && !currentStage->pause)
			{
				ControlTimerUpdate(hWnd, wParam, lParam);
			}
		}
	}
	break;
	case WM_PAINT:
	{
		// 绘图
		Paint(hWnd);
	}
	break;

	// TODO：其他消息
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
	{
		// 窗口退出
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#pragma region 事件处理函数

// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// 加载图像资源
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_Green = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GREEN));
	bmp_Yellow = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_YELLOW));
	bmp_Red = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_RED));
	bmp_Bg = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BG));
	//bmp_Line = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_LINE));
	//bmp_WhiteBackground = CreateWhiteBackground(hWnd);
	//bmp_GameBackground = CreateGameBackground(hWnd);

	//形状载入
	CopyShape(shape_1, shapeSet[0]);
	CopyShape(shape_L, shapeSet[1]);
	CopyShape(shape_O, shapeSet[2]);
	CopyShape(shape_Z, shapeSet[3]);
	CopyShape(shape_LR, shapeSet[4]);
	CopyShape(shape_ZR, shapeSet[5]);
	CopyShape(shape_T, shapeSet[6]);

	// 初始化开始场景
	InitStage(hWnd, STAGE_START);

	// 初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
	SetTimer(hWnd, TIMER_PAINTTIMER, TIMER_PAINTTIMER_ELAPSE, NULL);
	SetTimer(hWnd, TIMER_CONTROLTIMER, TIMER_CONTROLTIMER_ELAPSE, NULL);
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 0x57:  // W
	case VK_UP: // 方向键上
		keyUpDown = true;
		break;
	case 0x53:    // S
	case VK_DOWN: // 方向键下
		keyDownDown = true;
		break;
	case 0x41:    // A
	case VK_LEFT: // 方向键左
		keyLeftDown = true;
		break;
	case 0x44:     // D
	case VK_RIGHT: // 方向键右
		keyRightDown = true;
		break;

		// TODO：其他按键
	default:
		break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 0x57:  // W
	case VK_UP: // 方向键上
		keyUpDown = false;
		break;
	case 0x53:    // S
	case VK_DOWN: // 方向键下
		keyDownDown = false;
		break;
	case 0x41:    // A
	case VK_LEFT: // 方向键左
		keyLeftDown = false;
		left_lock = 0;
		break;
	case 0x44:     // D
	case VK_RIGHT: // 方向键右
		keyRightDown = false;
		right_lock = 0;
		break;

		// TODO：其他按键
	default:
		break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

	// 判断鼠标是否点击按钮
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->x <= mouseX && button->x + button->width >= mouseX && button->y <= mouseY && button->y + button->height >= mouseY)
		{
			switch (button->buttonID)
			{
			case BUTTON_STARTGAME_ID:
				InitStage(hWnd, STAGE_GAME);
				break;

				// TODO: 其他按钮
			default:
				break;
			}
		}
	}
}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// 状态更新
	switch (currentStage->stageID)
	{
	case STAGE_START:
		break;
	case STAGE_GAME:
	{
		UpdateBrick(hWnd);
		UpdateBlock(hWnd);
	}
	break;

	// TODO：添加多个游戏场景
	default:
		break;
	}

	// 刷新显示
	// InvalidateRect(hWnd, NULL, FALSE);
}

void PaintUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// 动画更新
	/*
	if (currentStage->stageID == STAGE_GAME) {
		brick->frame_index = (brick->frame_index + 1) % BRICK_FRAME_SIZE;
	}
	*/
	InvalidateRect(hWnd, NULL, FALSE);
}

void ControlTimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (keyUpDown&& up_lock ==1)
	{
		up_long_press++;
	}
	if (keyUpDown==0)
	{
		up_lock = 0;
	}
	if ((keyUpDown&&up_lock==0||up_long_press>2)&&block!=nullptr)
	{
		up_long_press = 0; up_lock = 1;
		bool shape_trans = 1;
		up_long_press = 0;
		block->shape_id = (block->shape_id + 1) % 4;
		memcpy(block->shape, shapeSet[block->type][block->shape_id], sizeof(block->shape));
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block->shape[i][j])
				{
					if (block->x + j < 0)
					{
						shape_trans = 0;
					}
					else if (Data[block->x + j][block->y + i] == 1)
					{
						shape_trans = 0;
					}
				}
			}
		}
		if (shape_trans == 0)
		{
			block->shape_id = (block->shape_id + 3) % 4;
			memcpy(block->shape, shapeSet[block->type][block->shape_id], sizeof(block->shape));
		}
		else
		{
			up_lock = 1;
			if (Cnt%time_space==0)
			{
				block->y--;
			}
		}
	}

}

void CopyShape(int src[4][4][4], int des[4][4][4])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				des[i][j][k] = src[i][j][k];
			}
		}
	}
}

#pragma endregion

#pragma region 其它游戏状态处理函数

// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例
	if (currentStage != NULL)
		delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;

	switch (stageID)
	{
	case STAGE_START:
	{
		currentStage->bg = bmp_Bg;
		currentStage->pause = false;

		// 删除旧的按钮
		for (size_t i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			delete button;
		}
		buttons.clear();

		// 添加新的开始按钮
		Button* startButton = CreateButton(
			BUTTON_STARTGAME_ID, bmp_StartButton,
			BUTTON_STARTGAME_X, BUTTON_STARTGAME_Y,
			BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT);
		buttons.push_back(startButton);
	}
	break;
	case STAGE_GAME:
	{
		currentStage->bg = bmp_Bg;
		currentStage->pause = false;

		// 删除旧的按钮
		for (size_t i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			delete button;
		}
		buttons.clear();

		// 删除旧的砖块，创建新的砖块
		if (brick != NULL)
			delete brick;
		brick = CreateBrick(bmp_Green);

		//随机数种子
		srand(time(0));
	}
	break;

	// TODO：添加多个游戏场景
	default:
		break;
	}

	// 刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int x, int y, int width, int height)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->x = x;
	button->y = y;
	button->width = width;
	button->height = height;
	return button;
}

// 添加砖块函数
Brick* CreateBrick(HBITMAP img)
{
	Brick* brick = new Brick();
	brick->img = img;
	brick->frame_index = 0;
	brick->cell_x = BRICK_COLUMNS / 2;
	brick->cell_y = BRICK_ROWS / 2;
	return brick;
}

void CreateTextBox(wstring text, HDC hdc_memBuffer, RECT rect)
{
	// 创建一个字体，设置高度为 30（单位是逻辑单元）
	HFONT hFont = CreateFont(
		30,                       // 字体高度
		0,                        // 字体宽度（0 表示自动计算）
		0,                        // 字体的倾斜角度
		0,                        // 字体的基线倾斜角度
		FW_NORMAL,                // 字体的粗细
		FALSE,                    // 是否斜体
		FALSE,                    // 是否下划线
		FALSE,                    // 是否删除线
		DEFAULT_CHARSET,          // 字符集
		OUT_DEFAULT_PRECIS,       // 输出精度
		CLIP_DEFAULT_PRECIS,      // 剪裁精度
		DEFAULT_QUALITY,          // 输出质量
		DEFAULT_PITCH | FF_SWISS, // 字体家族和间距
		TEXT("微软雅黑")          // 字体名称
	);
	// 选择自定义字体到设备上下文
	HFONT hOldFont = (HFONT)SelectObject(hdc_memBuffer, hFont);
	// 设置背景颜色为黑色
	SetBkColor(hdc_memBuffer, RGB(0, 0, 0));
	// 设置文字颜色为白色，以确保在黑色背景上可见
	SetTextColor(hdc_memBuffer, RGB(255, 255, 255));

	// 绘制
	DrawText(hdc_memBuffer, text.c_str(), (int)text.length(), &rect, DT_CENTER);
	// 恢复原来的字体
	SelectObject(hdc_memBuffer, hOldFont);
	// 删除自定义字体以释放资源
	DeleteObject(hFont);
}

Block* CreateBlock(HBITMAP img, int type)
{
	int pos_y = HEAD_HEIGHT_Y;
	Block* b = nullptr;
	b = new Block();
	b->img = img;
	b->x = BRICK_COLUMNS / 2;
	b->y = pos_y;
	b->shape_id = 0;
	b->type = type;
	memcpy(b->shape, shapeSet[type][b->shape_id], sizeof(b->shape));

	return b;
}

void UpdateBlock(HWND hWnd) {
	if (block == nullptr) {
		int r = rand() % 7;
		block = CreateBlock(bmp_Green, r);
	}
	Cnt++;
	if (Cnt % time_space == 0)
	{
		block->y++;
	}
	if (Cnt >= 40) {
		Cnt = 0;
	}//DROP

	int command_type = 0;//1为向左，2为向右
	bool onLeftBorder = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block->shape[i][j]) {
				if (block->x + j <= 0) {
					onLeftBorder = 1;
				}
			}
		}
	}


	//方向锁解除
	if (keyLeftDown == 0 )
	{
		left_lock = 0;
	}
	else if (keyRightDown == 0 )
	{
		right_lock = 0;
	}
	
	
	//长按计时器启动
	if (left_lock == 1 && keyLeftDown == 1&&onLeftBorder == 0)
	{
		left_long_press++;
	}
	if (right_lock == 1 && keyRightDown == 1)
	{
		right_long_press++;
	}	
	if (down_lock == 1 && keyDownDown == 1)
	{
		down_long_press++;
	}
	
	
	// 位置更新
	if (keyDownDown&& recheck_times == 0)
	{
		time_space =10;
	}
	else if (keyDownDown==0)
	{
		time_space = 40;
	}
	

	if (left_lock == 0 && keyLeftDown && onLeftBorder == 0 || left_long_press > 5&& onLeftBorder == 0)
	{
		bool left_access = 1; left_long_press = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block->shape[i][j])
				{
					if (onLeftBorder == 0 && Data[block->x + j - 1][block->y + i] == 1)//左方有砖块
					{
						left_access = 0;
					}
				}
			}
		}
		if (left_access)
		{
			block->x--; command_type = 1;
			if (Cnt % time_space == 0)
			{
				block->y--;
			}			
		}
		left_lock = 1;

	}
	if (right_lock == 0 && keyRightDown || right_long_press > 5)
	{
		bool right_access = 1; right_long_press = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block->shape[i][j])
				{
					if (Data[block->x + j + 1][block->y + i] == 1)//右方有砖块
					{
						right_access = 0;
					}
				}
			}
		}
		if (right_access)
		{
			block->x++; command_type = 2;
			if (Cnt % time_space == 0)
			{
				block->y--;
			}
			right_lock = 1;
		}


	}
	// 防止超出屏幕
	/*
	if (block->x < 0)
	{
		block->x = 0;
	}*/
	if (block->x >= BRICK_COLUMNS)
	{
		block->x = BRICK_COLUMNS - 1;
	}
	if (block->y < 0)
	{
		block->y = 0;
	}


	

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block->shape[i][j]) {
				if (block->y + i >= BRICK_ROWS - 1)
				{
					flag = true;
					block->y = BRICK_ROWS - i - 2;//底部
				}
				if (block->x + j >= BRICK_COLUMNS - 1)
				{
					block->x = BRICK_COLUMNS - 2 - j;//右侧
				}
				/*
				if (block->x + j < 0)
				{
					onLeftBorder = 1;
					block->x++;
					//左侧
				}

				if (command_type==2&&Data[block->x + j][block->y + i ] == 1)//右方有砖块
				{
					block->x --;
				}
				if (command_type == 1 && onLeftBorder == 0&&Data[block->x + j ][block->y + i] == 1)//左方有砖块
				{
					block->x++;
				}
				*/
				if (Data[block->x + j][block->y + i + 1] == 1)//下方有砖块
				{
					flag = true;
				}
			}
		}
	}

	//Recheck

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block->shape[i][j])
			{

				while (Data[block->x + j][block->y + i] == 1)
				{
					if (command_type == 0)
					{
						block->y--;
					}
					else if (command_type == 1)
					{
						block->x++;
					}
					else if (command_type == 2)
					{
						block->x--;
					}
					recheck_times++;
					flag = 1;
					if (recheck_times >= 2)
					{
						break;
					}
				}
				if (recheck_times == 0 && block->y + i < BRICK_ROWS - 5 && Data[block->x + j][block->y + i + 1] == 0)
				{
					flag = 0;
				}
				else if (command_type != 0)
				{
					flag = 0;
				}
			}
		}
	}
	recheck_times = 0;

	//固化砖块    
	if (flag&&command_type==0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block->shape[i][j]) {
					Brick* b = CreateBrick(bmp_Green);
					b->cell_x = block->x + j;
					b->cell_y = block->y + i;
					Data[b->cell_x][b->cell_y] = 1;
					base.push_back(b);
				}
			}
		}
		flag=0;
		delete block;
		block = nullptr;
	}
	ClearRow();
	// block->y = BRICK_ROWS - 1;

	
}

void ClearRow()
{
	int r = -1;
	for (int row = 28; row >= 0; row--)
	{
		bool full = 1;
		for (int col = 0; col <= 18; col++)
		{
			if (Data[col][row] == 0)
			{
				full = 0; break;
			}
		}
		if (full)
		{
			r = row;
			break;
		}
	}
	if (r >= 0)
	{
		for (int row = r; row >= 1; row--)
		{
			for (int col = 0; col <= 18; col++)
			{
				Data[col][row] = Data[col][row - 1];
			}
		}
		for (int col = 0; col <= 18; col++)
		{
			Data[col][0] = 0;
		}
		PaintData();
	}
}

// 刷新砖块状态函数
void UpdateBrick(HWND hWnd)
{
	Cnt++;
	/*
	if (Cnt%10==0)
	{
		brick->cell_y++;
	}
	if (Cnt >= 20) {
		Cnt = 0;
	}

	// 位置更新
	if (keyUpDown)
	{
		brick->cell_y--;
	}
	if (keyDownDown)
	{
		brick->cell_y++;
	}
	if (keyLeftDown)
	{
		brick->cell_x--;
	}
	if (keyRightDown)
	{
		brick->cell_x++;
	}
	// 防止超出屏幕
	if (brick->cell_x < 0)
	{
		brick->cell_x = 0;
	}
	if (brick->cell_x >= BRICK_COLUMNS)
	{
		brick->cell_x = BRICK_COLUMNS - 1;
	}
	if (brick->cell_y < 0)
	{
		brick->cell_y = 0;
	}
	if (brick->cell_y >= BRICK_ROWS)
	{
		brick->cell_y = BRICK_ROWS - 1;
	}*/
}

#pragma endregion

#pragma region 绘图函数

// 绘图函数
void Paint(HWND hWnd)
{
	// 开始绘制
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	// 创建缓存
	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	// 初始化缓存
	HBITMAP blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 绘制背景到缓存
	SelectObject(hdc_loadBmp, currentStage->bg);
	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

	// 按场景分类绘制内容到缓存
	switch (currentStage->stageID)
	{
	case STAGE_START:
	{
		// 绘制开始界面提示文字       
		// 设置字体区域
		RECT rect = {
			BUTTON_STARTGAME_X,
			BUTTON_STARTGAME_Y + 200,
			BUTTON_STARTGAME_X + BUTTON_STARTGAME_WIDTH,
			BUTTON_STARTGAME_Y + 300 };
		// 绘制        
		CreateTextBox(TEXT("使用WASD控制方块移动"), hdc_memBuffer, rect);
	}
	break;
	case STAGE_GAME:
	{

		if (block != nullptr) {
			SelectObject(hdc_loadBmp, bmp_Green);
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block->shape[i][j] == 1) {
						TransparentBlt(
							hdc_memBuffer, (block->x + j) * BRICK_CELL_WIDTH, (block->y + i) * BRICK_CELL_HEIGHT,
							BRICK_CELL_WIDTH, BRICK_CELL_HEIGHT,
							hdc_loadBmp, 0, 0, 640, 640,
							RGB(255, 255, 255));
					}
				}
			}
		}
		for (int i = 0; i < base.size(); i++) {
			SelectObject(hdc_loadBmp, base[i]->img);
			Brick* brick = base[i];
			TransparentBlt(
				hdc_memBuffer, brick->cell_x * BRICK_CELL_WIDTH, brick->cell_y * BRICK_CELL_HEIGHT,
				BRICK_CELL_WIDTH, BRICK_CELL_HEIGHT,
				hdc_loadBmp, 0, 0, 640, 640,
				RGB(255, 255, 255));
		}
	}
	break;

	// TODO：添加多个游戏场景
	default:
		break;
	}

	// 绘制按钮到缓存
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		SelectObject(hdc_loadBmp, button->img);
		TransparentBlt(
			hdc_memBuffer, button->x, button->y,
			button->width, button->height,
			hdc_loadBmp, 0, 0, button->width, button->height,
			RGB(255, 255, 255));
	}

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteDC(hdc_loadBmp);
	DeleteDC(hdc_memBuffer);

	// 结束绘制
	EndPaint(hWnd, &ps);
}

// 创建纯白背景图像

HBITMAP CreateWhiteBackground(HWND hWnd)
{
	// 开始绘制
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	// 创建缓存
	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);

	// 初始化缓存
	HBITMAP bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, bmp_output);

	// 创建纯白色画刷
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

	// 创建一个矩形
	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	// 将缓存填充为白色
	FillRect(hdc_memBuffer, &rect, hBrush);

	// 回收资源所占的内存（非常重要）
	DeleteObject(hBrush);
	DeleteDC(hdc_memBuffer);

	// 结束绘制
	EndPaint(hWnd, &ps);

	return bmp_output;
}


HBITMAP CreateGameBackground(HWND hWnd)
{
	// 开始绘制
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	// 创建缓存
	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	// 初始化缓存
	HBITMAP bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, bmp_output);

	// 初始化为白色背景
	//SelectObject(hdc_loadBmp, bmp_WhiteBackground);
	//BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);


	// 画四周石头
	/*
	SelectObject(hdc_loadBmp, bmp_Green);
	for (int i = 0; i < BG_ROWS; i++)
	{
		BitBlt(hdc_memBuffer, 0, i * BRICK_CELL_HEIGHT, BRICK_CELL_WIDTH, BRICK_CELL_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
		BitBlt(hdc_memBuffer, (BG_COLUMNS - 1) * BRICK_CELL_WIDTH, i * BRICK_CELL_HEIGHT, BRICK_CELL_WIDTH, BRICK_CELL_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
	}
	for (int j = 0; j < BG_COLUMNS; j++)
	{
		BitBlt(hdc_memBuffer, j * BRICK_CELL_WIDTH, 0, BRICK_CELL_WIDTH, BRICK_CELL_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
		BitBlt(hdc_memBuffer, j * BRICK_CELL_WIDTH, (BG_ROWS - 1) * BRICK_CELL_HEIGHT, BRICK_CELL_WIDTH, BRICK_CELL_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
	}
	*/

	// 回收资源所占的内存（非常重要）
	DeleteDC(hdc_loadBmp);
	DeleteDC(hdc_memBuffer);

	// 结束绘制
	EndPaint(hWnd, &ps);

	return bmp_output;
}

void PaintData()
{
	base.clear();
	for (int row = 0; row <= 28; row++)
	{
		for (int col = 0; col <= 18; col++)
		{
			if (Data[col][row])
			{
				Brick* b = CreateBrick(bmp_Green);
				b->cell_x = col;
				b->cell_y = row;
				base.push_back(b);
			}
		}
	}
}

#pragma endregion