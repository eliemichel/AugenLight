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

#include <QKeyEvent>

#include "MainWindow.h"
#include "ViewerWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("AugenLight v1.0 / Option Qt -- Copyright (c) 2017 - 2019 -- Élie Michel");
	resize(800, 600);

	// You might want to put this new ViewerWidget in an attribute later if you
	// need to access it from others methods of MainWindow (e.g. slots).
	// Note that even though this is a mamory allocation (a call to "new"), it
	// does not need to be explicitely freed. Because we provide 'this' as the
	// parent widget, whenever 'this' (i.e. the current MainWindow) will be
	// freed, the ViewerWidget will be as well.
	setCentralWidget(new ViewerWidget(this));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape) {
		close();
	}
}
