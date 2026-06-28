#include "cardutils.h"

#include <QFile>
#include <QProcess>
#include <QRegularExpression>
#include <QStandardPaths>

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

bool isPipewireRunning()
{
    if (QStandardPaths::findExecutable(QStringLiteral("pactl")).isEmpty())
        return false;
    const QString runtimeDir = QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    return QFile::exists(runtimeDir + QStringLiteral("/pipewire-0"));
}

QList<PipeWireSink> parsePipewireSinks(const QString &paListSinksOutput)
{
    QList<PipeWireSink> result;
    PipeWireSink current;
    bool inSink = false;

    for (const QString &line : paListSinksOutput.split(u'\n')) {
        if (line.startsWith(QStringLiteral("Sink #"))) {
            if (inSink && !current.name.isEmpty())
                result << current;
            current = {};
            inSink = true;
        } else if (inSink) {
            const QString trimmed = line.trimmed();
            if (trimmed.startsWith(QStringLiteral("Name:")))
                current.name = trimmed.mid(5).trimmed();
            else if (trimmed.startsWith(QStringLiteral("Description:")))
                current.description = trimmed.mid(12).trimmed();
        }
    }
    if (inSink && !current.name.isEmpty())
        result << current;
    return result;
}

} // namespace CardUtils
