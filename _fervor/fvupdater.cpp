#include "fvupdater.h"
#include "fvupdatewindow.h"
#include "fvupdateconfirmdialog.h"
#include "fvplatform.h"
#include "fvignoredversions.h"
#include "fvavailableupdate.h"
#include <QApplication>
#include <QtNetwork>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDebug>

#ifndef FV_APP_NAME
#	error "FV_APP_NAME is undefined (must have been defined by Fervor.pri)"
#endif
#ifndef FV_APP_VERSION
#	error "FV_APP_VERSION is undefined (must have been defined by Fervor.pri)"
#endif


#ifdef FV_DEBUG
	// Unit tests
#	include "fvversioncomparatortest.h"
#endif


FvUpdater* FvUpdater::m_Instance = 0;


FvUpdater* FvUpdater::sharedUpdater()
{
	static QMutex mutex;
	if (! m_Instance) {
		mutex.lock();

		if (! m_Instance) {
			m_Instance = new FvUpdater;
		}

		mutex.unlock();
	}

	return m_Instance;
}

void FvUpdater::drop()
{
	static QMutex mutex;
	mutex.lock();
	delete m_Instance;
	m_Instance = 0;
	mutex.unlock();
}

FvUpdater::FvUpdater() : QObject(0)
{
	m_reply = 0;
	m_updaterWindow = 0;
	m_updateConfirmationDialog = 0;
	m_proposedUpdate = 0;

	// Translation mechanism
	installTranslator();

#ifdef FV_DEBUG
	// Unit tests
	FvVersionComparatorTest* test = new FvVersionComparatorTest();
	test->runAll();
	delete test;
#endif

}

FvUpdater::~FvUpdater()
{
	if (m_proposedUpdate) {
		delete m_proposedUpdate;
		m_proposedUpdate = 0;
	}

	hideUpdateConfirmationDialog();
	hideUpdaterWindow();
}

void FvUpdater::installTranslator()
{
	QTranslator translator;
	QString locale = QLocale::system().name();
	translator.load(QString("fervor_") + locale);
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	qApp->installTranslator(&translator);
}

void FvUpdater::showUpdaterWindowUpdatedWithCurrentUpdateProposal()
{
	// Destroy window if already exists
	hideUpdaterWindow();

	// Create a new window
	m_updaterWindow = new FvUpdateWindow();
	m_updaterWindow->UpdateWindowWithCurrentProposedUpdate();
	m_updaterWindow->show();
}

void FvUpdater::hideUpdaterWindow()
{
	if (m_updaterWindow) {
		if (! m_updaterWindow->close()) {
			qWarning() << "Update window didn't close, leaking memory from now on";
		}

		// not deleting because of Qt::WA_DeleteOnClose

		m_updaterWindow = 0;
	}
}

void FvUpdater::updaterWindowWasClosed()
{
	// (Re-)nullify a pointer to a destroyed QWidget or you're going to have a bad time.
	m_updaterWindow = 0;
}


void FvUpdater::showUpdateConfirmationDialogUpdatedWithCurrentUpdateProposal()
{
	// Destroy dialog if already exists
	hideUpdateConfirmationDialog();

	// Create a new window
	m_updateConfirmationDialog = new FvUpdateConfirmDialog();
	m_updateConfirmationDialog->UpdateWindowWithCurrentProposedUpdate();
	m_updateConfirmationDialog->show();
}

void FvUpdater::hideUpdateConfirmationDialog()
{
	if (m_updateConfirmationDialog) {
		if (! m_updateConfirmationDialog->close()) {
			qWarning() << "Update confirmation dialog didn't close, leaking memory from now on";
		}

		// not deleting because of Qt::WA_DeleteOnClose

		m_updateConfirmationDialog = 0;
	}
}

void FvUpdater::updateConfirmationDialogWasClosed()
{
	// (Re-)nullify a pointer to a destroyed QWidget or you're going to have a bad time.
	m_updateConfirmationDialog = 0;
}


void FvUpdater::SetFeedURL(QUrl feedURL)
{
	m_feedURL = feedURL;
}

void FvUpdater::SetFeedURL(QString feedURL)
{
	SetFeedURL(QUrl(feedURL));
}

QString FvUpdater::GetFeedURL()
{
	return m_feedURL.toString();
}

FvAvailableUpdate* FvUpdater::GetProposedUpdate()
{
	return m_proposedUpdate;
}


void FvUpdater::InstallUpdate()
{
	qDebug() << "Install update";

	showUpdateConfirmationDialogUpdatedWithCurrentUpdateProposal();
}

