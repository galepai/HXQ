#ifndef _CAMERA_WIDGET_H
#define _CAMERA_WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <queue>
#include <QStandardItemModel>

namespace Ui {
class cameraWidget;
}

class cameraWidget : public QWidget
{
    Q_OBJECT

public:
	explicit cameraWidget(QWidget *parent = 0);
	~cameraWidget();

	void updateCombox();

public slots:
	void SaveExposureToIni();
	void ReadIni(int index);

private:
	Ui::cameraWidget *ui;
	std::vector<QString> name, type, value;
	std::vector<void*> vector_horizontalLayout, vector_labels, vector_lineEdits;
	//std::queue<void*> queue_labels, queue_lineEdits;
	//QHBoxLayout* horizontalLayout;
	//QLineEdit* value2;
	//QLabel* nameLabel2;
	QStandardItemModel*	_model;
};

#endif // _CAMERA_WIDGET_H
