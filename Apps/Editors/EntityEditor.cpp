
/* EntityEditor.cpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <fstream>
#include <functional>

#include <QFileDialog>
#include <QListWidgetItem>
#include <QResizeEvent>
#include <QWidget>

#include <nlohmann/json.hpp>

#include "EntityEditor.hpp"
#include "ui_EntityEditor.h"

using nlohmann::json;

EntityEditor::EntityEditor(QWidget* parent, const QString& filepath)
    : QWidget(parent)
    , filepath(filepath)
    , available_components(this)
    , ui(*(new Ui::EntityEditor()))
{
	ui.setupUi(this);

	/*
	for (auto& type : components->GetRegisteredTypes()) {
	        QString typeName = type.name.c_str();
	        ui.availableComponentsListWidget->addItem(typeName);
	}*/
	this->load_file(filepath);

	connect(
	    ui.selectPushButton,
	    SIGNAL(clicked(bool)),
	    this,
	    SLOT(selectButtonClicked(bool))
	);

	connect(
	    ui.deselectPushButton,
	    SIGNAL(clicked(bool)),
	    this,
	    SLOT(deselectButtonClicked(bool))
	);

	connect(
	    ui.saveButton,
	    SIGNAL(clicked(bool)),
	    this,
	    SLOT(saveButtonClicked(bool))
	);
}

EntityEditor::~EntityEditor()
{
	delete &ui;
}

void EntityEditor::selectButtonClicked(bool checked)
{
	auto qlist = ui.availableComponentsListWidget->selectedItems();

	for (auto* listItem : qlist) {
		ui.selectedComponentsListWidget->addItem(listItem->text());
	}
}

void EntityEditor::deselectButtonClicked(bool checked)
{
	auto* selectedItem = ui.selectedComponentsListWidget->currentItem();
	if (selectedItem) {
		delete selectedItem;
	}
}

void EntityEditor::saveButtonClicked(bool checked)
{
	QString file_name = QFileDialog::getSaveFileName(
	    (QWidget*)this, "Save as .json", QDir::current().dirName()
	);

	this->save_file(file_name);
}

void EntityEditor::save_file(const QString& file_name)
{
	std::ofstream output_file(file_name.toStdString());

	json jsbuffer;

	auto& listWidget = *(ui.selectedComponentsListWidget);
	for (auto index = 0; index < listWidget.count(); index++) {
		auto& component = *(listWidget.item(index));
		std::string component_name = component.text().toStdString();

		json component_json = {};
		jsbuffer["components"][index] = {
			{ component_name, component_json },
		};
	}

	output_file << jsbuffer;
	output_file.flush();
	output_file.close();
}

void EntityEditor::load_file(const QString& file_name)
{
	// Open the input file stream
	std::ifstream input_file(file_name.toStdString());
	json json_data;

	input_file >> json_data;

	json components = json_data["components"];
	// Store the key-value pairs in the entity_components map
	for (auto& [key, value] : components.items()) {
		QListWidgetItem listItem(
		    ui.selectedComponentsListWidget,
		    QListWidgetItem::ItemType::UserType
		);
		listItem.setText(key.c_str());
		ui.selectedComponentsListWidget->addItem(listItem.text());
	}
}
// void EntityEditor::paintEvent(QPaintEvent* event)
//{
//	ui.imageLabel->resize(qobject_cast<QWidget*>(this->parent())->size());
// }

//  clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80
// ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
//
