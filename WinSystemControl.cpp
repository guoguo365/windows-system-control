#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

bool improvePrivilege();
bool powerOffProc();
bool logOffProc();
bool reBootProc();

int main() {
	char ch;
	printf("***************************************\n*\n");
	printf("* 1. Power Off\n*\n");
	printf("* 2. Log Off\n*\n");
	printf("* 3. ReBoot\n*\n");
	printf("***************************************\n*");
	ch = getchar();
	while (true) {
		switch (ch)
		{
		case '1':
			if (!powerOffProc()) {
				printf("Process Error!!!\n");
				continue;
			}
			return 0;
		case '2':
			if (!logOffProc()) {
				printf("Process Error!!!\n");
				continue;
			}
			return 0;
		case '3':
			if (!reBootProc()) {
				printf("Process Error!!!\n");
				continue;
			}
			return 0;
		default:
			printf("Please input 1-3");
			break;
		}
	}

	system("pause");
	return 0;
}

bool powerOffProc() {
	if (!improvePrivilege() || !ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION)) {
		return false;
	}
	return true;
}

bool logOffProc() {
	if (!improvePrivilege() || !ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION)) {
		return false;
	}
	return true;
}

bool reBootProc() {
	if (!improvePrivilege() || !ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_APPLICATION)) {
		return false;
	}
	return true;
}

bool improvePrivilege() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// 打开当前进程的权限令牌
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
		return false;
	}

	// 获取权限的权限标识LUID， 保存在tkp中
	// 第1个参数：NULL 表示要查看的是当前本地系统
	// 第2个参数：指定特权名称
	// 第3个参数：用来接收返回特权名称信息
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
		return false;
	}

	// 设置特权数组的个数
	tkp.PrivilegeCount = 1;

	// 启动特权
	// SE_PRIVILEGE_ENABLE_BY_DEFAULT 特权默认启动
	// SE_PRIVILEGE_ENABLED			  特权启动
	// SE_PRIVILEGE_USER_FOR_ACCESS   用来访问一个对象或者服务
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// 调用AdjustTokenPrivileges来提权
	// 修改令牌权限
	// 第1个参数：调整特权句柄
	// 第2个参数：禁止所有权限
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, NULL, NULL, NULL)) {
		return false;
	}
	return true;

}