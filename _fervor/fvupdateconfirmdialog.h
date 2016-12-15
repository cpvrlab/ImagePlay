#ifndef FVUPDATECONFIRMDIALOG_H
#define FVUPDATECONFIRMDIALOG_H

#include <QDialog>

namespace Ui {
class FvUpdateConfirmDialog;
}

class FvUpdateConfirmDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit FvUpdateConfirmDialog(QWidget *parent = 0);
	~FvUpdateConfirmDialog();

	// Update the current update proposal from FvUpdater
	bool UpdateWindowWithCurrentProposedUpdate();

	void closeEvent(QCloseEvent* event);

private:
	Ui::FvUpdateConfirmDialog* m_ui;
};

#endif // FVUPDATECONFIRMDIALOG_H
