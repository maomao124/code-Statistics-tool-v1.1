#include<iostream>
#include<stdio.h>
#include<string.h>
#include <Windows.h>
#include<time.h>
#include <graphics.h>
#include <conio.h>
#include<algorithm>
#include<stdlib.h>
#include <random>
#include <string>
#include <fstream>
#include <ddraw.h>
#include<iomanip>
#include <Psapi.h>
#include<vector>
#include<io.h>
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>
#include <sapi.h>                                             // 语音头文件
#include <sphelper.h>                                     // 语音识别头文件
#include <thread>                                           //多线程
#include"resource1.h"
//vs2019自定义控制台代码模板 by mao
//作者QQ1296193245
using namespace std;
#pragma warning(disable : 4996)
#pragma comment (lib,"ddraw.lib")
#pragma comment(lib,"sapi.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"Winmm.lib")                     // 引用 Windows Multimedia API
#pragma intrinsic(__rdtsc)
unsigned __int64 t1, t2, t3, t4;
double run_time;                                                   //微秒级算法时间计时器
LARGE_INTEGER time_start;	                                //开始时间
LARGE_INTEGER time_over;	                                //结束时间
double dqFreq;		                                               //计时器频率
LARGE_INTEGER f;	                                               //计时器频率
double tt;
long double totaltime;

inline void runstart()
{
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	t1 = __rdtsc();
	QueryPerformanceCounter(&time_start);	       //计时开始
}
inline void runend()
{
	QueryPerformanceCounter(&time_over);	        //计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	t2 = __rdtsc();
	t3 = t2 - t1;
}
void ShowProcessMemoryUsageInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
	float memoryUsage_K = pmc.WorkingSetSize / 1024.0;
	std::cout << std::fixed << setprecision(0) << "程序内存使用率：" << memoryUsage_K << " KB = " << setprecision(2) << memoryUsage_M << " MB" << std::endl;
}
__int64 CompareFileTime(FILETIME time1, FILETIME time2)//转换为整数时间
{
	__int64 a = static_cast<long long>(time1.dwHighDateTime) << 32 | time1.dwLowDateTime;
	__int64 b = static_cast<long long>(time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
	return   (b - a);
}
void rundisplay()
{
	cout << endl;
	cout << "-------------------------------------------------------" << endl;
	if (t3 < 10000)
	{
		cout << "CPU脉冲数：" << (t3) << endl;
	}
	else if (t3 < 100000000 && t3 >= 10000)
	{
		tt = t3;
		cout << "CPU脉冲数：" << (tt) / 10000 << "万" << endl;
	}
	else if (t3 > 100000000000)
	{
		t4 = t3;
		t4 = t3 / 10000000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10 << "亿" << endl;
	}
	else
	{
		t4 = t3;
		t4 = t3 / 10000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10000 << "亿" << endl;
	}
	if (run_time > 10000)
	{
		cout << "算法运行时间：" << run_time / 1000 << "秒" << endl;
	}
	else if (run_time < 1)
	{
		cout << "算法运行时间：" << run_time * 1000 << "微秒" << endl;
	}
	else
	{
		cout << "算法运行时间：" << run_time << "毫秒" << endl;
	}
	totaltime = clock();
	cout << "程序运行时间：" << totaltime / 1000 << "秒" << endl;
	if (run_time > 0.4)
	{
		double frequency = t3 / run_time / 1000;
		cout << "CPU频率：" << frequency << "MHZ" << endl;
	}
	ShowProcessMemoryUsageInfo();
	long lRet;
	HKEY hKey;
	TCHAR tchData[64];
	DWORD dwSize;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)tchData, &dwSize);
		if (lRet == ERROR_SUCCESS)
		{
			cout << "CPU 名字：";
			wcout << tchData << endl;
		}
		else
		{
			cout << "CPU 名字：";
			wcout << L"未知" << endl;
		}
	}
	else
	{
		cout << "CPU 名字：";
		wcout << L"未知" << endl;
	}
	RegCloseKey(hKey);
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	double totalphy = statex.ullTotalPhys / 1024 / 1024;
	totalphy = totalphy / 1024;
	cout << "总物理内存：    " << setprecision(4) << left << setw(7) << totalphy << " GB" << endl;
	double totalvir = statex.ullTotalPageFile / 1024 / 1024;
	totalvir = totalvir / 1024;
	cout << "总虚拟内存：    " << setw(7) << totalvir - totalphy << " GB" << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 }; //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		//printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	//   光标位置   
	COORD     cursorPos;
	//   标准输出句柄   
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
	double freephy = statex.ullAvailPhys / 1024 / 1024;
	freephy = freephy / 1024;
	double usephy = (totalphy - freephy) * 100.0 / totalphy;
	cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
	double freetotal = statex.ullAvailPageFile / 1024 / 1024;
	freetotal = freetotal / 1024;
	cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
	double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
	cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
	double usevir = freevir * 100 / (totalvir - totalphy);
	cout << "虚拟内存使用率：";
	printf("%.2f%%", usevir);
	cout << endl;
	cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << 0.00 << "%" << endl;
	cout << "CPU空闲率：    " << setw(6) << 100.0 << "%" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "按任意键退出程序" << endl;
	while (!_kbhit())
	{
		GlobalMemoryStatusEx(&statex);
		WaitForSingleObject(hEvent, 1000);
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		int idle = CompareFileTime(preidleTime, idleTime);
		int kernel = CompareFileTime(prekernelTime, kernelTime);
		int user = CompareFileTime(preuserTime, userTime);
		float cpu = (kernel + user - idle) * 100.0 / (kernel + user);
		float cpuidle = (idle) * 100.0 / (kernel + user);
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 0;
		SetConsoleCursorPosition(hStdout, cursorPos);
		//cout << "内存使用率：" << statex.dwMemoryLoad << "%" << endl;
		double freephy = statex.ullAvailPhys / 1024 / 1024;
		freephy = freephy / 1024;
		double usephy = (totalphy - freephy) * 100.0 / totalphy;
		cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 1;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freetotal = statex.ullAvailPageFile / 1024 / 1024;
		freetotal = freetotal / 1024;
		cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 2;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
		cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 3;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double usevir = freevir * 100 / (totalvir - totalphy);
		cout << "虚拟内存使用率：";
		printf("%.2f%%", usevir);
		cout << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 4;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << cpu << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 5;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU空闲率：    " << setw(6) << cpuidle << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 6;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "-------------------------------------------------------" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 7;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "按任意键退出程序" << endl;
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
	cout << endl;

}


