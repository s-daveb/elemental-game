/* EditorWindow.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/Exception.hpp"

#include <QMainWindow>
#include <QWidget>

namespace ResourceEditor {
class MainWindow final : public QMainWindow {
	Q_OBJECT; // NOLINT
    public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow() final = default;

	void openFile(bool checked) { throw IOCore::NotImplementedException(); }

    private:
};

}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen ::w
