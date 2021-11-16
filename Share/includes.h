#pragma once
#define WIN32_LEAN_AND_MEAN
#pragma comment (lib,"WS2_32.lib")
#pragma comment (lib, "MSWSock.LIB")
#include <WS2tcpip.h>
#include <MSWSock.h>

#include <typeinfo>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <map>
#include <array>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <span>
#include <concepts>
#include <type_traits>
#include <shared_mutex>
#undef max
#undef min
#include "../Share/types.h"
#include "../Share/Protocol.h"

#include "../Share/SocketUtil.h"

#include "../Share/SUNKUE.h"
#include "../Share/Spinlock.h"
