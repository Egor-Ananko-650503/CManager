#include <QtTest>

#include "../CManager/src/filemodel.h"

class logic_tests : public QObject
{
    Q_OBJECT

public:
    logic_tests();
    ~logic_tests();

    FileModel model;
    Files nonEmptyList = {
        path("C:\\"),
        path("C:\\Users"),
        path("D:\\"),
        path("D:\\Games\\Battle.net"),
        path("D:\\Films")
    };
    Files emptyList;

private slots:
    void initTestCase();
    void test_getContent();
    void test_contentCount();
    void test_formatSizeByThousands();
    void cleanupTestCase();
};

logic_tests::logic_tests()
{
}

logic_tests::~logic_tests()
{
}

void logic_tests::initTestCase()
{
    model.resetModel(nonEmptyList);
}

void logic_tests::test_getContent()
{
    for (int i = 0; i < nonEmptyList.count(); ++i)
        QCOMPARE(model.getContent(i).wstring(), nonEmptyList.at(i).wstring());
}

void logic_tests::test_contentCount()
{
    QCOMPARE(model.contentCount(), nonEmptyList.count());
    model.resetModel(emptyList);
    QCOMPARE(model.contentCount(), emptyList.count());
    model.resetModel(nonEmptyList);
}

void logic_tests::test_formatSizeByThousands()
{
    QCOMPARE(model.formatSizeByThousands("12345678"), "12 345 678");
    QCOMPARE(model.formatSizeByThousands("1234"), "1 234");
    QCOMPARE(model.formatSizeByThousands("123"), "123");
    QCOMPARE(model.formatSizeByThousands("1"), "1");
}

void logic_tests::cleanupTestCase()
{
    model.resetModel(emptyList);
}

QTEST_APPLESS_MAIN(logic_tests)

#include "tst_logic_tests.moc"
