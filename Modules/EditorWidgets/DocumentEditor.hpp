/* DocumentEditor.hpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <QFileInfo>
#include <QWidget>

#include <memory>

namespace Ui {
class DocumentEditor;
}

class QMainWindow;

class DocumentEditor : public QWidget {
	// NOLINTNEXTLINE
	Q_OBJECT

	template<typename TData>
	using Ptr = std::unique_ptr<TData>;

    public:
	DocumentEditor(
	    QWidget* parent = nullptr, QMainWindow* mainWindow = nullptr,
	    const QString& filepath = ""
	);
	~DocumentEditor() override;

	auto getFileInfo() -> const QFileInfo& { return this->file_info; }

	template<typename TChild>
	void connectActions(const TChild* child);

    protected slots:
	void saveFile(bool compact = true);
	void loadFile(const QString& filePath);

    protected:
	QFileInfo file_info;

	void setupActions();

	QAction* separator = nullptr;
	QAction* action_save = nullptr;
	QAction* action_save_as = nullptr;

	QWidget* editor_widget = nullptr;
	QMainWindow* main_window = nullptr;

	Ptr<Ui::DocumentEditor> ui;
};

#include <QFileDialog>

template<typename TChild>
void DocumentEditor::connectActions(const TChild* child)
{
	connect(child->getSaveAction, &QAction::triggered, child, [child]() {
		child->saveFile(false);
	});
	connect(child->action_save_as, &QAction::triggered, child, [&]() {
		auto selected_filepath = QFileDialog::getSaveFileName(
		    this->main_window,
		    tr("Save File As"),
		    child->file_info.filePath()
		);
	});
}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
