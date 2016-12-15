#ifndef FVPLATFORM_H
#define FVPLATFORM_H

#include <QObject>

class FvPlatform : public QObject
{
	Q_OBJECT

public:
	static bool CurrentlyRunningOnPlatform(QString platform);

private:
	explicit FvPlatform(QObject *parent = 0);
		
};

#endif // FVPLATFORM_H
