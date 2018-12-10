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
	void OnZhuaShouLeft();	//抓手左
	void OnZhuaShouRight();	//抓手右
	void OnZhuaShouUp();	//抓手上
	void OnZhuaShouDown();	//抓手下
	void OnHandOpen();	//手指开
	void OnHandClose();	//手指关
	void OnBoShouUp();	//拨手伸
	void OnBoShouDown();	//拨手缩
	void OnBoShouShiPingUp();	//拨手左
	void OnBoShouShiPingDown();	//拨手右
	void OnProtectUp();	//换向器保护升
	void OnProtectDown();	//换向器保护缩

	void OnVisionUp();	//视觉升
	void OnVisionDown();	//视觉降
	void OnRotateStart();	//开始旋转
	void OnRotateStop();	//停止旋转
	void OnPaoGuang();	//停止旋转

	void OnManualMode();	//手动模式
	void OnAutoMode();	//自动模式
	void OnAutoStart();	//自动启动
	void OnAutoStop();	//自动停止

	//单步
	void OnForward();
	void OnBackWard();
	void OnForward_2();
	void OnBackWard_2();
	void OnRotateStepStart();
	void OnRotateStepStop();
	void OnWindUp();
	void OnWindDown();

	void OnDistanceConfirm(); //距离设置
	void OnDistanceConfirm_2(); //模组2距离设置
	void OnTimeConfirm();	//时间设置

	void OnDetectEnd();	//检测结束
	void OnResultGood();	//检测结果合格
	void OnResultBad();	//检测结果不合格

};

#endif // TestDialog_H
