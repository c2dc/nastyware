#pragma comment(lib, "fltlib.lib")

#include<Windows.h>
#include<stdio.h>
#include<fltuser.h>
#include <psapi.h>

// To exec function
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <algorithm>
#include <iterator>
using std::cout; using std::endl;
using std::string; using std::reverse;

// To convert string to int
#include <iostream>
#include <string>
#include <sstream>

// from docs.microsoft.com
#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>


// Convert char* to TCHAR
#include <iostream>
#include <stdlib.h>
#include <string>
#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"
using namespace std;
//using namespace System;


#define AV_SCAN_PORT_NAME L"\\mf"
#define FINISHED 0
#define FOREVER 1
#define MAX_STR 8192
#define SUCCESS(X) X==S_OK
#define USUAL_VOLUME "C:\\"
#define USUAL_VOLUME_OFFSET 24
#define YARA_WHITELIST(X) strstr(X,YARA_BINARY_NAME)==NULL
#define YARA_BINARY_NAME "C:\\yara_rules\\yara64.exe"
#define YARA_RULES_PATH "C:\\yara_rules\\Ransomware.yar"
#define PIPE L"\\pipe\temp"


/* I/O communication struct */
typedef struct _AV_MESSAGE {
	FILTER_MESSAGE_HEADER p;	// Windows Headers
	UINT64 pid;					// Data itself
} AVmsg, *PAVmsg;


void GetProcessPath (UINT64 pid, char* path)
{
	char name[MAX_STR];
	HANDLE p = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	GetProcessImageFileNameA(p, name, MAX_STR);
	/* Adjust Volume (ugly way)*/
	strcpy_s(path, MAX_STR, USUAL_VOLUME);
	strcat_s(path, MAX_STR, name + USUAL_VOLUME_OFFSET);
	CloseHandle(p);
}

DWORD WINAPI CallYARA (LPVOID lpParameter)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	/* Create Command Line */
	char cmd[MAX_STR];
	sprintf_s(cmd, MAX_STR, "%s -c -w %s %s", YARA_BINARY_NAME, YARA_RULES_PATH, (char*)lpParameter);

	printf("[NastywareConsoleApp] CallYARA cmd: %s\n", cmd);
	if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		printf("[NastywareConsoleApp] CallYARA CreateProcess failed: %d\n", GetLastError());
		return FINISHED;
	}

	printf("[NastywareConsoleApp] CallYARA pi.dwProcessId: %i e pi.hProcess: %x\n", pi.dwProcessId, pi.hProcess);
	printf("[NastywareConsoleApp] CallYARA pi.dwThreadId: %i e pi.hThread: %x\n", pi.dwThreadId, pi.hThread);

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// its over
	return FINISHED;
}


DWORD WINAPI CallYARA2(LPVOID lpParameter)
{
	int check;
	std::stringstream ss;
	/* Create Command Line */
	char cmd[MAX_STR];
	sprintf_s(cmd, MAX_STR, "%s -c -w %s %s", YARA_BINARY_NAME, YARA_RULES_PATH, (char*)lpParameter);
	printf("[CallYARA2] cmd: %s\n", cmd);

	std::array<char, 128> buffer;
	std::string result;

	printf("[CallYARA2] Antes do _popen.\n");
	
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);

	if (!pipe) {
		throw std::runtime_error("_popen() failed.!");
	}

	printf("[CallYARA2] Depois do _popen.\n");
	
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
			result += buffer.data();
		}
	}

	printf("[CallYARA2] Depois do while.\n");

	ss << result;
	ss >> check;

	if (check) {
		printf("[CallYARA2] %s is Ransomware! Result: %i\n", (char*)lpParameter, check);
		Sleep(5000);
		return 0;
	}
	else {
		printf("[CallYARA2] %s is Goodware... Result: %i\n", (char*)lpParameter, check);
		Sleep(5000);
		return 1;
	}
}


int main()
{
	char path[MAX_STR];
	HANDLE hport;
	AVmsg msg;

	/* Starting */
	printf("[NastywareConsoleApp] Starting..\n");
	/* Connect to I/O port */
	HRESULT h = FilterConnectCommunicationPort(AV_SCAN_PORT_NAME, 0, NULL, 0, NULL, &hport);
	if (!SUCCESS(h))
	{
		printf("[NastywareConsoleApp] Connection Fail\n");
		exit(FINISHED);
	}
	printf("[NastywareConsoleApp] Connected\n");
	while (FOREVER) {
		h = FilterGetMessage(hport, &msg.p, sizeof(msg), NULL);
		if (!SUCCESS(h))
		{
			printf("[NastywareConsoleApp] GetMsg Fail: %x\n", h);
			exit(FINISHED);
		}
		GetProcessPath(msg.pid, path);
		/* Avoid Recursive Calls */
		if (YARA_WHITELIST(path))
		{
			printf("[NastywareConsoleApp] Created: %s\n", path);
			/* Avoid Blocking */
			CreateThread(NULL, 0, CallYARA, path, 0, NULL);
		}
	}
	return FINISHED;
}