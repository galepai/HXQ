#ifndef _MOTION_CARD_WIDGET_H
#define _MOTION_CARD_WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include "GalilThread.h"

namespace Ui {
	class motionCardWidget;
}

class motionCardWidget : public QWidget
{
    Q_OBJECT

public:
	explicit motionCardWidget(QWidget *parent = 0);
	~motionCardWidget();

	void ReadFromXml();

public slots:
	void SaveToXml();
	void OnTest();
	void OnReceiveVarValue(QString varValue);

private:
	Ui::motionCardWidget *ui;
	std::vector<QString> name, type, value;
	std::vector<void*> vector_horizontalLayout, vector_labels, vector_lineEdits;
	Galil_Thread* m_pGalil;
};

#endif // _MOTION_CARD_WIDGET_H
