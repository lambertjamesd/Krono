#include "InputState.h"

namespace krono
{

InputState::InputState(void) :
	mPreviousState(NULL)
{
}


InputState::~InputState(void)
{
}

void InputState::SetMouseButon(MouseButton::Type button, bool value)
{
	if (mMouseButton.size() <= (size_t)button)
	{
		mMouseButton.resize(button + 1);
	}

	mMouseButton[button] = value;
}

bool InputState::GetMouseButton(MouseButton::Type button) const
{
	if ((size_t)button < mMouseButton.size())
	{
		return mMouseButton[button];
	}
	else
	{
		return false;
	}
}

bool InputState::GetMouseButtonDown(MouseButton::Type button) const
{
	if (mPreviousState == NULL)
	{
		return false;
	}
	else
	{
		return GetMouseButton(button) && !mPreviousState->GetMouseButton(button);
	}
}

bool InputState::GetMouseButtonUp(MouseButton::Type button) const
{
	if (mPreviousState == NULL)
	{
		return false;
	}
	else
	{
		return !GetMouseButton(button) && mPreviousState->GetMouseButton(button);
	}
}

void InputState::SetKey(KeyCode::Type key, bool value)
{
	if (mKeyState.size() <= (size_t)key)
	{
		mKeyState.resize(key + 1);
	}

	mKeyState[key] = value;
}

bool InputState::GetKey(KeyCode::Type key) const
{
	if ((size_t)key < mKeyState.size())
	{
		return mKeyState[key];
	}
	else
	{
		return false;
	}
}

bool InputState::GetKeyDown(KeyCode::Type key) const
{
	if (mPreviousState == NULL)
	{
		return false;
	}
	else
	{
		return GetKeyUp(key) && !mPreviousState->GetKeyUp(key);
	}
}

bool InputState::GetKeyUp(KeyCode::Type key) const
{
	if (mPreviousState == NULL)
	{
		return false;
	}
	else
	{
		return !GetKeyUp(key) && mPreviousState->GetKeyUp(key);
	}
}

void InputState::SetMousePosition(const Vector2i& value)
{
	mMousePosition = value;
}

const Vector2i InputState::GetMousePosition() const
{
	return mMousePosition;
}

Vector2i InputState::GetMouseMove() const
{
	if (mPreviousState == NULL)
	{
		return Vector2i();
	}
	else
	{
		return GetMousePosition() - mPreviousState->GetMousePosition();
	}
}

void InputState::SetScreenSize(const Vector2i& value)
{
	mScreenSize = value;
}

const Vector2i& InputState::GetScreenSize() const
{
	return mScreenSize;
}

void InputState::SetPreviousState(const InputState* previousState)
{
	mPreviousState = previousState;
}

}