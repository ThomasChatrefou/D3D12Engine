#pragma once

#include "Windows.h"
#include "windowsX.h"

#include "assert.h"
#include <exception>

#include "d3d12.h"
#include "dxgi1_6.h"

#include "../resource.h"

#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)
#define ThrowIfFailed(hr) if (FAILED(hr)) throw std::exception();

