#ifndef PLUGININFO
#define PLUGININFO


class PluginInfo
{
public:
	PluginInfo(const QString& targetName, const QString &targetPath, bool isStart) :
						m_str_name(targetName), m_str_path(targetPath), m_isStart(isStart) {}
	PluginInfo() {}
	QString m_str_name; 
	QString m_str_path;
	bool m_isStart;
};

#endif //PLUGININFO
