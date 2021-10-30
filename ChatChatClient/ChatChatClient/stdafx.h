#pragma once

#pragma comment(lib,"./Dependencies/OPGL/x64GL/glew32.lib")
#pragma comment(lib,"./Dependencies/OPGL/x64GL/glew32s.lib")
#pragma comment(lib,"./Dependencies/OPGL/x64GL/freeglut.lib")


#define WIN32_LEAN_AND_MEAN


#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <cassert>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <chrono>
#include <array>
#include <queue>
#include <memory>
#include <list>

using namespace std;
using namespace chrono;
using namespace std::literals::string_view_literals;
using clk = std::chrono::high_resolution_clock;

#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 26812)


#include "Dependencies/OPGL/glew.h"
#include "Dependencies/OPGL/wglew.h"
#include "Dependencies/OPGL/freeglut.h"


#include "Dependencies/OPGL/glm/glm.hpp"
#include "Dependencies/OPGL/glm/gtx/projection.hpp"
#include "Dependencies/OPGL/glm/gtx/transform.hpp"
#include "Dependencies/OPGL/glm/gtx/quaternion.hpp"
#include "Dependencies/OPGL/glm/gtx/matrix_decompose.hpp"
#include "Dependencies/OPGL/glm/ext.hpp"

#include "stb_image.h"
#include "texture.h"
#pragma warning(pop)

#include "SUNKUE.hpp"
#include "defines.h"
#include "game_system.h"

