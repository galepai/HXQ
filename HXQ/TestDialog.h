#ifndef TestDialog_H
#define TestDialog_H

#include <QDialog>
#include <QLineEdit>
#include <vector>

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
	std::vector<bool> m_X_States; 
	std::vector<bool> m_origin_States;
	std::vector<ushort> m_D_Register;

	virtual void paintEvent(QPaintEvent *event);

	void setBtnQss(QPushButton *btn,
		QString normalColor, QString normalTextColor,
		QString hoverColor, QString hoverTextColor,
		QString pressedColor,QString pressedTextColor);

	void setTxtQss(QLineEdit *txt, QString normalColor, QString focusColor);
	void PaintCirle(QPainter& painter, const QPointF& center_circle, int radius, const QPen &pen, const QBrush &brush);
	void updateButtonStatu(QPushButton* PushButton, bool status);

public slots:
	void readyDataSlot(QByteArray str);
	void ChangeStyle();
	void CloseWindow();
	void OnZhuaShouLeft();	//ץ����
	void OnZhuaShouRight();	//ץ����
	void OnZhuaShouUp();	//ץ����
	void OnZhuaShouDown();	//ץ����
	void OnHandOpen();	//��ָ��
	void OnHandClose();	//��ָ��
	void OnBoShouUp();	//������
	void OnBoShouDown();	//������
	void OnBoShouShiPingUp();	//������
	void OnBoShouShiPingDown();	//������
	void OnProtectUp();	//������������
	void OnProtectDown();	//������������

	void OnVisionUp();	//�Ӿ���
	void OnVisionDown();	//�Ӿ���
	void OnRotateStart();	//��ʼ��ת
	void OnRotateStop();	//ֹͣ��ת
	void OnPaoGuang();	//ֹͣ��ת

	void OnManualMode();	//�ֶ�ģʽ
	void OnAutoMode();	//�Զ�ģʽ
	void OnAutoStart();	//�Զ�����
	void OnAutoStop();	//�Զ�ֹͣ

	//����
	void OnForward();
	void OnBackWard();
	void OnForward_2();
	void OnBackWard_2();
	void OnRotateStepStart();
	void OnRotateStepStop();
	void OnWindUp();
	void OnWindDown();

	void OnDistanceConfirm(); //��������
	void OnDistanceConfirm_2(); //ģ��2��������
	void OnTimeConfirm();	//ʱ������

	void OnDetectEnd();	//������
	void OnResultGood();	//������ϸ�
	void OnResultBad();	//��������ϸ�

};

#endif // TestDialog_H
