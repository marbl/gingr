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
	QPushButton * clearButton = new QPushButton("Clear");
	QCheckBox * caseCheckBox = new QCheckBox("Match case");
	lineEdit = new QLineEdit();
	labelResultsTracks = new StatusLabel();
	labelResultsAnnotations = new StatusLabel();
	
	layoutSearch->addWidget(lineEdit);
	layoutSearch->addWidget(clearButton);
	layout->addLayout(layoutSearch);
	layout->addWidget(caseCheckBox);
	layout->addWidget(labelResultsTracks);
	layout->addWidget(labelResultsAnnotations);
	
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
		labelResultsTracks->setText(QString("%1 tracks").arg(results));
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
		labelResultsAnnotations->setText(QString("%1 annotations").arg(results));
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
