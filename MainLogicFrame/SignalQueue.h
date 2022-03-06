#ifndef   __SIGNALQUEUE__
#define   __SIGNALQUEUE__
#include "MainFrame_global.h"
#include "MacroDefine.h"
#include "AbstractObject.h"
/*!
 * \file SignalQueue.h
 *
 * \brief 接收信号类，被动开启线程接收上层发来的信号并进行处理
 *
 * \author yhw
 * \date 2021/09/12 13:16
 *
 * 
 */
class SignalQueue;
class MainWidget;
class AbstractWidget;
class AbstractNetWork;
class AbstractObject;

extern MAINFRAME_EXPORT SignalQueue* g_pSignal;

#define GETINSTANCE SignalQueue::GetTargetInstance

/*!
 * \class ParamInfo
 *
 * \brief 参数信息包，由参数和发送方和接收方构成
 *
 * \author yhw
 * \date 九月 
 */
struct ParamInfo{
	/// \brief 构造函数
	ParamInfo() :m_Params(NULL), m_pOldWidget(NULL){}
	
	/// \brief 构造函数
	/// \param[in] param 信号参数
	/// \param[in] strTarget 接收名称
	ParamInfo(void * param, QString strTarget) : m_Params(param), strTgtName(strTarget), m_pOldWidget(NULL){} 

	void *m_Params;          ///< 参数
	AbstractObject* m_pOldWidget;   ///< 发送方
	QString strTgtName;      ///< 接收方
};

/*!
 * \class SignalQueue
 *
 * \brief 信号接收队列
 *
 *
 * \author yhw
 * \date 2021年9月12日 
 */
class MAINFRAME_EXPORT SignalQueue : public QThread
{
	Q_OBJECT
public:

	/// \brief 构造函数
	SignalQueue();

	/// \brief 析构函数
	~SignalQueue();

	/// \brief 线程运行函数
	void run();

	/// \brief 发送参数与信号，外部调用
	/// \param[in] signal_ 信号
	/// \param[in] param 信号参数
	void Send_Message(Signal_ signal_, void *param); 

	void Send_Message(Signal_ SIG, AbstractNetWork* pNet, QString strName);

	/// \brief 发送参数与信号，外部调用
	/// \param[in] SIG 信号
	/// \param[in] param 信号参数
	/// \param[in] widget 目标参数
	void Send_Message(Signal_ SIG, void* param, const AbstractObject* widget);

	/// \brief 获取目标实例，加载目标动态库，挂载到框架中
	/// \param[in] strTarget 目标动态库名称
	/// \retval 返回该动态库实例
	static void* GetTargetInstance(QString strTarget);

	/// \brief 发送参数与信号，外部调用
	/// \param[in] signal_ 信号
	/// \param[in] param 信号参数
	void SetUserIdentify(void *, SystemUser user);

	/// \brief 查找使用信号队列的使用者
	/// \param[in] user 用户名
	/// \retval 返回该用户实例
	void *ReturnUser(SystemUser user);

	/// \brief 删除使用信号队列的使用者
	/// \param[in] user 用户名
	void removeUser(SystemUser SysUser);

	/// \brief 删除所有实例
	/// \param[in] pTgtWidget 目标上层实例
	void DeleteAll(MainWidget* pTgtWidget);
signals:
	/// \brief 隐藏窗口
	void hide_Window();
	/// \brief 关闭窗口
	void close_Window();
	/// \brief 退出系统
	void ExitSystem();
	/// \brief 最小化窗口
	void minWindow();
	/// \brief 最大化窗口
	void maxWindow();
	/// \brief 发送参数与信号，外部调用
	/// \param[in] that 目标窗口
	/// \param[in] strParentName 父窗口名称
	void showWindow(AbstractWidget* that, const QString& strParentName);
	/// \brief 更新窗口位置
	/// \param[in] that 目标窗口
	void UpdateWindowGeometry(AbstractWidget* that);
	
	void InitNet(AbstractNetWork* net, const QString& strTgtName);

private:
	/// \brief 选择信号，并且执行
	/// \param[in] p 信号与参数
	void selectSignal(QPair<Signal_, void *> p);

	/// \brief 将参数与信号压入队列
	/// \param[in] p 信号与参数
	void push_queue(QPair<Signal_, void *> p);

private:
	bool m_isRuning;    ///< 线程运行标志
	QQueue<QPair<Signal_, void *>> m_queue;   ///< 信号与参数队列
	QWaitCondition m_waitMutex;               ///< 条件变量
	QMutex m_Mutex;							  ///< 互斥锁
	QMap<SystemUser, void*> m_mapUser;        ///< 信号队列的使用者映射
	ParamInfo m_ParamInfo;					  ///< 参数信息包
	friend class MainFrame;                   ///< 友元类
};

#endif  //SIGNALQUEUE__

