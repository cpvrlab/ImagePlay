#include "fvupdatewindow.h"
#include "ui_fvupdatewindow.h"
#include "fvupdater.h"
#include "fvavailableupdate.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QNetworkRequest>


FvUpdateWindow::FvUpdateWindow(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::FvUpdateWindow)
{
	m_ui->setupUi(this);

	m_appIconScene = 0;

	// Delete on close
	setAttribute(Qt::WA_DeleteOnClose, true);

	// Set the "new version is available" string
	QString newVersString = m_ui->newVersionIsAvailableLabel->text().arg(QString::fromUtf8(FV_APP_NAME));
	m_ui->newVersionIsAvailableLabel->setText(newVersString);

	// Connect buttons
	connect(m_ui->installUpdateButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(InstallUpdate()));
	connect(m_ui->skipThisVersionButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(SkipUpdate()));
	connect(m_ui->remindMeLaterButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(RemindMeLater()));
}

FvUpdateWindow::~FvUpdateWindow()
{
    //m_ui->releaseNotesWebView->stop();
    //cancelDownloadReleaseNotes();
	delete m_ui;
}

bool FvUpdateWindow::UpdateWindowWithCurrentProposedUpdate()
{
	FvAvailableUpdate* proposedUpdate = FvUpdater::sharedUpdater()->GetProposedUpdate();
	if (! proposedUpdate) {
		return false;
	}

	QString downloadString = m_ui->wouldYouLikeToDownloadLabel->text()
			.arg(QString::fromUtf8(FV_APP_NAME), proposedUpdate->GetEnclosureVersion(), QString::fromUtf8(FV_APP_VERSION));
    m_ui->wouldYouLikeToDownloadLabel->setText(downloadString);

    m_ReleaseNotesURL = proposedUpdate->GetReleaseNotesLink();


    QString releaseNotesLinkString = m_ui->releaseNotesLabel->text()
            .arg(proposedUpdate->GetReleaseNotesLink().toString());
    m_ui->releaseNotesLabel->setText(releaseNotesLinkString);

    // startDownloadReleaseNotes(m_ReleaseNotesURL);

    //m_ui->textEdit->loadResource(QTextDocument::HtmlResource, m_ReleaseNotesURL);

	return true;
}

void FvUpdateWindow::closeEvent(QCloseEvent* event)
{
	FvUpdater::sharedUpdater()->updaterWindowWasClosed();
	event->accept();
}


void FvUpdateWindow::startDownloadReleaseNotes(QUrl url)
{
    m_ReleaseNotes.clear();

    //m_ui->textEdit->setText(QString("Downloading from: %1").arg(m_ReleaseNotesURL.toString()));

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/html");
    request.setHeader(QNetworkRequest::UserAgentHeader, QApplication::applicationName());
    request.setUrl(url);

    m_reply = m_qnam.get(request);

    connect(m_reply, SIGNAL(readyRead()), this, SLOT(httpReleaseNotesReadyRead()));
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(httpReleaseNotesUpdateDataReadProgress(qint64, qint64)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(httpReleaseNotesDownloadFinished()));
}

void FvUpdateWindow::cancelDownloadReleaseNotes()
{
    if (m_reply) {
        m_httpRequestAborted = true;
        m_reply->abort();
    }
}

void FvUpdateWindow::httpReleaseNotesReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    m_ReleaseNotes.append(m_reply->readAll());
}

void FvUpdateWindow::httpReleaseNotesUpdateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    Q_UNUSED(bytesRead);
    Q_UNUSED(totalBytes);

    if (m_httpRequestAborted) {
        return;
    }
}

void FvUpdateWindow::httpReleaseNotesDownloadFinished()
{
    if (m_httpRequestAborted) {
        m_reply->deleteLater();
        return;
    }
    QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (m_reply->error()) {

        // Error.
        //showErrorDialog(tr("ReleaseNotes download failed: %1.").arg(m_reply->errorString()), false);
        //m_ui->textEdit->append("Error: ");
        //m_ui->textEdit->append(m_reply->errorString());

    } else if (! redirectionTarget.isNull()) {
        QUrl newUrl = m_ReleaseNotesURL.resolved(redirectionTarget.toUrl());


        //m_ui->textEdit->append("redirectionTarget");
        //m_ui->textEdit->append(newUrl.toString());


        m_ReleaseNotesURL = newUrl;
        m_reply->deleteLater();

        startDownloadReleaseNotes(m_ReleaseNotesURL);
        return;

    } else {

        // Done.
        //m_ui->textEdit->setText(m_ReleaseNotes);

    }

    m_reply->deleteLater();
    m_reply = 0;
}
