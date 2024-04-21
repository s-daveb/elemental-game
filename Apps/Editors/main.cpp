/* main.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IOCore/Exception.hpp"
#include "MainWindow.hpp"

#include <stdexcept>

#include <QApplication>
#include <QDir>
#include <QMessageBox>

void handle_error(QApplication& app, const std::exception& exception)
{
	QMessageBox::critical(
	    nullptr, "Error", exception.what(), QMessageBox::Ok
	);
}

auto main(int argc, char* argv[]) -> int
{
	auto app = QApplication(argc, argv);
	try {
		QDir app_bundle_dir(app.applicationDirPath());
		app_bundle_dir.cdUp();
		app_bundle_dir.cdUp();
		QDir::setCurrent(app_bundle_dir.path());

		auto window = ResourceEditor::MainWindow();

		window.show();     // Show the main window
		return app.exec(); // Enter the application's event loop
	} catch (IOCore::Exception& exception) {
		handle_error(app, exception);
	} catch (std::exception& exception) {
		handle_error(app, exception);
	} catch (...) {
		handle_error(
		    app, std::runtime_error("Caught unknown exception")
		);
	}

	return -1;
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :

