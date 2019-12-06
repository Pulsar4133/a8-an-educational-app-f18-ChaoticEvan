#include "scrolltext.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QWidget>

ScrollText::ScrollText(QWidget *parent) : QWidget(parent), scrollPos(0)
{
    // Sets variables for actual text
    staticText.setTextFormat(Qt::PlainText);
    setFixedHeight(fontMetrics().height() + 10);
    leftMargin = width();
    seperator = "   ";
    timer.setInterval(30);

    QObject::connect(&timer, &QTimer::timeout, this, &ScrollText::timer_timeout);
}

ScrollText::~ScrollText()
{

}

QString ScrollText::getText()
{
    return text;
}

void ScrollText::setText(QString _text)
{
    text = _text;
    updateText();
    update();
}

QString ScrollText::getSeperator()
{
    return seperator;
}

void ScrollText::setSeperator(QString _seperator)
{
    seperator = _seperator;
    updateText();
    update();
}

void ScrollText::updateText()
{
    singleTextWidth = fontMetrics().width(text);
    timer.start();
    scrollPos = 0;
    staticText.setText(text);
    staticText.prepare(QTransform(), font());
    wholeTextSize = QSize(fontMetrics().width(staticText.text()), fontMetrics().height());
}

void ScrollText::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    buffer = QImage(size(), QImage::Format_ARGB32_Premultiplied);

    buffer.fill(qRgba(0, 0 ,0, 0));
    QPainter pb(&buffer);
    pb.setPen(painter.pen());
    pb.setFont(painter.font());

    int x = qMin(-scrollPos, 0)+ leftMargin + (leftMargin/2);

    if(x < width())
    {
        pb.drawStaticText(QPointF(x, (height() - wholeTextSize.height()) / 2) + QPoint(0,0), staticText);
        x +=  wholeTextSize.width();

        if(x < 0)
        {
            scrollPos = 0;
        }

        painter.drawImage(0, 0, buffer);
    }
}

void ScrollText::timer_timeout()
{
    scrollPos = (scrollPos + 2);
    update();
}

