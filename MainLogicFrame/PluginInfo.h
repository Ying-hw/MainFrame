#ifndef PLUGININFO
#define PLUGININFO


class PluginInfo
{
public:
	PluginInfo(const QString& targetName, const QString &targetPath, bool isStart) :
		m_str_name(targetName), m_str_path(targetPath), m_isStart(isStart), m_OffSet(0) {}
	PluginInfo() : m_OffSet(0) {}
	QString m_str_name; 
	QString m_str_path;
	bool m_isStart;
public:
	void operator<< (const QString& data) {
		m_OffSet++;
		switch (m_OffSet) {
		case 1:
			m_str_name = data;
			break;
		case 2:
			m_str_path = data;
			break;
		case 3:
			m_isStart = data.toInt();
		default:
			m_OffSet = 0;
			break;
		}
	}
private:
	int m_OffSet;
};

#endif //PLUGININFO
