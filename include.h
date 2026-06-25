#ifndef include_h
#define include_h

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <dwmapi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <random>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <winhttp.h>
#include <wincodec.h>
#include <objidl.h>
#include <comdef.h>
#include <winternl.h>
#include <ntstatus.h>

#include <Windows.h>
#include <winternl.h>
#include <iostream>
#include <tlhelp32.h>
#include <cstdint>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <intrin.h>
#include <fstream>
#include <Psapi.h>
#include <functional>
#include <map>
#include <chrono>
#include <algorithm>
#include <cwctype>
#include <cstdio>


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "windowscodecs.lib")

#include <set>
#include <bit>
#include <iostream>
#include <string>
#include <cstdint>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cwctype>
#include <thread>
#include <mutex>
#include <chrono>
#include <cfloat>
#include <array>
#include <cmath>
#include <numeric>
#include <map>
#include <fstream>
#include <functional>
#include <limits>

#include "utilities/driver.h"
#include "utilities/debug.h"
#include "utilities/imgui/imgui.h"
#include "utilities/imgui/imgui_internal.h"
#include "utilities/imgui/imgui_impl_dx11.h"
#include "utilities/imgui/imgui_impl_win32.h"

#include "globals.h"
#include "flags.h"

#include "overlay/overlay.h"

#include "sdk/offset.h"
#include "sdk/sdk.h"

using namespace std;

#endif