void FvUpdater::SkipUpdate()
{
	qDebug() << "Skip update";

	FvAvailableUpdate* proposedUpdate = GetProposedUpdate();
	if (! proposedUpdate) {
		qWarning() << "Proposed update is NULL (shouldn't be at this point)";
		return;
	}

	// Start ignoring this particular version
	FVIgnoredVersions::IgnoreVersion(proposedUpdate->GetEnclosureVersion());

	hideUpdaterWindow();
	hideUpdateConfirmationDialog();	// if any; shouldn't be shown at this point, but who knows
}

void FvUpdater::RemindMeLater()
{
	qDebug() << "Remind me later";

	hideUpdaterWindow();
	hideUpdateConfirmationDialog();	// if any; shouldn't be shown at this point, but who knows
}

void FvUpdater::UpdateInstallationConfirmed()
{
	qDebug() << "Confirm update installation";

	FvAvailableUpdate* proposedUpdate = GetProposedUpdate();
	if (! proposedUpdate) {
		qWarning() << "Proposed update is NULL (shouldn't be at this point)";
		return;
	}

	// Open a link
	if (! QDesktopServices::openUrl(proposedUpdate->GetEnclosureUrl())) {
		showErrorDialog(tr("Unable to open this link in a browser. Please do it manually."), true);
		return;
	}

	hideUpdaterWindow();
	hideUpdateConfirmationDialog();
}

void FvUpdater::UpdateInstallationNotConfirmed()
{
	qDebug() << "Do not confirm update installation";

	hideUpdateConfirmationDialog();	// if any; shouldn't be shown at this point, but who knows
	// leave the "update proposal window" inact
}


bool FvUpdater::CheckForUpdates(bool silentAsMuchAsItCouldGet)
{
	if (m_feedURL.isEmpty()) {
		qCritical() << "Please set feed URL via setFeedURL() before calling CheckForUpdates().";
		return false;
	}

	m_silentAsMuchAsItCouldGet = silentAsMuchAsItCouldGet;

	// Check if application's organization name and domain are set, fail otherwise
	// (nowhere to store QSettings to)
	if (QApplication::organizationName().isEmpty()) {
		qCritical() << "QApplication::organizationName is not set. Please do that.";
		return false;
	}
	if (QApplication::organizationDomain().isEmpty()) {
		qCritical() << "QApplication::organizationDomain is not set. Please do that.";
		return false;
	}

	// Set application name / version is not set yet
	if (QApplication::applicationName().isEmpty()) {
		QString appName = QString::fromUtf8(FV_APP_NAME);
		qWarning() << "QApplication::applicationName is not set, setting it to '" << appName << "'";
		QApplication::setApplicationName(appName);
	}
	if (QApplication::applicationVersion().isEmpty()) {
		QString appVersion = QString::fromUtf8(FV_APP_VERSION);
		qWarning() << "QApplication::applicationVersion is not set, setting it to '" << appVersion << "'";
		QApplication::setApplicationVersion(appVersion);
	}

	cancelDownloadFeed();
	m_httpRequestAborted = false;
	startDownloadFeed(m_feedURL);

	return true;
}

bool FvUpdater::CheckForUpdatesSilent()
{
	return CheckForUpdates(true);
}

bool FvUpdater::CheckForUpdatesNotSilent()
{
	return CheckForUpdates(false);
}


void FvUpdater::startDownloadFeed(QUrl url)
{
	m_xml.clear();

	QNetworkRequest request;
 	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
 	request.setHeader(QNetworkRequest::UserAgentHeader, QApplication::applicationName());
 	request.setUrl(url);

	m_reply = m_qnam.get(request);

	connect(m_reply, SIGNAL(readyRead()), this, SLOT(httpFeedReadyRead()));
	connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(httpFeedUpdateDataReadProgress(qint64, qint64)));
	connect(m_reply, SIGNAL(finished()), this, SLOT(httpFeedDownloadFinished()));
}

void FvUpdater::cancelDownloadFeed()
{
	if (m_reply) {
		m_httpRequestAborted = true;
		m_reply->abort();
	}
}

void FvUpdater::httpFeedReadyRead()
{
	// this slot gets called every time the QNetworkReply has new data.
	// We read all of its new data and write it into the file.
	// That way we use less RAM than when reading it at the finished()
	// signal of the QNetworkReply
	m_xml.addData(m_reply->readAll());
}

void FvUpdater::httpFeedUpdateDataReadProgress(qint64 bytesRead,
											   qint64 totalBytes)
{
	Q_UNUSED(bytesRead);
	Q_UNUSED(totalBytes);

	if (m_httpRequestAborted) {
		return;
	}
}

