//
//  SearchControl.h
//  gavqt
//
//  Created by Brian Ondov on 5/6/13.
//
//

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
	void resultsChanged(int results);
	void textChanged(const QString &);
	
signals:
	
	void signalSearchChanged(const QString & text, bool matchCase) const;
	
private:
	
	void updateSearch() const;
	
	QLineEdit * lineEdit;
	StatusLabel * labelResults;
	bool matchCase;
};

#endif /* defined(__gavqt__SearchControl__) */
