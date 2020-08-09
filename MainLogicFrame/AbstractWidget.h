#ifndef  __ABSTRACTWIDGET__
#define  __ABSTRACTWIDGET__
#include "MainFrame_global.h"
#include "SignalQueue.h"

class MAINFRAME_EXPORT AbstractWidget : public QWidget
{
	Q_OBJECT
public:
	/// \brief 构造函数
	/// \param[in] parent 父窗口
	AbstractWidget(QWidget* parent = 0);

	/// \brief 虚析构函数，防止内存泄漏
	virtual ~AbstractWidget();

	/// \brief 收消息函数
	/// 在函数内部执行get函数即可收到消息，阻塞等待接收
	virtual void OnMessage();

	/// \brief 窗口关闭事件
	virtual void OnClose();

	/// \brief 输出log
	/// \param[in] strTgtLog 日志内容
	void Log(const QString& strTgtLog) const;

	/// \brief 计算分辨率大小，并且返回计算后的合适大小
	/// \param[in] size 原来的大小
	/// \retval 返回计算后的值
	int CalculateResolution(const int& size) const;

	/// \brief 根据给定的插件实例判断是否在运行
	/// \param[in] strPlug 插件名称
	/// \param[in] strInstance 插件实例名称
	bool PlugIsRuning(const QString& strPlug, const QString& strInstance) const;

	/// \brief 发送信号
	/// \param[in] sig 信号种类
	/// \param[in] arg 信号参数
	void SendSIG(Signal_ sig, void* arg) const;

private:
	static AbstractWidget* m_pInstanceWidget;
	friend class MainFrame;
};

#endif

