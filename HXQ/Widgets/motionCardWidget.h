#ifndef _MOTION_CARD_WIDGET_H
#define _MOTION_CARD_WIDGET_H

#include <QWidget>

namespace Ui {
	class motionCardWidget;
}

class motionCardWidget : public QWidget
{
    Q_OBJECT

public:
	explicit motionCardWidget(QWidget *parent = 0);
	~motionCardWidget();

	void ReadIni();

public slots:
	void SaveMotionCardToIni();

private:
	Ui::motionCardWidget *ui;
};

#endif // _MOTION_CARD_WIDGET_H
