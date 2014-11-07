// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__FileInput__
#define __gavqt__FileInput__

#include <QLineEdit>
#include <QPushButton>

class FileInput : public QWidget
{
	Q_OBJECT
	
public:
	
	FileInput(QWidget * parent, const QString & typesNew);
	
	void disable();
	void enable();
	const QString & getFileName() const;
	void setFileName(const QString & fileName);
	void setTypes(const QString & typesNew);
	
public slots:
	
	void lineEditChanged(const QString & text);
	void openFileDialog();
	
signals:
	
	void signalFileChanged() const;
	
private:
	
	QString types;
	QString fileName; // QLineEdit acts strangely when querying its text
	QLineEdit * lineEdit;
	QPushButton * button;
};

inline const QString & FileInput::getFileName() const { return fileName; }

#endif /* defined(__gavqt__StatusLabel__) */
