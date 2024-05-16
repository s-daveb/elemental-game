/* DocumentEditor.tpp
 * Copyright © 2024 Saul D. Beniquez
 * License: The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ui_DocumentEditor.h"

#include "DocumentEditor.hpp"
#include "ExceptionDialog.hpp"
#include "JsonEditor.hpp"

#include "IOCore/Exception.hpp"
#include "fmt/core.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>

#include <memory>

DocumentEditor::DocumentEditor(
    QWidget* parent, QMainWindow* mainWindow, const QString& filepath
)
    : QWidget(parent)
    , main_window(mainWindow)
    , file_info(filepath)
    , ui(std::make_unique<Ui::DocumentEditor>())
{
	this->ui->setupUi(this);
	ASSERT(this->main_window);

	this->setupActions();
}
DocumentEditor::~DocumentEditor() = default;

void DocumentEditor::saveFile(bool compact)
{
	if (file_info.suffix() == "json") {
		auto json_editor =
		    qobject_cast<JsonEditor*>(this->editor_widget);
		if (json_editor) {
			json_editor->saveFile(compact);
		}
		this->main_window->statusBar()->showMessage("File saved", 2000);
	} else {
		throw IOCore::NotImplementedException();
	}
}

void DocumentEditor::setupActions()
{
	this->action_save = new QAction("Save", this);
	this->action_save_as = new QAction("Save As", this);

	this->action_save->setShortcut(QKeySequence::Save);
	this->action_save->setStatusTip("Save currently delected file");

	this->action_save_as->setShortcut(QKeySequence::SaveAs);
	this->action_save_as->setStatusTip(
	    "Save currently delected file with a new name"
	);

	auto file_menu =
	    this->main_window->menuBar()->findChild<QMenu*>("menuFile");

	this->separator = file_menu->addSeparator();
	file_menu->addAction(this->action_save);
	file_menu->addAction(this->action_save_as);

	connect(this->action_save, &QAction::triggered, this, [this]() {
		this->saveFile(false);
	});
	connect(this->ui->saveButton, &QPushButton::clicked, this, [this]() {
		this->saveFile(false);
	});
	connect(this->action_save_as, &QAction::triggered, this, [this]() {
		auto selected_filepath = QFileDialog::getSaveFileName(
		    this->main_window,
		    tr("Save File As"),
		    this->file_info.filePath()
		);

		if (!selected_filepath.isEmpty()) {
			this->file_info.setFile(selected_filepath);
			this->saveFile(true);
		}
	});
}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
