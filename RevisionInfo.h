#ifndef REVISIONINFO_H
#define REVISIONINFO_H

#include <QDateTime>
#include <QMap>
#include <QStringList>

struct Revision
{
    enum Change {
        Added, Removed, Modified
    };

    int         name;
    QString     comment;
    QDateTime   timestamp;
    QString     user;

    QMap< Change, QStringList > modifications;
};

class RevisionHistory
{
public:
    RevisionHistory();

protected:
    QList<Revision> m_revisionList;
};

#endif // REVISIONINFO_H
