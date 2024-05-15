
/* JsonEditor.cpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "JsonEditor.hpp"
#include "ui_JsonEditor.h"

#include "IOCore/Exception.hpp"
#include "QJsonModel.hpp"

#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QResizeEvent>
#include <QWidget>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>
#include <functional>
#include <memory>

using nlohmann::json;

const auto k_default_perms = static_cast<QJsonModel::FieldPermissions>(
    QJsonModel::kWritableKey | QJsonModel::kWritableValue
);

JsonEditor::JsonEditor(QWidget* parent, const QString& filePath)
    : QWidget(parent)
    , file_info(filePath)
    , json_model(std::make_unique<QJsonModel>(filePath, this, k_default_perms))
    , ui(std::make_unique<Ui::JsonEditor>())
{
	this->ui->setupUi(this);
	this->main_window = qobject_cast<QMainWindow*>(parent->parent());

	this->loadFile(filePath);

	this->action_save = new QAction(tr("Save"), this);
	this->action_save_as = new QAction(tr("Save As..."), this);

	connect(this->action_save, &QAction::triggered, this, [this]() {
		this->saveFile(false);
	});
	connect(this->action_save_as, &QAction::triggered, this, [this]() {
		auto selected_filepath = QFileDialog::getSaveFileName(
		    this->main_window,
		    tr("Save File As"),
		    this->file_info.filePath()
		);
		this->file_info.setFile(selected_filepath);
		this->saveFile(false);
	});

	this->show();
}

JsonEditor::~JsonEditor()
{
	this->ui.reset(nullptr);
}

void JsonEditor::saveFile(bool compact)
{
	ASSERT(this->json_model);
	std::ofstream output_file(file_info.filePath().toStdString());

	auto jsonified = QString(this->json_model->json(compact));

	output_file << jsonified.toStdString();
	output_file.flush();
	output_file.close();
}

void JsonEditor::loadFile(const QString& path)
{
	QFile file(path);

	if (this->json_model->load(path) == QJsonModel::kError) {
		throw IOCore::Exception(
		    fmt::format("Failed to load file: {}", path.toStdString())
		);
	}
	this->ui->treeView->setModel(this->json_model.get());
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
