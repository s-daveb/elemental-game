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
#include <QWidget>

#include <memory>

DocumentEditor::DocumentEditor(QWidget* parent, const QString& filepath)
    : QWidget(parent)
    , file_info(filepath)
    , ui(std::make_unique<Ui::DocumentEditor>())
{
	this->ui->setupUi(this);
	this->main_window = qobject_cast<QMainWindow*>(parent->parent());

	connect(this->action_save, &QAction::triggered, this, [this]() {
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

	this->main_window->addAction(this->action_save);
	this->main_window->addAction(this->action_save_as);

	this->loadFile(filepath);
}
DocumentEditor::~DocumentEditor() = default;

void DocumentEditor::saveFile(bool compact)
{
	throw IOCore::NotImplementedException();
}

void DocumentEditor::loadFile(const QString& path)
{
	QFileInfo file_info(path);
	if (!file_info.exists()) {
		throw IOCore::Exception(
		    fmt::format("File not found: {}", path.toStdString())
		);
	}
	// Check if mainWidget already has a layout, otherwise create one
	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(this->layout());
	if (!layout) {
		layout = new QVBoxLayout(this);
	}

	auto filename = file_info.baseName();
	auto suffix = file_info.suffix();

	if (suffix == "json") {
		this->editor_widget = new JsonEditor(this, path);
		layout->insertWidget(0, this->editor_widget);
	} else {
		try {
			throw IOCore::Exception(fmt::format(
			    "Unsupported file type: {}", suffix.toStdString()
			));
		} catch (IOCore::Exception& e) {
			auto dialog = ExceptionDialog::display(this, e);
		}
	}
}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