void FvUpdater::httpFeedDownloadFinished()
{
	if (m_httpRequestAborted) {
		m_reply->deleteLater();
		return;
	}

	QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (m_reply->error()) {

		// Error.
		showErrorDialog(tr("Feed download failed: %1.").arg(m_reply->errorString()), false);

	} else if (! redirectionTarget.isNull()) {
		QUrl newUrl = m_feedURL.resolved(redirectionTarget.toUrl());

		m_feedURL = newUrl;
		m_reply->deleteLater();

		startDownloadFeed(m_feedURL);
		return;

	} else {

		// Done.
		xmlParseFeed();

	}

	m_reply->deleteLater();
	m_reply = 0;
}

bool FvUpdater::xmlParseFeed()
{
	QString currentTag, currentQualifiedTag;

	QString xmlTitle, xmlLink, xmlReleaseNotesLink, xmlPubDate, xmlEnclosureUrl,
			xmlEnclosureVersion, xmlEnclosurePlatform, xmlEnclosureType;
	unsigned long xmlEnclosureLength = 0;

	// Parse
	while (! m_xml.atEnd()) {

		m_xml.readNext();

		if (m_xml.isStartElement()) {

			currentTag = m_xml.name().toString();
			currentQualifiedTag = m_xml.qualifiedName().toString();

			if (m_xml.name() == "item") {

				xmlTitle.clear();
				xmlLink.clear();
				xmlReleaseNotesLink.clear();
				xmlPubDate.clear();
				xmlEnclosureUrl.clear();
				xmlEnclosureVersion.clear();
				xmlEnclosurePlatform.clear();
				xmlEnclosureLength = 0;
				xmlEnclosureType.clear();

			} else if (m_xml.name() == "enclosure") {

				QXmlStreamAttributes attribs = m_xml.attributes();

				if (attribs.hasAttribute("fervor:platform")) {

					if (FvPlatform::CurrentlyRunningOnPlatform(attribs.value("fervor:platform").toString().trimmed())) {

						xmlEnclosurePlatform = attribs.value("fervor:platform").toString().trimmed();

						if (attribs.hasAttribute("url")) {
							xmlEnclosureUrl = attribs.value("url").toString().trimmed();
						} else {
							xmlEnclosureUrl = "";
						}

                        // First check for Sparkle's version, then overwrite with Fervor's version (if any)
                        if (attribs.hasAttribute("sparkle:version")) {
                            QString candidateVersion = attribs.value("sparkle:version").toString().trimmed();
                            if (! candidateVersion.isEmpty()) {
                                xmlEnclosureVersion = candidateVersion;
                            }
                        }
                        if (attribs.hasAttribute("fervor:version")) {
                            QString candidateVersion = attribs.value("fervor:version").toString().trimmed();
                            if (! candidateVersion.isEmpty()) {
                                xmlEnclosureVersion = candidateVersion;
                            }
                        }

						if (attribs.hasAttribute("length")) {
							xmlEnclosureLength = attribs.value("length").toString().toLong();
						} else {
							xmlEnclosureLength = 0;
						}
						if (attribs.hasAttribute("type")) {
							xmlEnclosureType = attribs.value("type").toString().trimmed();
						} else {
							xmlEnclosureType = "";
						}

					}

				}

			}

		} else if (m_xml.isEndElement()) {

			if (m_xml.name() == "item") {

				// That's it - we have analyzed a single <item> and we'll stop
				// here (because the topmost is the most recent one, and thus
				// the newest version.

				return searchDownloadedFeedForUpdates(xmlTitle,
													  xmlLink,
													  xmlReleaseNotesLink,
													  xmlPubDate,
													  xmlEnclosureUrl,
													  xmlEnclosureVersion,
													  xmlEnclosurePlatform,
													  xmlEnclosureLength,
													  xmlEnclosureType);

			}

		} else if (m_xml.isCharacters() && ! m_xml.isWhitespace()) {

			if (currentTag == "title") {
				xmlTitle += m_xml.text().toString().trimmed();

			} else if (currentTag == "link") {
				xmlLink += m_xml.text().toString().trimmed();

			} else if (currentQualifiedTag == "sparkle:releaseNotesLink") {
				xmlReleaseNotesLink += m_xml.text().toString().trimmed();

			} else if (currentTag == "pubDate") {
				xmlPubDate += m_xml.text().toString().trimmed();

			}

		}

		if (m_xml.error() && m_xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {

			showErrorDialog(tr("Feed parsing failed: %1 %2.").arg(QString::number(m_xml.lineNumber()), m_xml.errorString()), false);
			return false;

		}
	}

    // No updates were found if we're at this point
    // (not a single <item> element found)
    showInformationDialog(tr("No updates were found."), false);

	return false;
}


bool FvUpdater::searchDownloadedFeedForUpdates(QString xmlTitle,
											   QString xmlLink,
											   QString xmlReleaseNotesLink,
											   QString xmlPubDate,
											   QString xmlEnclosureUrl,
											   QString xmlEnclosureVersion,
											   QString xmlEnclosurePlatform,
											   unsigned long xmlEnclosureLength,
											   QString xmlEnclosureType)
{
    qDebug() << "Title:" << xmlTitle;
    qDebug() << "Link:" << xmlLink;
    qDebug() << "Release notes link:" << xmlReleaseNotesLink;
    qDebug() << "Pub. date:" << xmlPubDate;
    qDebug() << "Enclosure URL:" << xmlEnclosureUrl;
    qDebug() << "Enclosure version:" << xmlEnclosureVersion;
    qDebug() << "Enclosure platform:" << xmlEnclosurePlatform;
    qDebug() << "Enclosure length:" << xmlEnclosureLength;
    qDebug() << "Enclosure type:" << xmlEnclosureType;

	// Validate
	if (xmlReleaseNotesLink.isEmpty()) {
		if (xmlLink.isEmpty()) {
			showErrorDialog(tr("Feed error: \"release notes\" link is empty"), false);
			return false;
		} else {
			xmlReleaseNotesLink = xmlLink;
		}
	} else {
		xmlLink = xmlReleaseNotesLink;
	}
	if (! (xmlLink.startsWith("http://") || xmlLink.startsWith("https://"))) {
		showErrorDialog(tr("Feed error: invalid \"release notes\" link"), false);
		return false;
	}
	if (xmlEnclosureUrl.isEmpty() || xmlEnclosureVersion.isEmpty() || xmlEnclosurePlatform.isEmpty()) {
        showErrorDialog(tr("Feed error: invalid \"enclosure\" with the download link"), false);
		return false;
	}

	// Relevant version?
	if (FVIgnoredVersions::VersionIsIgnored(xmlEnclosureVersion)) {
		qDebug() << "Version '" << xmlEnclosureVersion << "' is ignored, too old or something like that.";

		showInformationDialog(tr("No updates were found."), false);

		return true;	// Things have succeeded when you think of it.
	}


	//
	// Success! At this point, we have found an update that can be proposed
	// to the user.
	//

	if (m_proposedUpdate) {
		delete m_proposedUpdate; m_proposedUpdate = 0;
	}
	m_proposedUpdate = new FvAvailableUpdate();
	m_proposedUpdate->SetTitle(xmlTitle);
	m_proposedUpdate->SetReleaseNotesLink(xmlReleaseNotesLink);
	m_proposedUpdate->SetPubDate(xmlPubDate);
	m_proposedUpdate->SetEnclosureUrl(xmlEnclosureUrl);
	m_proposedUpdate->SetEnclosureVersion(xmlEnclosureVersion);
	m_proposedUpdate->SetEnclosurePlatform(xmlEnclosurePlatform);
	m_proposedUpdate->SetEnclosureLength(xmlEnclosureLength);
	m_proposedUpdate->SetEnclosureType(xmlEnclosureType);

	// Show "look, there's an update" window
	showUpdaterWindowUpdatedWithCurrentUpdateProposal();

	return true;
}


void FvUpdater::showErrorDialog(QString message, bool showEvenInSilentMode)
{
	if (m_silentAsMuchAsItCouldGet) {
		if (! showEvenInSilentMode) {
			// Don't show errors in the silent mode
			return;
		}
	}

	QMessageBox dlFailedMsgBox;
	dlFailedMsgBox.setIcon(QMessageBox::Critical);
	dlFailedMsgBox.setText(tr("Error"));
	dlFailedMsgBox.setInformativeText(message);
	dlFailedMsgBox.exec();
}

void FvUpdater::showInformationDialog(QString message, bool showEvenInSilentMode)
{
	if (m_silentAsMuchAsItCouldGet) {
		if (! showEvenInSilentMode) {
			// Don't show information dialogs in the silent mode
			return;
		}
	}

	QMessageBox dlInformationMsgBox;
	dlInformationMsgBox.setIcon(QMessageBox::Information);
	dlInformationMsgBox.setText(tr("Information"));
	dlInformationMsgBox.setInformativeText(message);
	dlInformationMsgBox.exec();
}
