/**
 * This file is part of Augen Light
 *
 * Copyright (c) 2017 - 2019 -- Élie Michel <elie.michel@exppad.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and non-infringement. In no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising
 * from, out of or in connection with the software or the use or other dealings
 * in the Software.
 */

#ifndef H_VIEWER
#define H_VIEWER

#include <glad/glad.h> // must be the first include

/**
 * Viewer code is identical to non-qt branches. This part of the code is and
 * must remain independent from Qt. It uses plain OpenGL to draw.
 * Its attributes are what was globals in level0 branch.
 */
class Viewer
{
public:
	Viewer();
	~Viewer();

	void update(double time);
	void render();
	void resize(int width, int height);

private:
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_program;
};

#endif // H_VIEWER
