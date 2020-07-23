#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "PluginInfo.h"
#include "SignalQueue.h"
#include "plugin.pb.h"
#include "AbstractNetWork.h"
#include "AbstractWidget.h"

#define   CONFIG   "../Data/Config/"
#define   LOG      "../Data/Log/"
#define   IMAGE    "../Data/Image/"

#define LOCATION
#define STRTOLOCATION(LOCATION) QRect(LOCATION.section(",",0,0).toInt(), \
									LOCATION.section(",", 1, 1).toInt(), \
									LOCATION.section(",", 2, 2).toInt(), \
									LOCATION.section(",", 3, 3).toInt());   

#define LOCATIONTOSTR(LOCATION) QString("%1,%2,%3,%4").arg(LOCATION->geometry().x()) \
										.arg(LOCATION->geometry().y()).arg(LOCATION->geometry().width()) \
										.arg(LOCATION->geometry().height());   ///< 矩形转为字符串，待废弃

enum class LogGrade{
	SeriousError,
	Error,
	Warning,
	Tips
};

namespace CommonTemplate {
	class InitType;
}
class MessageThread;
class MAINFRAME_EXPORT MainFrame : public QObject
{
	Q_OBJECT
public:
	
	/// \brief 构造函数
	MainFrame();
	
	/// \brief 析构函数
	~MainFrame();

	/// \brief 查找子节点的界面矩形位置
	/// 如果查不到就重新添加一个节点，并返回它的默认位置
	/// \param[in] targetWidget 目标界面
	/// \param[in] ChildName 子节点名称
	/// \param[in] strParent 父项名称
	/// \retval 返回界面位置
	const QRect GetNewTargetLocation(const AbstractWidget* targetWidget, const QString& ChildName, const QString& strParent);

	/// \brief 检查给定的插件或者实例是否在运行状态中
	/// 如果给定的插件或者实例正在加载的状态就运行消息线程，给上层插件主动发消息
	/// \param[in] strPlugName 插件名称
	/// \param[in] strChildName 实例名称
	/// \param[in] type 消息实例
	/// \retval 返回true代表给定的插件名称或者实例名称正在运行中，返回false没有在运行
	bool SendMsgThread(const QString& strPlugName, const QString& strChildName, CommonTemplate::InitType* type);

	/// \brief 检查给定的插件或者实例是否在运行状态中
	/// 如果给定的插件或者实例正在加载的状态就运行消息线程，给上层插件主动发消息
	/// \param[in] strPlugName 插件名称
	/// \param[in] strChildName 实例名称
	/// \retval 返回true代表给定的插件名称或者实例名称正在运行中，返回false没有在运行
	bool CheckPlugIsRuning(const QString& strPlugName, const QString& strChildName);

	/// \brief 释放已经加载的插件
	/// \param[in] pthis this指针
	/// \param[in] that 目标实例
	void FreeLib(AbstractWidget* that);

	/// \brief 释放已经加载的插件
	/// \param[in] strPlugName 插件名称
	void FreeLib(const QString& strPlugName);

	/// \brief 加载插件
	/// \param[in] pthis this指针
	/// \param[in] strTargetName 目标插件名称
	void LoadLib(const QString strTargetName);

	/// \brief 返回插件实例 ------- 待废弃
	AbstractWidget* LoadLib(const QString strTargetName, bool noShow);

	/// \brief 获取目标子类的信号集
	/// \param[in] pTgtChild 目标子类
	/// \retval 返回信号集实例
	SignalQueue* GetTgtSigQueueInstance(AbstractWidget* pTgtChild);

	/// \brief 更新该插件所有的窗口位置
	/// \param strPlugName 插件名称
	void UpdataGeometry(const QString& strPlugName);

	/// \brief 删除对应的widget
	/// 在释放之前删除对应的界面，并且返回剩余数量
	/// \param[in] mainWidget 对应的widget
	/// \retval 返回剩余数量
	int GetShowWidgetCount(MainWidget* mainWidget);

	/// \brief 根据子项获取父项的名称
	/// \param[in] ChildWidget 子项
	/// \retval 返回父项的名称
	const QString GetParentName(const AbstractWidget* ChildWidget);

	/// \brief 根据对应的widget获取本身的名字
	/// \param[in] AbsWidget 目标widget
	/// \retval 返回自身的名称
	const QString GetMyselfName(const AbstractWidget* AbsWidget);

public slots:
	/// \brief 初始化网络抽象类
	/// 通过父类指针指向子类，实现多态虚函数特性，可自由调用子类接口
	/// \param[in] net 所继承的网络接口子类
	/// \param[in] strChildName 对应的类名称
	void Initialize_NetInterface(AbstractNetWork* net, const QString& strChildName);

	/// \brief 初始化widget并且显示
	/// \param[in] pTgtWidget 目标Widget
	/// \param[in] strChildName 对应的类名称
	void Initialize_WidgetInterface(AbstractWidget* pTgtWidget, const QString& strChildName);

	/// \brief 更新窗口位置
	/// \param Tgt 窗口位置
	void UpdataGeometry(AbstractWidget* Tgt);

signals:
	/// \brief 释放插件信号
	/// \param[in] strPlugName 插件名称
	/// \param[in] isParent 父窗口
	void ReleaseWidget(const QString& strPlugName, bool isParent);

	/// \brief 初始化插件中的界面
	/// \param[in] targetPlug 目标插件信息
	void InitWidget(const PluginInfo* targetPlug);
	
private:
	/// \brief 读取插件配置文件
	void ConfigPlug();

	/// \brief 查找插件
	/// 查找启动插件，在插件配置中有启动优先级标识
	void FindPlugin();

	/// \brief 待废弃
	void StartPluginControl();

	/// \brief 更新配置文件 
	void UpdateConfigFile();

	/// \brief 读取XML文件
	/// \retval 返回读取到的根节点
	QDomNode ReadXML();

private slots:
	/// \brief 释放当前插件界面
	/// \param[in] strPlugName 插件名称
	/// \param[in] isParent 父窗口
	void ReleaseCurrentWidget(const QString& strPlugName, bool isParent);

	/// \brief 初始化当前插件界面
	/// \param[in] targetPlug 当前插件
	void LinkCurrentWidgetInterface(const PluginInfo* targetPlug);
private:
	QFile m_logFile;										///< 日志文件
	QList<QLibrary*> m_LstLoadlib;							///< 加载库
	QVector<PluginInfo> m_PluginConfig;						///< 所有插件信息
	QMap<AbstractWidget*, MainWidget*> m_mapMainWidget;     ///< 子窗口和框架窗口
	Allplugins m_pAllPlugins;								///< 待废弃
	QMap<QString, AbstractWidget*> m_mapAbstractWidget;		///< 插件或插件中的实例名称-抽象插件基类
	MessageThread* m_pMsgThread;							///< 消息线程
	QMap<QString, AbstractNetWork*> m_mapAbstractNet;       ///< 窗口对应的名称--窗口对应的网络接口
};

#endif // MAINFRAME_H
