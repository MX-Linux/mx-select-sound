#include <QTest>
#include "cardutils.h"

class TestParsing : public QObject
{
    Q_OBJECT

private slots:
    void parseCards_empty();
    void parseCards_single();
    void parseCards_multiple();
    void parseCards_skipsDescriptionLines();
    void parseDefaultCard_found();
    void parseDefaultCard_notFound();
    void parseDefaultCard_returnsFirst();
    void parseDefaultCard_ignoresCtl();
    void parseDefaultCard_toleratesLeadingWhitespace();
};

void TestParsing::parseCards_empty()
{
    QVERIFY(CardUtils::parseCards(QString()).isEmpty());
    QVERIFY(CardUtils::parseCards(QStringLiteral("no cards here\n")).isEmpty());
}

void TestParsing::parseCards_single()
{
    const QString input = QStringLiteral(
        " 0 [PCH            ]: HDA-Intel - HDA Intel PCH\n"
        "                      HDA Intel PCH at 0xf7220000 irq 44\n");
    const QStringList result = CardUtils::parseCards(input);
    QCOMPARE(result.size(), 1);
    QCOMPARE(result.at(0), QStringLiteral("PCH: HDA-Intel - HDA Intel PCH"));
}

void TestParsing::parseCards_multiple()
{
    const QString input = QStringLiteral(
        " 0 [PCH            ]: HDA-Intel - HDA Intel PCH\n"
        "                      HDA Intel PCH at 0xf7220000 irq 44\n"
        " 1 [NVidia         ]: HDA-Intel - HDA NVidia\n"
        "                      HDA NVidia at 0xf7080000 irq 17\n");
    const QStringList result = CardUtils::parseCards(input);
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.at(0), QStringLiteral("PCH: HDA-Intel - HDA Intel PCH"));
    QCOMPARE(result.at(1), QStringLiteral("NVidia: HDA-Intel - HDA NVidia"));
}

void TestParsing::parseCards_skipsDescriptionLines()
{
    const QString input = QStringLiteral(
        " 0 [USB            ]: USB-Audio - USB Audio Device\n"
        "                      Generic USB Audio Device at usb-0000:00:14.0-1\n");
    QCOMPARE(CardUtils::parseCards(input).size(), 1);
}

void TestParsing::parseDefaultCard_found()
{
    const QString input = QStringLiteral(
        "defaults.pcm.!card PCH\n"
        "defaults.ctl.!card PCH\n");
    QCOMPARE(CardUtils::parseDefaultCard(input), QStringLiteral("PCH"));
}

void TestParsing::parseDefaultCard_notFound()
{
    QVERIFY(CardUtils::parseDefaultCard(QString()).isEmpty());
    QVERIFY(CardUtils::parseDefaultCard(QStringLiteral("# empty\n")).isEmpty());
}

void TestParsing::parseDefaultCard_returnsFirst()
{
    const QString input = QStringLiteral(
        "defaults.pcm.!card PCH\n"
        "defaults.pcm.!card NVidia\n");
    QCOMPARE(CardUtils::parseDefaultCard(input), QStringLiteral("PCH"));
}

void TestParsing::parseDefaultCard_ignoresCtl()
{
    QVERIFY(CardUtils::parseDefaultCard(QStringLiteral("defaults.ctl.!card PCH\n")).isEmpty());
}

void TestParsing::parseDefaultCard_toleratesLeadingWhitespace()
{
    QCOMPARE(CardUtils::parseDefaultCard(QStringLiteral("  defaults.pcm.!card NVidia\n")),
             QStringLiteral("NVidia"));
}

QTEST_MAIN(TestParsing)
#include "test_parsing.moc"
