#pragma once
#include <windows.h>
#include <vector>


#define FORCEINLINE __forceinline

template <class T>
constexpr void RemoveFromVector(std::vector<T> v, T value)
{
	v.erase(std::remove(v.begin(), v.end(), value), v.end());
}

inline int SGE_GetKeyboardState(LPARAM inLParam, WPARAM inWParam, LPWSTR data)
{
	BYTE keyState[256];
	GetKeyboardState(keyState);
	// clear all of the modifier keys so ToUnicode will ignore them
	keyState[VK_CONTROL] = keyState[VK_SHIFT] = keyState[VK_MENU] = 0;
	keyState[VK_LCONTROL] = keyState[VK_LSHIFT] = keyState[VK_LMENU] = 0;
	keyState[VK_RCONTROL] = keyState[VK_RSHIFT] = keyState[VK_RMENU] = 0;
	// convert the WM_KEYDOWN/WM_KEYUP/WM_SYSKEYDOWN/WM_SYSKEYUP to characters
	UINT scanCode = (inLParam >> 16) & 0xFF;
	int inOutBufLenCharacters = 32;	
	int i = ToUnicode(inWParam, scanCode, keyState, data, inOutBufLenCharacters, 0);
	return i;
}