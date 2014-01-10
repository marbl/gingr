//
//  SearchControl.cpp
//  gavqt
//
//  Created by Brian Ondov on 5/6/13.
//
//

#include "SearchControl.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>

SearchControl::SearchControl(QWidget * parent)
: OptionPanel(parent)
{
}

void SearchControl::initialize()
{
	QHBoxLayout * layoutSearch = new QHBoxLayout();
	QVBoxLayout * layout = new QVBoxLayout();
	QHBoxLayout * layoutTracks = new QHBoxLayout();
	QHBoxLayout * layoutAnnotations = new QHBoxLayout();
	
	QPushButton * clearButton = new QPushButton("Clear");
	QCheckBox * caseCheckBox = new QCheckBox("Match case");
	lineEdit = new QLineEdit();
	labelResultsTracks = new StatusLabel();
	labelResultsAnnotations = new StatusLabel();
	
	QLabel * labelTracks = new QLabel(tr("tracks"));
	QLabel * labelAnnotations = new QLabel(tr("annotations"));
	
	labelResultsAnnotations->setMinimumWidth(100);
	labelResultsAnnotations->setMaximumWidth(100);
	labelAnnotations->setMinimumWidth(100);
	labelAnnotations->setMaximumWidth(100);
	labelResultsTracks->setMinimumWidth(100);
	labelResultsTracks->setMaximumWidth(100);
	labelTracks->setMinimumWidth(100);
	labelTracks->setMaximumWidth(100);
	labelResultsTracks->setMinimumHeight(19);
	labelResultsAnnotations->setMinimumHeight(19);
	
	layoutSearch->addWidget(lineEdit);
	layoutSearch->addWidget(clearButton);
	layout->addLayout(layoutSearch);
	layout->addWidget(caseCheckBox);
	layoutTracks->addWidget(labelResultsTracks);
	layoutTracks->addWidget(labelTracks);
	layoutTracks->setAlignment(Qt::AlignLeft);
	layoutAnnotations->addWidget(labelResultsAnnotations);
	layoutAnnotations->addWidget(labelAnnotations);
	layoutAnnotations->setAlignment(Qt::AlignLeft);
	layout->addLayout(layoutTracks);
	layout->addLayout(layoutAnnotations);
	
	setLayout(layout);
	
	connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
	connect(caseCheckBox, SIGNAL(stateChanged(int)), this, SLOT(matchCaseChaged(int)));
	
	matchCase = false;
	setWindowTitle("Find");
	adjustSize();
}

void SearchControl::minimize(int targetX, int targetY)
{
	OptionPanel::minimize(targetX, targetY);
	emit signalSearchChanged("", false);
}

void SearchControl::restore()
{
	OptionPanel::restore();
	updateSearch();
	lineEdit->selectAll();
}

void SearchControl::clear()
{
	lineEdit->setText("");
}

void SearchControl::matchCaseChaged(int state)
{
	matchCase = state == Qt::Checked;
	updateSearch();
}

void SearchControl::resultsChangedTracks(int results)
{
	if ( minimized )
	{
		return;
	}
	
	if ( lineEdit->text().size() == 0 )
	{
		labelResultsTracks->setText("");
	}
	else
	{
		labelResultsTracks->setText(QString("%1").arg(results));
	}
}

void SearchControl::resultsChangedAnnotations(int results)
{
	if ( minimized )
	{
		return;
	}
	
	if ( lineEdit->text().size() == 0 )
	{
		labelResultsAnnotations->setText("");
	}
	else
	{
		labelResultsAnnotations->setText(QString("%1").arg(results));
	}
}

void SearchControl::textChanged(const QString &)
{
	updateSearch();
}

void SearchControl::updateSearch() const
{
	emit signalSearchChanged(lineEdit->text(), matchCase);
}
