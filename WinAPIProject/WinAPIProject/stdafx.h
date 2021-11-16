// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>


// Library
#pragma comment(lib, "msimg32")
#pragma comment(lib, "ws2_32")

// Header
#include <assert.h>
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <string>
#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <iostream>

#include <commdlg.h>

using std::vector;
using std::list;
using std::map;
using std::string;
using std::wstring;

using std::make_pair;


#include "define.h"
#include "struct.h"
#include "func.h"



