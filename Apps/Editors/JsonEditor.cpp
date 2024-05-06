
/* JsonEditor.cpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "JsonEditor.hpp"
#include "QJsonModel.hpp"
#include "ui_JsonEditor.h"

#include "IOCore/Exception.hpp"

#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
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
    QJsonModel::WritableKey | QJsonModel::WritableValue
);

JsonEditor::JsonEditor(QWidget* parent, const QString& file_path)
    : QWidget(parent)
    , file_info(file_path)
    , json_model(file_path, this, k_default_perms)
    , ui(std::make_unique<Ui::JsonEditor>())
{
	this->ui->setupUi(this);

	// this->setWindowFilePath(file_path);
	this->load_file(file_path);

	this->show();
}

JsonEditor::~JsonEditor()
{
	this->ui.reset(nullptr);
}

void JsonEditor::save_file(const QString& file_name)
{
	std::ofstream output_file(file_name.toStdString());
	json jsbuffer;

	/*
	auto& listWidget = *(ui.selectedComponentsListWidget);
	for (auto index = 0; index < listWidget.count(); index++) {
	        auto& cmponent = *(listWidget.item(index));
	        std::string component_name = component.text().toStdString();

	        json component_json = {};
	        jsbuffer["components"][index] = {
	                { component_name, component_json },
	        };
	} */

	output_file << jsbuffer;
	output_file.flush();
	output_file.close();
}

void JsonEditor::load_file(const QString& path)
{
	QFile file(path);

	if (this->json_model.load(path)) {
		throw IOCore::Exception(
		    fmt::format("Failed to load file: {}", path.toStdString())
		);
	}
	this->ui->treeView->setModel(&this->json_model);
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
