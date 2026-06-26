#pragma once

#include <QString>
#include <QStringList>

namespace CardUtils
{
QStringList parseCards(const QString &procAsoundCards);
QString parseDefaultCard(const QString &asoundrc);
} // namespace CardUtils
