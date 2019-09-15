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

#ifndef H_VIEWER_WIDGET
#define H_VIEWER_WIDGET

#include <QOpenGLWidget>
#include <QElapsedTimer>
#include <QTimer>

class Viewer;

/**
 * Proxy class binding the core viewer from Viewer.h to Qt's widget ecosystem.
 * It is named 'ViewerWidget' because it has kind of the same role toward
 * 'Viewer' than 'ListWidget' has toward 'ListView'.
 * It overrides from QOpenGLWidget to be able to override methods like paintGL
 * and replace them with calls to the underlying Viewer object (raw OpenGL).
 */
class ViewerWidget : public QOpenGLWidget
{
	Q_OBJECT;
public:
	explicit ViewerWidget(QWidget *parent = 0);
	~ViewerWidget();

protected:
	// Overrides of QOpenGLWidget that we forward to the Viewer instance
	// See individual method's documentation for details. Here is a summary:

	/**
	 * Called after the OpenGL context has been setup (internally by Qt).
	 * Here we can allocate and initialize the Viewer.
	 */
	void initializeGL() override;

	/**
	 * Called when the context is resized (meaning typically that the widget
	 * size changed).
	 * We update glViewport, and if you use some kind of camera you will need
	 * to update its projection matrix here, at least for the aspect ratio.
	 */
	void resizeGL(int w, int h) override;

	/**
	 * Called at each frame, when the widget's context is ready to be pointed
	 * on by some raw OpenGL calls.
	 */
	void paintGL() override;

private slots:
	/**
	 * Slot triggering the update() method of the widget
	 * The update method will itself call paintGL at some appropriate point
	 * NB: Widget::update() is not a slot so we cannot use it where we use triggerUpdate().
	 */
	inline void triggerUpdate() { update(); }

private:
	/**
	 * The core rendering behavior is actually handled by this Viewer instance.
	 * It is a dynamically allocated pointer so that Viewer's constructor can
	 * assume that OpenGL context has been loaded first.
	 */
	Viewer *m_viewerCore;

	/**
	 * Timer used for scheduling frame redraw the widget every 20 ms.
	 */
	QTimer m_renderTimer;

	/**
	 * Simple timer forwarded to the core viewer for animation timing.
	 */
	QElapsedTimer m_timer;
};

#endif // H_VIEWER_WIDGET
