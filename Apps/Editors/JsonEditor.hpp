/* JsonEditor.hpp
 * Copyright © 2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ui_JsonEditor.h"

#include <QFileInfo>
#include <QFrame>
#include <QObject>
#include <QWidget>

#include <QJsonModel.hpp>

#include <algorithm>
#include <memory>

namespace Ui {
class JsonEditor;
}

class JsonEditor : public QWidget {
	// NOLINTNEXTLINE
	Q_OBJECT

	template<typename TData>
	using Ptr = std::unique_ptr<TData>;

    public:
	JsonEditor(QWidget* parent = nullptr, const QString& filepath = "");
	~JsonEditor() override;

	auto getFileInfo() -> const QFileInfo& { return this->file_info; }
	auto getJsonModel() -> QJsonModel& { return this->json_model; }

    protected:
	void save_file(const QString& file_name);
	void load_file(const QString& file_path);

	QFileInfo file_info;
	QJsonModel json_model;

    private:
	Ptr<Ui::JsonEditor> ui;
};

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
