#pragma once
#include <QWidget>
#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QGradient>
#include <observer.h>
#include <tlv.h>
#include <tlv_container.h>

#define TYPE_ON 0x12
#define TYPE_OFF 0x13
#define TYPE_CHANGE_COLOR 0x20

enum class SWITCH
{
    OFF, ON
};

class RenderLantern : public Observer
{
public:
    RenderLantern(QWidget* parent, const tlv::TLVContainer* tlv_container);
    void update(const tlv::TLV& package) override;
    void drawLed(QPainter* p);
    void resize();

private:
    QWidget* win;
    QColor ledColor;
    QColor lightColor;
    QColor shadowColor;
    QColor ringShadowDarkColor;
    QColor ringShadowMedColor;
    QColor ringShadowLightColor;
    QColor topReflexUpColor;
    QColor topReflexDownColor;
    QColor bottomReflexCenterColor;
    QColor bottomReflexSideColor;

    int height;
    int width;
    int minDim;
    int half;
    int centerX;
    int centerY;
    QRect drawingRect;

    int outerBorderWidth;
    int innerBorderWidth;
    int outerBorderRadius;
    int innerBorderRadius;
    int topReflexY;
    int bottomReflexY;
    int topReflexWidth;
    int topReflexHeight;
    int bottomReflexWidth;
    int bottomReflexHeight;

    SWITCH power = SWITCH::OFF;
    bool default_color = true;
    void swithOn();
    void swithOff();
    void changeColor(const tlv::TLV& package);
};
