#include <QString>
#include <QtTest>

#include "IPLImage.h"
#include "IPLColor.h"

class IPLTests : public QObject
{
    Q_OBJECT

public:
    IPLTests();

private Q_SLOTS:
    void testImageGrayscale();
    void testImageColor();

    void testColor();
};

IPLTests::IPLTests()
{

}

void IPLTests::testImageGrayscale()
{
    // init
    IPLImage* image = new IPLImage(IPL_IMAGE_GRAYSCALE, 1024, 512);
    QCOMPARE(image->width(), 1024);
    QCOMPARE(image->height(), 512);
    QCOMPARE(image->type(), IPL_IMAGE_GRAYSCALE);
    QCOMPARE(image->getNumberOfPlanes(), 1);

    // fill color
    QCOMPARE(image->plane(0)->p(100,100), 0.0f);
    image->fillColor(0.23f);
    QCOMPARE(image->plane(0)->p(100,100), 0.23f);

    // delete
    delete image;
}

void IPLTests::testImageColor()
{
    // init
    IPLImage* image = new IPLImage(IPL_IMAGE_COLOR, 1024, 512);
    QCOMPARE(image->width(), 1024);
    QCOMPARE(image->height(), 512);
    QCOMPARE(image->type(), IPL_IMAGE_COLOR);
    QCOMPARE(image->getNumberOfPlanes(), 3);

    // fill color
    QCOMPARE(image->plane(0)->p(100,100), 0.0f);
    QCOMPARE(image->plane(1)->p(100,100), 0.0f);
    QCOMPARE(image->plane(2)->p(100,100), 0.0f);
    image->fillColor(0.23f);
    QCOMPARE(image->plane(0)->p(100,100), 0.23f);
    QCOMPARE(image->plane(1)->p(100,100), 0.23f);
    QCOMPARE(image->plane(2)->p(100,100), 0.23f);

    // delete
    delete image;
}

void IPLTests::testColor()
{
    IPLColor color;
    QCOMPARE(color.red(), 0.0f);
    QCOMPARE(color.green(), 0.0f);
    QCOMPARE(color.blue(), 0.0f);
    QCOMPARE(color.hue(), 0.0f);
    QCOMPARE(color.saturation(), 0.0f);
    QCOMPARE(color.lightness(), 0.0f);
    QCOMPARE(color.value(), 0.0f);

    color = IPLColor::fromRGB(0.125f, 0.25f, 0.50f);
    QCOMPARE(color.red(), 0.125f);
    QCOMPARE(color.green(), 0.25f);
    QCOMPARE(color.blue(), 0.50f);
    QCOMPARE(color.hue(), 0.6111111f);
    QCOMPARE(color.saturation(), 0.75f);
    QCOMPARE(color.lightness(), 0.3125f);
    QCOMPARE(color.value(), 0.50f);

    color = IPLColor::fromHSL(0.6111111f, 0.75f, 0.3125f);
    QCOMPARE(color.red(), 0.125f);
    QCOMPARE(color.green(), 0.25f);
    QCOMPARE(color.blue(), 0.50f);
    QCOMPARE(color.hue(), 0.6111111f);
    QCOMPARE(color.saturation(), 0.75f);
    QCOMPARE(color.lightness(), 0.3125f);
    QCOMPARE(color.value(), 0.50f);

    color = IPLColor::fromHSV(0.6111111f, 0.75f, 0.50f);
    QCOMPARE(color.red(), 0.125f);
    QCOMPARE(color.green(), 0.25f);
    QCOMPARE(color.blue(), 0.50f);
    QCOMPARE(color.hue(), 0.6111111f);
    QCOMPARE(color.saturation(), 0.75f);
    QCOMPARE(color.lightness(), 0.3125f);
    QCOMPARE(color.value(), 0.50f);
}

QTEST_APPLESS_MAIN(IPLTests)

#include "tst_ipl.moc"
