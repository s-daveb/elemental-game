/* JsonEditor.hpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QFileInfo>
#include <QFrame>
#include <QObject>
#include <QWidget>

#include <algorithm>
#include <memory>

namespace Ui {
class JsonEditor;
}

class QMainWindow;
class QJsonModel;

class JsonEditor : public QWidget {
	// NOLINTNEXTLINE
	Q_OBJECT

    public:
	JsonEditor(QWidget* parent = nullptr, const QString& filepath = "");
	~JsonEditor() override;

	auto getFileInfo() -> const QFileInfo& { return this->file_info; }
	auto getJsonModel() -> std::unique_ptr<QJsonModel>&
	{
		return this->json_model;
	}

    protected:
	void saveFile(bool compact = true);
	void loadFile(const QString& filePath);

	QAction* action_save;
	QAction* action_save_as;

	QFileInfo file_info;
	std::unique_ptr<QJsonModel> json_model;

    private:
	QMainWindow* main_window = nullptr;
	std::unique_ptr<Ui::JsonEditor> ui;
};

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
