/* ExceptionDialog.hpp
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

#include "IOCore/Exception.hpp"
#include "ui_ExceptionDialog.h"

#include <QDialog>
#include <QLabel>
#include <QWidget>

#include <memory>

class ExceptionDialog : public QDialog {
	// NOLINTNEXTLINE
	Q_OBJECT

    public:
	explicit ExceptionDialog(
	    QWidget* parent = nullptr, QString problem = "Uncaught Exception"
	)
	{
		this->ui.setupUi(this);
		this->ui.label->setText(problem);
	}

	ExceptionDialog(const ExceptionDialog&) = delete;
	ExceptionDialog& operator=(const ExceptionDialog&) = delete;

	~ExceptionDialog() override = default;

	static std::shared_ptr<ExceptionDialog> display(
	    QWidget* parent, const IOCore::Exception& exception,
	    bool killProgram = false
	)
	{
		auto instance = std::make_shared<ExceptionDialog>(parent);
		if (parent && killProgram) {
			connect(
			    instance.get(),
			    &QDialog::finished,
			    parent,
			    &QWidget::close
			);
		}

		instance->setModal(true);
		instance->displayException(exception);
		instance->exec(); // Use exec() to make it modal immediately
		return instance;
	}

    protected:
	void displayException(const IOCore::Exception& exception)
	{
		this->ui.textDisplay->setText(exception.what());
	}

    private:
	Ui::ExceptionDialog ui;
};
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
