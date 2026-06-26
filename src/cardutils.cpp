#include "cardutils.h"

#include <QRegularExpression>

namespace CardUtils
{

QStringList parseCards(const QString &procAsoundCards)
{
    QStringList result;
    static const QRegularExpression re(QStringLiteral(R"(^\s*\d+\s+\[(\S+)\s*\]:\s*(.+)$)"),
                                       QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator it = re.globalMatch(procAsoundCards);
    while (it.hasNext()) {
        const QRegularExpressionMatch match = it.next();
        result << match.captured(1) + QStringLiteral(": ") + match.captured(2).trimmed();
    }
    return result;
}

QString parseDefaultCard(const QString &asoundrc)
{
    static const QRegularExpression re(QStringLiteral(R"(^\s*defaults\.pcm\.!card\s+(\S+))"),
                                       QRegularExpression::MultilineOption);
    const QRegularExpressionMatch match = re.match(asoundrc);
    return match.hasMatch() ? match.captured(1) : QString();
}

} // namespace CardUtils
