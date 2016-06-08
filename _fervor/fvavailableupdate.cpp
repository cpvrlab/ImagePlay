#include "fvavailableupdate.h"

FvAvailableUpdate::FvAvailableUpdate(QObject *parent) :
	QObject(parent)
{
	// noop
}

QString FvAvailableUpdate::GetTitle()
{
	return m_title;
}

void FvAvailableUpdate::SetTitle(QString title)
{
	m_title = title;
}

QUrl FvAvailableUpdate::GetReleaseNotesLink()
{
	return m_releaseNotesLink;
}

void FvAvailableUpdate::SetReleaseNotesLink(QUrl releaseNotesLink)
{
	m_releaseNotesLink = releaseNotesLink;
}

void FvAvailableUpdate::SetReleaseNotesLink(QString releaseNotesLink)
{
	SetReleaseNotesLink(QUrl(releaseNotesLink));
}

QString FvAvailableUpdate::GetPubDate()
{
	return m_pubDate;
}

void FvAvailableUpdate::SetPubDate(QString pubDate)
{
	m_pubDate = pubDate;
}

QUrl FvAvailableUpdate::GetEnclosureUrl()
{
	return m_enclosureUrl;
}

void FvAvailableUpdate::SetEnclosureUrl(QUrl enclosureUrl)
{
	m_enclosureUrl = enclosureUrl;
}

void FvAvailableUpdate::SetEnclosureUrl(QString enclosureUrl)
{
	SetEnclosureUrl(QUrl(enclosureUrl));
}

QString FvAvailableUpdate::GetEnclosureVersion()
{
	return m_enclosureVersion;
}

void FvAvailableUpdate::SetEnclosureVersion(QString enclosureVersion)
{
	m_enclosureVersion = enclosureVersion;
}

QString FvAvailableUpdate::GetEnclosurePlatform()
{
	return m_enclosurePlatform;
}

void FvAvailableUpdate::SetEnclosurePlatform(QString enclosurePlatform)
{
	m_enclosurePlatform = enclosurePlatform;
}

unsigned long FvAvailableUpdate::GetEnclosureLength()
{
	return m_enclosureLength;
}

void FvAvailableUpdate::SetEnclosureLength(unsigned long enclosureLength)
{
	m_enclosureLength = enclosureLength;
}

QString FvAvailableUpdate::GetEnclosureType()
{
	return m_enclosureType;
}

void FvAvailableUpdate::SetEnclosureType(QString enclosureType)
{
	m_enclosureType = enclosureType;
}
