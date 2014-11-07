// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#include "ImportWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>
#include <QSpacerItem>
#include <QLabel>
#include <QScrollBar>

bool ImportWindow::fileIsGenbank(const QString &fileName)
{
	return fileName.endsWith(".gb") || fileName.endsWith(".gbk") || fileName.endsWith("gbf");
}

ImportWindow::ImportWindow(QWidget * parent)
: QDialog(parent)
{
//	setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	setWindowModality(Qt::WindowModal);
	
	QVBoxLayout * layout = new QVBoxLayout();
	
	preview = new QPlainTextEdit();
	previewReference = new QLineEdit();
	
	QFont font = preview->font();
	font.setPixelSize(10);
	QFontMetrics fm(font);
	preview->setFont(font);
	preview->setFixedHeight(fm.height() * previewLines);
	preview->setStyleSheet("background-color: #F0F0F0; color: #555555");
	
	preview->setReadOnly(true);
	preview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	preview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	previewReference->setReadOnly(true);
	previewReference->setStyleSheet("background-color: #F0F0F0; color: #555555");
	previewReference->setFont(font);
	
	QLabel * labelFile = new QLabel(tr("File"));
	
	fileInput = new FileInput(0,
							  tr(
								 "All known file types (*.ggr *.hvt *.xmfa *.maf *.nxs *.mfa *.fasta *.fna *.fa *.afa *.aln *.gb *.gbk *.gbf *.tre *.tree *.nwk *.nxs *.nex);;"
								 "All files (*.*);;"
								 "Gingr workspaces (*.ggr *.hvt);;"
								 "Alignments (*.xmfa *.maf *.nxs *.nex *.mfa *.fasta *.fna *.fa *.afa *.aln);;"
								 "Trees (*.tre *.tree *.nwk *.nxs *.nex);;"
								 "Annotations (*.gb *.gbk *.gbf);;"
								 "Nexus files (*.nxs *.nex);;"
								 ));
	
	QLabel * labelReference = new QLabel(tr("Reference"));
	
	fileInputReference = new FileInput(0,
									   tr(
										  "All reference files (*.fasta *.fna *.fa *.gb *.gbk *.gbf);;"
										  "Fasta files (*.fasta *.fna *.fa);;"
										  "Genbank files (*.gb *.gbk *.gbf);;"
										  "All files (*);;"
										  ));
	
	QVBoxLayout * layoutFile = new QVBoxLayout();
	
	frameFile = new QFrame();
	frameFile->setLayout(layoutFile);
	frameFile->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	
	layoutFile->addWidget(labelFile);
	layoutFile->addWidget(fileInput);
	layoutFile->addWidget(preview);
	
	layout->addWidget(frameFile);
	layout->addSpacerItem(new QSpacerItem(20, 20));
	
	QHBoxLayout * layoutOpenAs = new QHBoxLayout();
	QLabel * labelOpenAs = new QLabel(tr("Open as"));
	
	comboBoxType = new QComboBox();
	comboBoxType->setMinimumWidth(500);
	
	addFileType(tr("Gingr workspace"), GINGR);
	addFileType(tr("MAF alignment"), ALN_MAF);
	addFileType(tr("Multi-fasta alignment"), ALN_MFA);
	addFileType(tr("XMFA alignment"), ALN_XMFA);
	addFileType(tr("VCF variants"), VAR_VCF);
	addFileType(tr("Nexus"), NEXUS);
	addFileType(tr("Nexus (import alignment only)"), ALN_NXS);
	addFileType(tr("Nexus (import tree only)"), TRE_NXS);
	addFileType(tr("Newick tree"), TRE_NWK);
	addFileType(tr("Genbank annotations"), ANN_GBK);
	
	layoutOpenAs->addWidget(labelOpenAs);
	layoutOpenAs->addWidget(comboBoxType);
	
	layout->addLayout(layoutOpenAs);
	layout->addSpacerItem(new QSpacerItem(20, 20));

	QVBoxLayout * layoutReference = new QVBoxLayout();
	
	frameReference = new QFrame();
	frameReference->setLayout(layoutReference);
	frameReference->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	
	layoutReference->addWidget(labelReference);
	layoutReference->addWidget(fileInputReference);
	layoutReference->addWidget(previewReference);
	
	layout->addWidget(frameReference);
	layout->addSpacerItem(new QSpacerItem(20, 20));
	
	QHBoxLayout * layoutBottom = new QHBoxLayout();
	
	QPushButton * buttonCancel = new QPushButton(tr("Cancel"));
	buttonOpen = new QPushButton(tr("Open"));
	
	layoutBottom->addWidget(buttonCancel);
	layoutBottom->addWidget(buttonOpen);
	
	layout->addLayout(layoutBottom);
	
	setLayout(layout);
	
	connect(fileInput, SIGNAL(signalFileChanged()), this, SLOT(updateFile()));
	connect(fileInputReference, SIGNAL(signalFileChanged()), this, SLOT(updateReference()));
	
	connect(buttonOpen, SIGNAL(clicked()), this, SLOT(open()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValuesChanged(int)));
	setWindowTitle(tr("Open"));
}

