// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "FileInput.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSettings>

FileInput::FileInput(QWidget * parent, const QString & typesNew)
: QWidget(parent)
{
	lineEdit = new QLineEdit();
	button = new QPushButton("...");
	
	QHBoxLayout * layout = new QHBoxLayout();
	
	layout->addWidget(lineEdit);
	layout->addWidget(button);
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);
	
	setLayout(layout);
	
	connect(button, SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(lineEditChanged(const QString &)));
	
	types = typesNew;
}

void FileInput::disable()
{
	lineEdit->setEnabled(false);
	button->setEnabled(false);
}

void FileInput::enable()
{
	lineEdit->setEnabled(true);
	button->setEnabled(true);
}

void FileInput::lineEditChanged(const QString & text)
{
	fileName = text;
	emit signalFileChanged();
}

void FileInput::openFileDialog()
{
	QSettings settings;
	QString fileNew = QFileDialog::getOpenFileName(this, tr("Choose file"), settings.value("default_dir").toString(), types);
	
	if ( ! fileNew.isEmpty() )
	{
		lineEdit->setText(fileNew);
		//emit signalFileChanged();
	}
}

void FileInput::setFileName(const QString &fileName)
{
	lineEdit->setText(fileName);
}

void FileInput::setTypes(const QString &typesNew)
{
	types = typesNew;
}
