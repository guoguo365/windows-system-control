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

	// �򿪵�ǰ���̵�Ȩ������
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
		return false;
	}

	// ��ȡȨ�޵�Ȩ�ޱ�ʶLUID�� ������tkp��
	// ��1��������NULL ��ʾҪ�鿴���ǵ�ǰ����ϵͳ
	// ��2��������ָ����Ȩ����
	// ��3���������������շ�����Ȩ������Ϣ
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid)) {
		return false;
	}

	// ������Ȩ����ĸ���
	tkp.PrivilegeCount = 1;

	// ������Ȩ
	// SE_PRIVILEGE_ENABLE_BY_DEFAULT ��ȨĬ������
	// SE_PRIVILEGE_ENABLED			  ��Ȩ����
	// SE_PRIVILEGE_USER_FOR_ACCESS   ��������һ��������߷���
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// ����AdjustTokenPrivileges����Ȩ
	// �޸�����Ȩ��
	// ��1��������������Ȩ���
	// ��2����������ֹ����Ȩ��
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, NULL, NULL, NULL)) {
		return false;
	}
	return true;

}