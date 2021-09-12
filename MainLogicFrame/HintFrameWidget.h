#ifndef __HINTFRAMEWIDGET__
#define __HINTFRAMEWIDGET__

/*!
 * \file HintFrameWidget.h
 *
 * \brief 提示框类 
 *
 * \author yhw
 * \date 2021/09/12 15:08
 *
 * 
 */

#include "MainFrame_global.h"
#include "AbstractWidget.h"
#include "AbstractNetWork.h"

/*!
 * \class HintFrameWidget
 *
 * \brief 提示框类
 *
 * \author yhw
 * \date 2021年9月12日
 */
class MAINFRAME_EXPORT HintFrameWidget : public QWidget
{
	Q_OBJECT
	enum class HintType{ BLOCK, NOBLOCK };  ///< 风格
public:
	/// \brief 构造函数
	/// \param[in] strHint 提示文字 
	/// \param[in] parent 父窗口
	/// \param[in] type 默认NOBLOCK
	HintFrameWidget(QString strHint, AbstractWidget* parent, HintFrameWidget::HintType type = HintType::NOBLOCK);

	/// \brief 构造函数
	/// \param[in] strHint 提示文字 
	/// \param[in] originPoint 起始位置
	/// \param[in] parent 父窗口
	/// \param[in] type 默认NOBLOCK
	HintFrameWidget(QString strHint, QPoint originPoint, AbstractNetWork* parent, HintFrameWidget::HintType type = HintType::NOBLOCK);

	/// \brief 构造函数
	/// \param[in] strHint 提示文字 
	/// \param[in] originPoint 起始位置
	/// \param[in] type 默认NOBLOCK
	HintFrameWidget(QString strHint, QPoint originPoint, HintFrameWidget::HintType type = HintType::NOBLOCK);

	/// \brief 析构函数
	~HintFrameWidget();

	/// \brief 初始化提示框信息
	/// \param[in] strHint 提示文字
	void initBlockModel(const QString& strHint);

	/// \brief 初始化大小
	void initSize();

	/// \brief 显示事件
	/// 在窗口内存创建成功之后，显示之前
	/// \param[in] event 事件参数
	void showEvent(QShowEvent *event);

	/// \brief 绘图事件
	/// 在窗口变化的时候调用
	/// \param[in] event 事件参数
	void paintEvent(QPaintEvent* event);
private slots:
	/// \brief 关闭窗口
	void CloseHintWindow();
private:
	QString m_strHintText;  ///< 提示文字
	QTimer m_TimeClose;     ///< 定时器
	AbstractWidget* m_ParentWidget;   ///< 父窗口
};

#endif //__HINTFRAMEWIDGET__

