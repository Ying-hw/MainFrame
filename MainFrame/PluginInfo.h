#ifndef PLUGININFO
#define PLUGININFO

extern QRect g_Rects;
class PluginInfo
{
public:
	PluginInfo() :m_bIsIvalid{ false }, m_OffSet{}{}
	QString m_str_name; 
	QString m_str_path;
	bool m_bIsIvalid;
	bool m_isStart;
	QRect m_rPosition;
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
			m_rPosition.setX(data.section(",", 0, 0).toInt());
			m_rPosition.setY(data.section(",", 1, 1).toInt());
			m_rPosition.setWidth(data.section(",", 2, 2).toInt());
			m_rPosition.setHeight(data.section(",", 3, 3).toInt());
			break;
		case 4:
			m_bIsIvalid = data.toInt();
			break;
		case 5:
			m_isStart = data.toInt();
			if (m_isStart)
				g_Rects = m_rPosition;
		default:
			m_OffSet = 0;
			break;
		}
	}
private:
	int m_OffSet;
};

#endif //PLUGININFO
