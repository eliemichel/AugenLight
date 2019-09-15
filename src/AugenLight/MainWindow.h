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

#ifndef H_MAIN_WINDOW
#define H_MAIN_WINDOW

#include <QMainWindow>

/**
 * Minimalistic main window that is filled with a ViewerWidget, for the sake of
 * the example.
 * Actually, we could have made the project slighty smaller by creating in the
 * main() function a ViewerWidget rather than a MainWindow. It would have
 * worked, but since it is very likely that you applicatino will then feature
 * more than one viewer widget (yeah, otherwise what's the point of using Qt at
 * all?) I've added this MainWindow class.
 * It is also a way to isolate user input (e.g. here press Escape to quit) from
 * the rendering logic (located in the ViewerWidget/Viewer classes).
 */
class MainWindow : public QMainWindow
{
	// Don't forget this if you wanna use Qt's signal/slot system with this
	// class (which will happend for sure eventually):
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);

protected:
	// Events are handled exactly as in any Qt program, by overriding the event
	// methods like for instance here keyPressEvent.
	void keyPressEvent(QKeyEvent *event) override;
};

#endif // H_MAIN_WINDOW
