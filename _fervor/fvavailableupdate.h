#ifndef FVAVAILABLEUPDATE_H
#define FVAVAILABLEUPDATE_H

#include <QObject>
#include <QUrl>

class FvAvailableUpdate : public QObject
{
	Q_OBJECT
public:
	explicit FvAvailableUpdate(QObject *parent = 0);

	QString GetTitle();
	void SetTitle(QString title);

	QUrl GetReleaseNotesLink();
	void SetReleaseNotesLink(QUrl releaseNotesLink);
	void SetReleaseNotesLink(QString releaseNotesLink);

	QString GetPubDate();
	void SetPubDate(QString pubDate);

	QUrl GetEnclosureUrl();
	void SetEnclosureUrl(QUrl enclosureUrl);
	void SetEnclosureUrl(QString enclosureUrl);

	QString GetEnclosureVersion();
	void SetEnclosureVersion(QString enclosureVersion);

	QString GetEnclosurePlatform();
	void SetEnclosurePlatform(QString enclosurePlatform);

	unsigned long GetEnclosureLength();
	void SetEnclosureLength(unsigned long enclosureLength);

	QString GetEnclosureType();
	void SetEnclosureType(QString enclosureType);

private:
	QString m_title;
	QUrl m_releaseNotesLink;
	QString m_pubDate;
	QUrl m_enclosureUrl;
	QString m_enclosureVersion;
	QString m_enclosurePlatform;
	unsigned long m_enclosureLength;
	QString m_enclosureType;

};

#endif // FVAVAILABLEUPDATE_H
