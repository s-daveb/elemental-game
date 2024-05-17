/* DocumentEditor.impl.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef DOCUMENT_EDITOR_IMPL_HPP
#define DOCUMENT_EDITOR_IMPL_HPP

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
#include <QVBoxLayout>
#include <QWidget>

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

template<typename TWidget>
void DocumentEditor::loadFile(const QString& path)
{
	QFileInfo file_info(path);
	if (!file_info.exists()) {
		throw IOCore::Exception(
		    fmt::format("File not found: {}", path.toStdString())
		);
	}

	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(this->layout());
	if (!layout) {
		layout = new QVBoxLayout(this);
		this->setLayout(layout);
	}

	auto filename = file_info.baseName();
	auto suffix = file_info.suffix();

	if (suffix == "json") {
		this->editor_widget = new JsonEditor(this, path);
		layout->insertWidget(0, this->editor_widget);

		this->main_window->addAction(this->action_save);
		this->main_window->addAction(this->action_save_as);

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

#endif
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
