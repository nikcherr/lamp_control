#include <render_lantern.h>
#include <tlv_container.h>
#include <functional>
using namespace tlv;

RenderLantern::RenderLantern(QWidget* parent, const TLVContainer* tlv_container)
    :
    win(parent),
    ledColor(Qt::gray),
    lightColor(QColor(0xE0, 0xE0, 0xE0)),
    shadowColor(QColor(0xd2, 0x76, 0x6e)),
    ringShadowDarkColor(QColor(0x50, 0x50, 0x50, 0xFF)),
    ringShadowMedColor(QColor(0x50, 0x50, 0x50, 0x20)),
    ringShadowLightColor(QColor(0xEE, 0xEE, 0xEE, 0x00)),
    topReflexUpColor(QColor(0xFF, 0xFF, 0xFF, 0xA0)),
    topReflexDownColor(QColor(0xFF, 0xFF, 0xFF, 0x00)),
    bottomReflexCenterColor(QColor(0xFF, 0xFF, 0xFF, 0x00)),
    bottomReflexSideColor(QColor(0xFF, 0xFF, 0xFF, 0x70))
{
    Observer observer(std::bind(&RenderLantern::update, this, std::placeholders::_1));
    if(tlv_container)
    {
        tlv_container->attach(observer);
    }
}

void RenderLantern::swithOn()
{
    power = SWITCH::ON;
    if(default_color) {
        ledColor = Qt::green;
        default_color = false;
    }
}

void RenderLantern::swithOff()
{
    power = SWITCH::OFF;
    ledColor = Qt::gray;
    win->repaint(drawingRect);
}

void RenderLantern::changeColor(const TLV& package)
{
    ledColor = QColor(package.getValue()[0], package.getValue()[1], package.getValue()[2]);
    default_color = false;
}

void RenderLantern::update(const TLV& package)
{
    switch (package.getType()) {
    case TYPE_ON:
        swithOn();
        break;
    case TYPE_OFF:
        swithOff();
        break;
    case TYPE_CHANGE_COLOR:
        changeColor(package);
        break;
    }
    if(power == SWITCH::ON)
        win->repaint(drawingRect);
}

void RenderLantern::drawLed(QPainter* p)
{
    QPen pen;
    pen.setStyle(Qt::NoPen);
    p->setPen(pen);

    QRadialGradient outerRingGradient(QPoint(centerX,
                          centerY - outerBorderRadius - (outerBorderWidth / 2)),
                          minDim - (outerBorderWidth / 2));
    outerRingGradient.setColorAt(0, lightColor);
    outerRingGradient.setColorAt(1, shadowColor);
    QBrush outerRingBrush(outerRingGradient);
    p->setBrush(outerRingBrush);
    p->drawEllipse(this->drawingRect);


    QRadialGradient innerRingGradient(QPoint(centerX,
                              centerY + innerBorderRadius + (innerBorderWidth / 2)),
                              minDim - (innerBorderWidth / 2));
    innerRingGradient.setColorAt(0, lightColor);
    innerRingGradient.setColorAt(1, shadowColor);
    QBrush innerRingBrush(innerRingGradient);
    p->setBrush(innerRingBrush);
    p->drawEllipse(QPoint(centerX, centerY),
                  outerBorderRadius, outerBorderRadius);

    QColor dark(ledColor.darker(120));
    QRadialGradient glassGradient(QPoint(centerX, centerY),
                              innerBorderRadius);
    glassGradient.setColorAt(0, ledColor);
    glassGradient.setColorAt(1, dark);
    QBrush glassBrush(glassGradient);
    p->setBrush(glassBrush);
    p->drawEllipse(QPoint(centerX, centerY),
                  innerBorderRadius,
                  innerBorderRadius);

    QRadialGradient shadowGradient(QPoint(centerX, centerY),
                                  innerBorderRadius);
    shadowGradient.setColorAt(0, ringShadowLightColor);
    shadowGradient.setColorAt(0.85, ringShadowMedColor);
    shadowGradient.setColorAt(1, ringShadowDarkColor);
    QBrush shadowBrush(shadowGradient);
    p->setBrush(shadowBrush);
    p->drawEllipse(QPoint(centerX, centerY),
                  innerBorderRadius,
                  innerBorderRadius);

    QLinearGradient topTeflexGradient(QPoint(centerX,
                                         (innerBorderWidth + outerBorderWidth)),
                                  QPoint(centerX, centerY));
    topTeflexGradient.setColorAt(0, topReflexUpColor);
    topTeflexGradient.setColorAt(1, topReflexDownColor);
    QBrush topReflexbrush(topTeflexGradient);
    p->setBrush(topReflexbrush);
    p->drawEllipse(QPoint(centerX, topReflexY), topReflexWidth, topReflexHeight);

    QRadialGradient bottomReflexGradient(QPoint(centerX,
                                     bottomReflexY + (bottomReflexHeight / 2)),
                              bottomReflexWidth);
    bottomReflexGradient.setColorAt(0, bottomReflexSideColor);
    bottomReflexGradient.setColorAt(1, bottomReflexCenterColor);
    QBrush bottomReflexBrush(bottomReflexGradient);
    p->setBrush(bottomReflexBrush);
    p->drawEllipse(QPoint(centerX, bottomReflexY),
                  bottomReflexWidth,
                  bottomReflexHeight);
}

void RenderLantern::resize()
{
    this->height = win->size().height();
    this->width = win->size().width();
    this->minDim = (height > width) ? width : height;
    this->half = minDim / 2;
    this->centerX = width / 2;
    this->centerY = height / 2;

    this->outerBorderWidth = minDim / 10;
    this->innerBorderWidth = minDim / 14;
    this->outerBorderRadius = half - outerBorderWidth;
    this->innerBorderRadius = half - (outerBorderWidth + innerBorderWidth);

    this->topReflexY = centerY
            - (half - outerBorderWidth - innerBorderWidth) / 2;
    this->bottomReflexY = centerY
            + (half - outerBorderWidth - innerBorderWidth) / 2;
    this->topReflexHeight = half / 5;
    this->topReflexWidth = half / 3;
    this->bottomReflexHeight = half / 5;
    this->bottomReflexWidth = half / 3;

    drawingRect.setTop((height - minDim) / 2);
    drawingRect.setLeft((width - minDim) / 2);
    drawingRect.setHeight(minDim);
    drawingRect.setWidth(minDim);
}
