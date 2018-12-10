#ifndef _MODEL_WIDGET_H
#define _MODEL_WIDGET_H

#include <QWidget>

namespace Ui {
class modelWidget;
}

class modelWidget : public QWidget
{
    Q_OBJECT

public:
	explicit modelWidget(QWidget *parent = 0);
	~modelWidget();

	void ReadIni();

public slots:
	void SaveExposureToIni();
	void loadGouModel();

private:
	Ui::modelWidget *ui;
};

#endif // _CAMERA_WIDGET_H
