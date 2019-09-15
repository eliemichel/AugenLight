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

#include <QApplication>

#include "MainWindow.h"

/**
 * This is a minimal setup creating a Qt main window with widget that is drawn
 * in raw OpenGL using glad. It is a port of AugenLight's level0 branch to Qt
 * (while the original relies on glfw).
 *
 * In case you are not aware of it, let me recall that this file is part of a
 * project of tiny but intensively commented code for getting started with
 * OpenGL. The project is called AugenLight and available on GitHub. It is
 * split in different levels, that you can follow in order, each build on the
 * previous one. If you havn't done yet, I invite you to read the level0 branch
 * on which this Qt-based fork is based:
 * https://github.com/eliemichel/AugenLight/blob/level0/src/AugenLight/main.cpp
 *
 * A version based on Qt is forcely slightly more complicated than the minimal
 * level0 branch, and assumes that you know a bit about Qt. I mean, you can
 * totally understand this whole code even though you are not familiar with Qt,
 * but to build further on this you'd better read some tutorial about Qt's own
 * logic and details. Anyways, if you're here reading this rather than the
 * level0 branch I will assume that you already made the choice to commit to
 * use the Qt framework.
 *
 * This main.cpp is all but regular Qt app. Go and browse other header files
 * for more information:
 *  - MainWindow.h Defines the main window. Again, it is very regular Qt
 *    boilerplate, with an example of event handling (pressing Escape quits the
 *    program) and displaying the OpenGL widget in full window.
 *  - ViewerWidget.h This defines a Qt Widget, so a class that handles like any
 *    other widget, plays well with layouts, etc. and can be integrated into
 *    windows' .ui files (using the Qt Designer editor for instance).
 *    ViewerWidget basically is a proxy to Viewer, the core viewer code, that
 *    on the opposite does not talk to Qt at all.
 *  - Viewer.h Defines the core render, in pure OpenGL. Nothing to do with Qt.
 *    It may even get called from a non Qt program as is. The Viewer.cpp file
 *    basically level0's main.cpp package as a class. FOr the most of it,
 *    you'll recognize it and will be able to follows level0's comments to
 *    understand it.
 */

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