void ImportWindow::setFile(const QString &fileNew)
{
	fileInput->setFileName(fileNew);
	fileInputReference->setFileName("");
	
	updateFile();
	updateReference();
}

void ImportWindow::cancel()
{
	hide();
}

void ImportWindow::comboBoxValuesChanged(int)
{
	updateOptions();
}

void ImportWindow::open()
{
	hide();
	emit signalOpen(fileInput->getFileName(), fileInputReference->getFileName(), comboBoxValues.at(comboBoxType->currentIndex()));
}

void ImportWindow::updateFile()
{
	QString fileName = fileInput->getFileName();
	FileType type = GINGR;
	
	if
	(
		fileName.endsWith(".nwk") ||
		fileName.endsWith(".tree") ||
		fileName.endsWith(".tre")
	)
	{
		type = TRE_NWK;
	}
	else if
		(
		 fileName.endsWith(".ggr") ||
		 fileName.endsWith(".hvt")
		 )
	{
		type = GINGR;
	}
	else if
		(
		 fileName.endsWith(".xmfa")
		 )
	{
		type = ALN_XMFA;
	}
	else if
		(
		 fileName.endsWith(".maf")
		 )
	{
		type = ALN_MAF;
	}
	else if ( fileIsGenbank(fileName) )
	{
		type = ANN_GBK;
	}
	else if
		(
		 fileName.endsWith(".mfa") ||
		 fileName.endsWith(".fasta") ||
		 fileName.endsWith(".fa") ||
		 fileName.endsWith(".afa") ||
		 fileName.endsWith(".aln") ||
		 fileName.endsWith(".fna")
		 )
	{
		type = ALN_MFA;
	}
	else if
		(
		 fileName.endsWith(".vcf")
		 )
	{
		type = VAR_VCF;
	}
	else if
		(
		 fileName.endsWith(".nxs") ||
		 fileName.endsWith(".nex")
		 )
	{
		type = ANN_GBK;
	}
	
	if ( type == ALN_MAF || type == VAR_VCF )
	{
		preview->setWordWrapMode(QTextOption::NoWrap);
	}
	else
	{
		preview->setWordWrapMode(QTextOption::WrapAnywhere);
	}
	
	comboBoxType->setCurrentIndex(getComboBoxIndexFromFileType(type));
	preview->clear();
	
	if ( type == GINGR )
	{
		preview->appendPlainText("[binary file]");
	}
	else if ( ! fileName.isEmpty() )
	{
		QFile file(fileName);
		QString previewString;
		
		if ( file.open(QFile::ReadOnly | QFile::Text) )
		{
			for ( int i = 0; i < previewLines; i++ )
			{
				previewString.append(file.readLine().left(1024));
				
				if ( previewString[previewString.length() - 1] != '\n' )
				{
					previewString.append('\n');
				}
			}
		}
		
		file.close();
		preview->appendPlainText(previewString);
		preview->verticalScrollBar()->triggerAction(QScrollBar::SliderToMinimum);
	}
	
	
}

void ImportWindow::updateReference()
{
	QString fileName = fileInputReference->getFileName();
	
	if ( fileName.isEmpty() )
	{
		previewReference->clear();
		return;
	}
	
	QFile file(fileName);
	QString line;
	
	if ( file.open(QFile::ReadOnly | QFile::Text) )
	{
		do
		{
			line = file.readLine(1024);
		}
		while ( line[0] == '#' );
	}
	
	file.close();
	previewReference->setText(line);
	previewReference->setCursorPosition(0);
}

void ImportWindow::addFileType(const QString & name, ImportWindow::FileType value)
{
	comboBoxType->addItem(name);
	comboBoxValues.push_back(value);
}

int ImportWindow::getComboBoxIndexFromFileType(ImportWindow::FileType type) const
{
	for (int i = 0; i < comboBoxValues.size(); i++)
	{
		if ( comboBoxValues.at(i) == type )
		{
			return i;
		}
	}
	
	return 0;
}

void ImportWindow::updateOptions()
{
	FileType type = comboBoxValues.at(comboBoxType->currentIndex());
	
	if ( type == ANN_GBK || type == TRE_NWK || type == TRE_NXS || type == GINGR )
	{
		frameReference->hide();
//		fileInputReference->setEnabled(false);
	}
	else
	{
		frameReference->show();
//		fileInputReference->setEnabled(true);
	}
	
	adjustSize();
}
