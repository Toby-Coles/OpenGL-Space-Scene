#pragma once

class HelloGL;

namespace GLUTCallbacks
{

	void Init(HelloGL* gl);
	void Timer(int preferredRefresh);
	void keyboard(unsigned char key, int x, int y);

	void Display();
}
