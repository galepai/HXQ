#ifndef _CAMERA2_WIDGET_H
#define _CAMERA2_WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <queue>

namespace Ui {
class camera2Widget;
}

class camera2Widget : public QWidget
{
    Q_OBJECT

public:
	explicit camera2Widget(QWidget *parent = 0);
	~camera2Widget();

	void ReadIni();

public slots:
	void SaveExposureToIni();

private:
	Ui::camera2Widget *ui;
	std::vector<QString> name, type, value;
	std::vector<void*> vector_horizontalLayout;
	std::queue<void*> queue_labels, queue_lineEdits;
	QHBoxLayout* horizontalLayout;
};

#endif // _CAMERA2_WIDGET_H
