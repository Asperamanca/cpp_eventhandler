#include "commandlineparser.h"

CCommandLineParser::CCommandLineParser(const QCoreApplication& app)
{
    m_CmdLineParser.addOption(m_EditOption);
    m_CmdLineParser.addOption(m_LassoOption);
    m_CmdLineParser.addHelpOption();
    m_CmdLineParser.process(app);
}

bool CCommandLineParser::isEditAllowed() const
{
    return m_CmdLineParser.isSet(m_EditOption);
}

bool CCommandLineParser::isLassoAllowed() const
{
    return m_CmdLineParser.isSet(m_LassoOption);
}
