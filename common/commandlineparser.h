#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>

class CCommandLineParser
{
public:
    CCommandLineParser(const QCoreApplication& app);

    bool isEditAllowed() const;
    bool isLassoAllowed() const;

private:
    QCommandLineParser m_CmdLineParser;
    QCommandLineOption m_EditOption{"edit","Edit allowed: User can drag-move items."};
    QCommandLineOption m_LassoOption{"lasso","Lasso allowed: User can multi-select items using a rectangular lasso"};
};

#endif // COMMANDLINEPARSER_H
