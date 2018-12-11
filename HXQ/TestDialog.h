#ifndef TestDialog_H
#define TestDialog_H

#include <QDialog>
#include <QLineEdit>
#include <vector>
#include "GalilThread.h"

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
	explicit TestDialog(QWidget *parent = 0);
	~TestDialog();


private:
	Ui::TestDialog *ui;
	QTimer* m_pTimer;
	std::vector<bool> m_Y_States;
	std::vector<bool> m_X_States, m_Input; 
	std::vector<bool> m_origin_States;
	std::vector<ushort> m_D_Register;

	Galil_Thread* m_Galil;

	virtual void paintEvent(QPaintEvent *event);

	void setBtnQss(QPushButton *btn,
		QString normalColor, QString normalTextColor,
		QString hoverColor, QString hoverTextColor,
		QString pressedColor,QString pressedTextColor);

	void setTxtQss(QLineEdit *txt, QString normalColor, QString focusColor);
	void PaintCirle(QPainter& painter, const QPointF& center_circle, int radius, const QPen &pen, const QBrush &brush);
	void updateButtonStatu(QPushButton* PushButton, bool status);

public slots:
	void receiveInputValue(int value);
	void ChangeStyle();
	void CloseWindow();
	

};

#endif // TestDialog_H
