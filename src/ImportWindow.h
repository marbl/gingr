// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gingr__ImportWindow__
#define __gingr__ImportWindow__

#include <QDialog>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QFrame>
#include "FileInput.h"

class ImportWindow : public QDialog
{
	Q_OBJECT
	
public:
	
	enum FileType
	{
		GINGR,
		ALN_MAF,
		ALN_MFA,
		ALN_NXS,
		VAR_VCF,
		ALN_XMFA,
		VAR_MFA,
		TRE_NWK,
		TRE_NXS,
		ANN_GBK,
		NEXUS,
	};
	
	const int previewLines = 30;
	
	static bool fileIsGenbank(const QString & fileName);
	ImportWindow(QWidget * parent);
	FileType getFileTypeFromComboBoxIndex(int index) const;
	void setFile(const QString & fileNew);
	
public slots:
	
	void cancel();
	void comboBoxValuesChanged(int);
	void open();
	void updateFile();
	void updateReference();
	
signals:
	
	void signalOpen(const QString & fileName, const QString & fileNameReference, ImportWindow::FileType type) const;
	
private:
	
	QVector<FileType> comboBoxValues;
	
	FileInput * fileInput;
	FileInput * fileInputReference;
	
	QFrame * frameFile;
	QFrame * frameReference;
	QPlainTextEdit * preview;
	QLineEdit * previewReference;
	
	QComboBox * comboBoxType;
	
	QPushButton * buttonOpen;
	
	void addFileType(const QString & name, FileType value);
	int getComboBoxIndexFromFileType(FileType type) const;
	void updateOptions();
};

inline ImportWindow::FileType ImportWindow::getFileTypeFromComboBoxIndex(int index) const { return comboBoxValues.at(index); }

#endif /* defined(__gingr__ImportWindow__) */