//**************************************************************//************************非主函数

#include <direct.h>
void getFiles(string path, string path2, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p, p2;
	if ((hFile = _findfirst(p.assign(path).append(path2).append("\\*").c_str(), &fileinfo)) != -1)// _findfirst(p.assign(path).append(path2).append("\\*").c_str(), &fileinfo)) != -1
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getFiles(p.assign(path).append("\\"), p2.assign(fileinfo.name).append("\\"), files);
				}
			}
			else
			{
				files.push_back(p.assign(path2).append(fileinfo.name));  //这一行可以给出相对路径
				//files.push_back(p.assign(fileinfo.name) );  //这一行可以给出文件名 
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


void getFilesAll(string path, vector<string>& files)
{
	//文件句柄
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					getFilesAll(p.assign(path).append("\\\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void getJustCurrentFile(string path, vector<string>& files)
{
	//文件句柄 
	long  hFile = 0;
	//文件信息 
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				;
			}
			else
			{
				files.push_back(fileinfo.name);
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name) ); 
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

string solve(string& s1, string& s2)
{
	if (s2.size() == 0)
		return s1;
	for (int i = 0;i < (int)(s1.size() - s2.size() + 1);)
	{
		if (s1.substr(i, s2.size()) == s2)
		{
			s1.erase(i, s2.size());
		}
		else
			i++;
	}
	return s1;
}


BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttributes);
}

#define FOREGROUND_BLUE      0x0001 // text color contains blue.文本颜色包含蓝色。
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.文本颜色增强。
#define BACKGROUND_BLUE      0x0010 // background color contains blue.//背景色包含蓝色。
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#define COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
#define COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
#define COMMON_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
#define COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
#define COMMON_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
#define COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
#define COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.
#define COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.

string workpath;

//**************************************************************//************************

int main()
{

	//*************************************************************************

	bool cpp;
	bool h;
	bool java;
	bool py;
	bool cs;
	bool cc;
	char choose;
	cout << "                                  欢迎使用!!!" << endl;
	cout << "                      软件支持c++、java、Python、c#和c语言" << endl;
	cout << endl;
	char   buffer[MAX_PATH];
	ifstream pathin("path.ini", ios::in);
	if (!pathin)
	{
		getcwd(buffer, MAX_PATH);
		cout << "当前工作目录：" << buffer << endl;
		workpath = buffer;
	}
	else
	{
		string s;
		getline(pathin, s);
		strcpy(buffer, s.c_str());
		cout << "当前工作目录：" << buffer << endl;
		workpath = buffer;
	}
	pathin.close();
	cout << endl;
	SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "是否统计c++的数据？            1.是          2.否" << endl;
	choose = _getch();
	if (choose == '1')
	{
		cpp = 1;
	}
	else if (choose == '2')
	{
		cpp = 0;
	}
	else
	{
		cpp = 0;
		cout << "\a";
	}
	cout << "是否统计c++头文件(.h)的数据？  1.是          2.否" << endl;
	choose = _getch();
	if (choose == '1')
	{
		h = 1;
	}
	else if (choose == '2')
	{
		h = 0;
	}
	else
	{
		h = 0;
		cout << "\a";
	}
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "是否统计java的数据？           1.是          2.否" << endl;
	choose = _getch();
	if (choose == '1')
	{
		java = 1;
	}
	else if (choose == '2')
	{
		java = 0;
	}
	else
	{
		java = 0;
		cout << "\a";
	}
	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "是否统计python的数据？         1.是          2.否" << endl;
	choose = _getch();
	if (choose == '1')
	{
		py = 1;
	}
	else if (choose == '2')
	{
		py = 0;
	}
	else
	{
		py = 0;
		cout << "\a";
	}
	SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "是否统计c#的数据？             1.是          2.否" << endl;
	choose = _getch();
	if (choose == '1')
	{
		cs = 1;
	}
	else if (choose == '2')
	{
		cs = 0;
	}
	else
	{
		cs = 0;
		cout << "\a";
	}
	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "是否统计c语言的数据？          1.是          2.否" << endl;
	choose = _getch();
	if (choose == '1')
	{
		cc = 1;
	}
	else if (choose == '2')
	{
		cc = 0;
	}
	else
	{
		cc = 0;
		cout << "\a";
	}
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合

	//*************************************************************************
	runstart();
	//*************************计时开始***************************************

	
	
	cout << "遍历文件目录中，请等待......." << endl;
	vector<string> files;
	string filePath;
	filePath.assign(buffer).append("\\");
	////获取该路径下的所有文件  
	getFilesAll(filePath, files);
	int size = files.size();
	int s = 0;
	filePath.append("\\\\");

	for (int i = 0;i < size;i++)
	{
		solve(files[i], filePath);
		if ((cpp == 1) && files[i].find(".cpp") < files[i].length() && files[i].find(".cpp") >= files[i].length() - 4)
		{
			s++;
			SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);    //绿
			cout << files[i].c_str() << endl;
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合
			continue;
		}
		else if ((h == 1) && files[i].find(".h") < files[i].length() && files[i].find(".h") >= files[i].length() - 2)
		{
			s++;
			SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);    //绿
			cout << files[i].c_str() << endl;
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合
			continue;
		}
		else if ((java == 1) && files[i].find(".java") < files[i].length() && files[i].find(".java") >= files[i].length() - 5)
		{
			s++;
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << files[i].c_str() << endl;
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合
			continue;
		}
		else if ((py == 1) && files[i].find(".py") < files[i].length() && files[i].find(".py") >= files[i].length() - 3)
		{
			s++;
			SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << files[i].c_str() << endl;
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合
			continue;
		}
		else if ((cs == 1) && files[i].find(".cs") < files[i].length() && files[i].find(".cs") >= files[i].length() - 3)
		{
			s++;
			SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			cout << files[i].c_str() << endl;
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合
			continue;
		}
		else if ((cc == 1) && files[i].find(".c") < files[i].length() && files[i].find(".c") >= files[i].length() - 2)
		{
			s++;
			//cout << "\033[m";
			SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << files[i].c_str() << endl;
			//cout << "\033[m";
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);  //默认情况下控制台颜色是白色，所以用“RGB”组合
			continue;
		}
	}
	cout << "正在把所有路径写入文件中......." << endl;
	cout << "在此期间请不要关闭程序，以免文件损坏" << endl;
	ofstream out("Data.txt", ios::out);
	out << s << endl;
	for (int i = 0;i < size;i++)
	{
		solve(files[i], filePath);
		if ((cpp == 1) && files[i].find(".cpp") < files[i].length() && files[i].find(".cpp") >= files[i].length() - 4)
		{
			//s++;
			out << files[i].c_str() << endl;
			continue;
		}
		else if ((h == 1) && files[i].find(".h") < files[i].length() && files[i].find(".h") >= files[i].length() - 2)
		{
			//s++;
			out << files[i].c_str() << endl;
			continue;
		}
		else if ((java == 1) && files[i].find(".java") < files[i].length() && files[i].find(".java") >= files[i].length() - 5)
		{
			//s++;
			out << files[i].c_str() << endl;
			continue;
		}
		else if ((py == 1) && files[i].find(".py") < files[i].length() && files[i].find(".py") >= files[i].length() - 3)
		{
			//s++;
			out << files[i].c_str() << endl;
			continue;
		}
		else if ((cs == 1) && files[i].find(".cs") < files[i].length() && files[i].find(".cs") >= files[i].length() - 3)
		{
			//s++;
			out << files[i].c_str() << endl;
			continue;
		}
		else if ((cc == 1) && files[i].find(".c") < files[i].length() && files[i].find(".c") >= files[i].length() - 2)
		{
			//s++;
			out << files[i].c_str() << endl;
			continue;
		}
	}
	out.close();
	cout << "写入完成" << endl;
	cout << "一共" << s << "个文件" << endl;


	string* paths = new string[s];
	string throwpath1;
	ifstream in("Data.txt", ios::in);
	getline(in, throwpath1);
	for (int i = 0; i < s; i++)
	{
		getline(in, paths[i]);
	}
	in.close();
	char c;
	long long strtotal = 0;
	long long str13 = 0;
	long long strspace = 0;
	ifstream test("out.txt", ios::in);
	if (!test)
	{

	}
	else
	{
		cout << "out.txt已经存在，已自动清除" << endl;
		ofstream test2("out.txt", ios::out);
		test2.close();
	}
	test.close();
	ofstream fileout("out.txt", ios::app);
	cout << "正在统计....." << endl;
	for (int i = 0; i < s; i++)
	{
		cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << "进度：" << i << "/" << s;
		paths[i] = workpath + "\\\\" + paths[i];
		ifstream file(paths[i], ios::in);
		fileout << paths[i] << endl;
		while (!file.eof())
		{
			c = file.get();
			//cout << c;
			fileout << c;
			if (c == '\n')
			{
				str13++;
			}
			else if (c == ' ')
			{
				strspace++;
			}
			else
			{
				strtotal++;
			}
		}
		fileout << endl;
		file.close();
	}
	cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << "进度：" << s << "/" << s << endl;
	fileout.close();
