#ifndef _CAMERA_WIDGET_H
#define _CAMERA_WIDGET_H

#include <QWidget>

namespace Ui {
class cameraWidget;
}

class cameraWidget : public QWidget
{
    Q_OBJECT

public:
	explicit cameraWidget(QWidget *parent = 0);
	~cameraWidget();

	void ReadIni();

public slots:
	void SaveExposureToIni();

private:
	Ui::cameraWidget *ui;
};

#endif // _CAMERA_WIDGET_H
