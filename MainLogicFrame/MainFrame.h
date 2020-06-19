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
	/// \retval 返回界面位置
	const QRect FindChildUiLocation(const QWidget* targetWidget, const QString& TypeName);

	/// \brief 检查给定的插件或者实例是否在运行状态中
	/// 如果给定的插件或者实例正在加载的状态就运行消息线程，给上层插件主动发消息
	/// \param[in] strPlugName 插件名称
	/// \param[in] strChildName 实例名称
	/// \param[in] type 消息实例
	/// \retval 返回true代表给定的插件名称或者实例名称正在运行中，返回false没有在运行
	bool CheckIsRuningPlug(const QString& strPlugName, const QString& strChildName, CommonTemplate::InitType* type);

	/// \brief 检查给定的插件或者实例是否在运行状态中
	/// 如果给定的插件或者实例正在加载的状态就运行消息线程，给上层插件主动发消息
	/// \param[in] strPlugName 插件名称
	/// \param[in] strChildName 实例名称
	/// \retval 返回true代表给定的插件名称或者实例名称正在运行中，返回false没有在运行
	bool CheckIsRuningPlug(const QString& strPlugName, const QString& strChildName);

	/// \brief 释放已经加载的插件
	/// \param[in] pthis this指针
	static void FreeLib(MainFrame* pthis);

	/// \brief 加载插件
	/// \param[in] pthis this指针
	/// \param[in] strTargetName 目标插件名称
	static void LoadLib(MainFrame* pthis, const QString strTargetName);

	/// \brief 待废弃
	static void* LoadLib(MainFrame* pthis, const QString strTargetName, bool noShow);

	/// \brief 初始化网络抽象类
	/// 通过父类指针指向子类，实现多态虚函数特性，可自由调用子类接口
	/// \param[in] net 所继承的子类
	static void InitializeNetInterface(AbstractNetWork* net);

	/// 待废弃
	inline void WriteLog(const QString& strlog) {
		m_logFile.write(strlog.toLocal8Bit());
	}

public slots:
	void MakePluginsProtobufFile(void* source);  //待废弃
	
	/// \brief 更新窗口位置
	void UpdataGeometry();
signals:
	/// \brief 释放插件信号
	void ReleaseWidget();

	/// \brief 初始化插件中的界面
	/// \param[in] targetPlug 目标插件信息
	void InitWidget(const PluginInfo* targetPlug);
private:
	/// \brief 读取插件配置文件
	void ReadPluginConfig();

	/// \brief 查找插件
	/// 查找启动插件，在插件配置中有启动优先级标识
	void FindPlugin();

	/// \brief 待废弃
	void StartPluginControl();

	/// \brief 更新配置文件 
	void UpdateConfigFile();
private slots:
	/// \brief 释放当前插件界面
	void ReleaseCurrentWidget();

	/// \brief 初始化当前插件界面
	/// \param[in] targetPlug 当前插件
	void InitCurrentWidget(const PluginInfo* targetPlug);
private:
	QFile m_logFile;										///< 日志文件
	QLibrary m_Loadlib;										///< 加载库
	QVector<PluginInfo> m_PluginConfig;						///< 所有插件信息
	QString m_CurrentloadedPlugName;         				///< 当前插件名称
	Allplugins m_pAllPlugins;								///< 待废弃
	QMap<QString, AbstractWidget*> m_mapAbstractWidget;		///< 插件或插件中的实例名称-抽象插件基类
	MessageThread* m_pMsgThread;							///< 消息线程
};

#endif // MAINFRAME_H
