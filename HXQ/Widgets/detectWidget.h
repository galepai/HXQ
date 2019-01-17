#ifndef _DETECT_WIDGET_H
#define _DETECT_WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>

namespace Ui {
class detectWidget;
}

class detectWidget : public QWidget
{
    Q_OBJECT

public:
	explicit detectWidget(QWidget *parent = 0);
	~detectWidget();

	void ReadFromXml();

public slots:
	void SaveToXml();

private:
	Ui::detectWidget *ui;
	std::vector<QString> name, type, value;
	std::vector<void*> vector_horizontalLayout, vector_labels, vector_lineEdits;
};

#endif // _DETECT_WIDGET_H
