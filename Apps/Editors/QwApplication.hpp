/* QwApplication.hpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/
 */

#pragma once

#include <QApplication>
#include <QMessageBox>
#include <typeinfo>

class QwApplication : public QApplication {
    public:
	QwApplication(int argc, char* argv[]) : QApplication(argc, argv) {}

	inline bool notify(QObject* receiver, QEvent* e) override
	{

		try {
			return QApplication::notify(receiver, e);
		} catch (std::exception& ex) {
			QMessageBox messageBox;
			messageBox.setWindowTitle("Uncaught Exception");
			messageBox.setIcon(QMessageBox::Critical);
			messageBox.setWindowFlag(Qt::WindowType::Dialog);
			messageBox.setText(ex.what());
			messageBox.exec();
			qFatal(
			    "Error %s sending event %s to object %s (%s)",
			    ex.what(),
			    typeid(*e).name(),
			    qPrintable(receiver->objectName()),
			    typeid(*receiver).name()
			);
		} catch (...) {
			QMessageBox messageBox;
			messageBox.setWindowTitle("Uncaught Exception");
			messageBox.setWindowFlag(Qt::WindowType::Dialog);
			messageBox.setIcon(QMessageBox::Critical);
			messageBox.setText("Unknown Error");
			messageBox.exec();
			qFatal(
			    "Error <unknown> sending event %s to object %s (%s)",
			    typeid(*e).name(),
			    qPrintable(receiver->objectName()),
			    typeid(*receiver).name()
			);
		}

		return false;
	}
};
