#pragma once

#include "Windows.h"
#include "windowsX.h"

#include "assert.h"
#include <exception>
#include <format>

#include "d3d12.h"
#include "dxgi1_6.h"

#include "../resource.h"
#include "DirectXMath.h"

#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)
#define ThrowIfFailed(hr) if (FAILED(hr)) throw std::exception();

