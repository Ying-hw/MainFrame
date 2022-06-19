#ifndef  __ABSTRACTOBJECT__
#define  __ABSTRACTOBJECT__

#include <QObject>

#include "MainFrame_global.h"
#include "SignalQueue.h"
#include <QString>
#include "TreeStruct.h"

class MAINFRAME_EXPORT AbstractObject 
{

public:
	AbstractObject();
	~AbstractObject();

	/// \brief 收消息函数
	/// 在函数内部执行get函数即可收到消息，阻塞等待接收
	virtual void OnMessage();


	/// \brief 输出log
	/// \brief Grade 日志等级
	/// \param[in] strTgtLog 日志内容
	static void Log(LogGrade Grade, const QString& strTgtLog);

	/// \brief 根据给定的插件实例判断是否在运行
	/// \param[in] strPlug 插件名称
	/// \param[in] strInstance 插件实例名称
	bool PlugIsRuning(const QString& strPlug, const QString& strInstance) const;

	/// \brief 发送信号
	/// 信号类型可分为命令类型和线程类型，命令类型发送立刻执行，等待执行完成，线程类型执行系统线程，不等待，默认为线程
	/// \param[in] sig 信号种类
	/// \param[in] arg 信号参数
	/// \param[in] type 信号类型
	void SendSIG(Signal_ sig, void* arg, Signal_Type type = Signal_Type::THREAD) const;

private:
	friend class MainFrame;					///< 友元类，可访问私有的函数
	AbstractObject* m_pObjectInstance;		///< root根节点
	friend class Tree_ApiFun::TreeFunction;  ///< 内部数据结构
};

#endif
