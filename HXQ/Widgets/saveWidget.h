#ifndef _SAVE_WIDGET_H
#define _SAVE_WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>

namespace Ui {
	class saveWidget;
}

class saveWidget : public QWidget
{
    Q_OBJECT

public:
	explicit saveWidget(QWidget *parent = 0);
	~saveWidget();

	void ReadFromXml();
	void InitWidget();
	void SaveToSingleXml( QString& name,  QString& value);

public slots:
	void SaveToXml();
	void OnTest();
	void OnSaveTopBadPath();
	void OnSaveSideBadPath();
	void OnSaveTopAllPath();
	void OnSaveSideAllPath();
	void OnBad();
	void OnReceiveVarValue(QString varValue);

private:
	Ui::saveWidget *ui;
	std::vector<QString> name, type, value;
	std::vector<void*> vector_horizontalLayout, vector_labels, vector_lineEdits;
};

#endif // _SAVE_WIDGET_H
