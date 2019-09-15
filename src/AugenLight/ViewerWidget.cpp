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

#include <glad/glad.h> // must be the first include, and for gladLoadGL()

#include "ViewerWidget.h"
#include "Viewer.h"

ViewerWidget::ViewerWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, m_viewerCore(nullptr)
{
	// This somehow sets the FPS: render a new frame every 20ms, so at 50 FPS
	connect(&m_renderTimer, &QTimer::timeout, this, &ViewerWidget::triggerUpdate);
	m_renderTimer.start(20);

	m_timer.start();

	// Inform Qt backend what kind of OpenGL context to allocate for this
	// widget. In level0, this is hidden in utils/windows.h. It is important
	// that this is done before the context is created, so before the widget is
	// shown. In particular, this could not go in initializeGL() which is
	// called right *after* the context has been created.
	// We ask here for a pure ("core") OpenGL 4.5 context.
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(4,5);
#ifndef NDEBUG
	// Without this, setting an OpenGL debug callback with
	// glDebugMessageCallback would have no effect (see utils/debug.cpp)
	format.setOption(QSurfaceFormat::DebugContext);
#endif // !NDEBUG
	setFormat(format);
}

ViewerWidget::~ViewerWidget()
{
	if (m_viewerCore) {
		delete m_viewerCore;
	}
}

void ViewerWidget::initializeGL()
{
	if (gladLoadGL()) {
		m_viewerCore = new Viewer;
	}
}

void ViewerWidget::resizeGL(int w, int h)
{
	if (!m_viewerCore) {
		return;
	}

	m_viewerCore->resize(w, h);
}

void ViewerWidget::paintGL()
{
	if (!m_viewerCore) {
		return;
	}

	m_viewerCore->update(m_timer.elapsed() / 1000.0f);
	m_viewerCore->render();
}