#define jianju 30
	cout << "结果：" << endl;
	cout << left << setw(jianju) << "字符总数：" << strtotal + strspace + str13 << endl;
	cout << setw(jianju) << "有效字符总数：" << strtotal << endl;
	cout << setw(jianju) << "空格数：" << strspace << endl;
	cout << setw(jianju) << "行数：" << str13 << right << endl;
	ofstream finalout("结果.txt", ios::app);
	SYSTEMTIME sys;                                    //获取系统时间
	GetLocalTime(&sys);
	finalout << endl;
	finalout << "日期：";
	finalout << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "      ";
	finalout << "时间：";
	finalout << setfill('0') << setw(2) << sys.wHour << ":" << setfill('0') << setw(2) << sys.wMinute << ":" << setfill('0') << setw(2) << sys.wSecond << endl;
	finalout << "路径：" << workpath << endl;
	if (cpp == 1)
	{
		finalout << "c++：           是" << endl;
	}
	else
	{
		finalout << "c++：           否" << endl;
	}
	if (h == 1)
	{
		finalout << "c++头文件： 是" << endl;
	}
	else
	{
		finalout << "c++头文件： 否" << endl;
	}
	if (java == 1)
	{
		finalout << "java：           是" << endl;
	}
	else
	{
		finalout << "java：           否" << endl;
	}
	if (py == 1)
	{
		finalout << "Python：      是" << endl;
	}
	else
	{
		finalout << "Python：      否" << endl;
	}
	if (cs == 1)
	{
		finalout << "C#：             是" << endl;
	}
	else
	{
		finalout << "C#：             否" << endl;
	}
	if (cc == 1)
	{
		finalout << "c语言：         是" << endl;
	}
	else
	{
		finalout << "c语言：         否" << endl;
	}
	finalout << setfill('-') << left << setw(jianju) << "文件数量：" << s << endl;
	finalout << setw(jianju) << "字符总数：" << strtotal + strspace + str13 << endl;
	finalout << setw(jianju) << "有效字符总数：" << strtotal << endl;
	finalout << setw(jianju) << "空格数：" << strspace << endl;
	finalout << setw(jianju) << "行数：" << str13 << endl;
	finalout << endl;
	finalout.close();
	cout << "最终结果保存在相对路径下" << endl;
	delete[]paths;


	//*************************计时结束***************************************
	runend();

	system("pause");





	rundisplay();                                                                   //计时结果显示
	_getch();
	//closegraph();
	// system("pause");
	//Sleep(10000);
	return 0;
}
