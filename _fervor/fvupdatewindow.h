#ifndef FVUPDATEWINDOW_H
#define FVUPDATEWINDOW_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
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

    //
    // HTTP release notes fetcher infrastructure
    //
    QUrl m_ReleaseNotesURL;					// Feed URL that will be fetched
    QNetworkAccessManager m_qnam;
    QNetworkReply* m_reply;
    int m_httpGetId;
    bool m_httpRequestAborted;

    QString m_ReleaseNotes;

    void startDownloadReleaseNotes(QUrl url);	// Start downloading release notes
    void cancelDownloadReleaseNotes();			// Stop downloading release notes

private slots:
    void httpReleaseNotesReadyRead();
    void httpReleaseNotesUpdateDataReadProgress(qint64 bytesRead,
                                        qint64 totalBytes);
    void httpReleaseNotesDownloadFinished();

private:
	Ui::FvUpdateWindow*	m_ui;
	QGraphicsScene* m_appIconScene;

};

#endif // FVUPDATEWINDOW_H
