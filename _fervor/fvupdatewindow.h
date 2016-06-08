#ifndef FVUPDATEWINDOW_H
#define FVUPDATEWINDOW_H

#include <QWidget>
class QGraphicsScene;

namespace Ui {
class FvUpdateWindow;
}

class FvUpdateWindow : public QWidget
{
	Q_OBJECT
	
public:
	explicit FvUpdateWindow(QWidget *parent = 0);
	~FvUpdateWindow();

	// Update the current update proposal from FvUpdater
	bool UpdateWindowWithCurrentProposedUpdate();

	void closeEvent(QCloseEvent* event);

private:
	Ui::FvUpdateWindow*	m_ui;
	QGraphicsScene* m_appIconScene;

};

#endif // FVUPDATEWINDOW_H
