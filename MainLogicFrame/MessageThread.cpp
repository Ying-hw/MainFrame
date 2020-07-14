#include "stdafx.h"
#include "MessageThread.h"

MessageThread::MessageThread(QObject* parent /*= 0*/) : QThread(parent), m_isRuning(true), m_pTypes(NULL)
{
	m_Mutex.lock();
	connect(this, SIGNAL(Message(AbstractWidget *)), this, SLOT(ExcuteOnMessage(AbstractWidget *)));
}

MessageThread::~MessageThread()
{
	m_isRuning = false;
	m_waitMutex.wakeOne();
	deleteLater();
}

void MessageThread::run()
{
	while (m_isRuning)
		if (m_queue.isEmpty())
			m_waitMutex.wait(&m_Mutex);
		else
			while (!m_queue.isEmpty())
				emit Message(m_queue.dequeue());
}

void MessageThread::pushinQueue(AbstractWidget* pAbstractW, CommonTemplate::InitType* type)
{
	while (!m_queue.isEmpty());
	m_pTypes = type;
	m_queue.enqueue(pAbstractW);
	m_waitMutex.wakeOne();
}

void MessageThread::ExcuteOnMessage(AbstractWidget* pAbstractW)
{
	pAbstractW->OnMessage();
	//Çå¿ÕÊý¾Ý
	for (int i = 0; (m_pTypes->m_Params + i)->GetParam().m_param != nullptr;i++)
	{
		(m_pTypes->m_Params + i)->GetParam().SetType(nullptr);
	}
}
