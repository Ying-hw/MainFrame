#ifndef __INITIATIVEMESSAGE__
#define __INITIATIVEMESSAGE__
#include "AbstractWidget.h"
#include "MessageTemplate.h"


class MessageThread : public QThread
{
	Q_OBJECT
public:
	MessageThread(QObject* parent = 0);
	~MessageThread();
	void run();
	void pushinQueue(AbstractWidget* pAbstractW, CommonTemplate::InitType* type);
signals:
	void Message(AbstractWidget* pAbstractW);
public slots:
	void ExcuteOnMessage(AbstractWidget* pAbstractW);
private:
	bool m_isRuning;
	QQueue<AbstractWidget*> m_queue;
	QWaitCondition m_waitMutex;
	QMutex m_Mutex;
	CommonTemplate::InitType* m_pTypes;
};

#endif

