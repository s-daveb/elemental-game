/* EntityEditor.hpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ui_EntityEditor.h"
#include <QFrame>
#include <QObject>
#include <QWidget>

#include <QStringListModel>

namespace Ui {
class EntityEditor;
}

class EntityEditor : public QWidget {
	Q_OBJECT

    public:
	explicit EntityEditor(
	    QWidget* parent = nullptr, const QString& filepath = ""
	);
	~EntityEditor();

	const QString& GetFileName() { return this->filepath; }

    public slots:
	void selectButtonClicked(bool checked = false);
	void deselectButtonClicked(bool checked = false);

	void saveButtonClicked(bool checked = false);

    protected:
	void save_file(const QString& fileName);
	void load_file(const QString& fileName);

    private:
	QString filepath;
	QStringListModel available_components;

	Ui::EntityEditor& ui;
};

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
