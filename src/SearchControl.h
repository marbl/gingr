// Copyright © 2014, Battelle National Biodefense Institute (BNBI);
// all rights reserved. Authored by: Brian Ondov, Todd Treangen, and
// Adam Phillippy
//
// See the LICENSE.txt file included with this software for license information.

#ifndef __gavqt__SearchControl__
#define __gavqt__SearchControl__

#include "OptionPanel.h"
#include <QLineEdit>
#include "StatusLabel.h"

class SearchControl : public OptionPanel
{
	Q_OBJECT;
	
public:
	
	SearchControl(QWidget * parent = 0);
	
	void initialize();
	void minimize(int targetX, int targetY);
	void restore();
	
public slots:
	
	void clear();
	void matchCaseChaged(int state);
	void resultsChangedTracks(int results);
	void resultsChangedAnnotations(int results);
	void textChanged(const QString &);
	
signals:
	
	void signalSearchChanged(const QString & text, bool matchCase) const;
	
private:
	
	void updateSearch() const;
	
	QLineEdit * lineEdit;
	StatusLabel * labelResultsTracks;
	StatusLabel * labelResultsAnnotations;
	bool matchCase;
};

#endif /* defined(__gavqt__SearchControl__) */
