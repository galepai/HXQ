#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
	class serialWidget;
}

class serialWidget : public QWidget
{
    Q_OBJECT

public:
	explicit serialWidget(QWidget *parent = 0);
	~serialWidget();

	void ReadIni();
	void CreateData();

public slots:
	void SavePortToIni();

private:
	Ui::serialWidget *ui;
};

#endif // WIDGET_H
