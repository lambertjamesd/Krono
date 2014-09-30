#pragma once

#include <vector>
#include "Math/Vector2.h"
#include "Core/Types.h"

namespace krono
{

namespace KeyCode
{
	enum Type
	{
		_0 = 0x30,
		_1, _2, _3, _4, _5,
		_6, _7, _8, _9,

		A = 0x41,
		B, C, D, E, F, G, H, I, J,
		K, L, M, N, O, P, Q, R,
		S, T, U, V, W, X, Y, Z
	};
}

namespace MouseButton
{
	enum Type
	{
		LeftButton,
		RightButton,
		MiddleButton
	};
}

class InputState
{
public:
	InputState(void);
	~InputState(void);

	void SetMouseButon(MouseButton::Type button, bool value);
	bool GetMouseButton(MouseButton::Type button) const;
	bool GetMouseButtonDown(MouseButton::Type button) const;
	bool GetMouseButtonUp(MouseButton::Type button) const;

	void SetKey(KeyCode::Type key, bool value);
	bool GetKey(KeyCode::Type key) const;
	bool GetKeyDown(KeyCode::Type key) const;
	bool GetKeyUp(KeyCode::Type key) const;

	void SetMousePosition(const Vector2i& value);
	const Vector2i GetMousePosition() const;
	Vector2i GetMouseMove() const;

	void SetScreenSize(const Vector2i& value);
	const Vector2i& GetScreenSize() const;

	void SetPreviousState(const InputState* previousState);
private:
	std::vector<bool> mMouseButton;
	std::vector<bool> mKeyState;
	Vector2i mMousePosition;
	Vector2i mScreenSize;

	const InputState* mPreviousState;
};

}
