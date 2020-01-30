#ifndef PLUGININFO
#define PLUGININFO


class PluginInfo
{
public:
	PluginInfo(const QString& targetName, const QString &targetPath, const QString& targetTitle, 
						bool isStart, const QRect& targetRect) :
						m_str_name(targetName), m_str_path(targetPath), m_str_title(targetTitle), 
						m_isStart(isStart), m_rPosition(targetRect) {}
	PluginInfo() {}
	QString m_str_name; 
	QString m_str_path;
	QString m_str_title;
	bool m_isStart;
	QRect m_rPosition;
	class ChildProject{
	public:
		ChildProject(const QString& targetName, const QString& targetTitle, const QRect& targetRect) :
					m_ChildName(targetName), m_strTitle(targetTitle), m_rPosition(targetRect) {}
		ChildProject() {}
		QString m_ChildName;
		QString m_strTitle; 
		QRect m_rPosition;
	};
	QVector<ChildProject> m_VecChild;
};

#endif //PLUGININFO
