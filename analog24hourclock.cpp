// Copyright (c) 2016 Arjen Hoogesteger
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
// Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "analog24hourclock.h"
#include <QtWidgets>
#include <algorithm>
#include <array>

Analog24HourClock::Analog24HourClock(QWidget* parent):
    QWidget(parent)
{
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);

    setWindowTitle("24hr Analog Clock");
}

void Analog24HourClock::paintEvent(QPaintEvent*)
{
    QTime now = QTime::currentTime();

    QPainter painter(this);

    const int sideLength = std::min(width(), height());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2.0, height() / 2.0);
    painter.scale(sideLength / 400.0, sideLength / 400.0);

    painter.setPen(QColor(190, 190, 190));
    painter.setBrush(QColor(20, 10, 200, 190));
    painter.setFont(QFont("monospace", 10));

    // draw the hours
    for (unsigned int i = 0; i < 24; ++i)
    {
        static const QRect hourRect(-32, -170, 64, 24);
        painter.save();

        painter.rotate(i * 15.0);

        if (i == static_cast<unsigned int>(now.hour()))
        {
            painter.save();
            painter.setPen(QColor(0, 0, 0));
            painter.drawText(hourRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(i));
            painter.restore();
        }
        else
            painter.drawText(hourRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(i));

        painter.restore();
    }

    // draw the seconds hand
    painter.save();
    painter.setPen(QColor(255, 0, 0));
    painter.rotate((now.second() * 1000 + now.msec()) / 60000.0 * 360.0);
    painter.drawLine(QPoint(0, 10), QPoint(0, -180));
    painter.restore();

    // draw the minutes hand
    static const std::array<QPoint, 3> minutesHand = {
        QPoint(-5, 5),
        QPoint(5, 5),
        QPoint(0, -180)
    };

    painter.save();
    painter.setPen(QColor(10, 200, 20));
    painter.setBrush(QColor(10, 200, 20, 127));
    painter.rotate((now.minute() * 60000 + now.second() * 1000 + now.msec()) / 3600000.0 * 360.0);
    painter.drawConvexPolygon(minutesHand.data(), 3);
    painter.restore();

    // draw the hours hand
    static const std::array<QPoint, 3> hoursHand = {
        QPoint(-5, 5),
        QPoint(5, 5),
        QPoint(0, -120)
    };

    painter.save();
    painter.setPen(QColor(10, 20, 200));
    painter.setBrush(QColor(10, 20, 200, 127));
    painter.rotate((now.hour() * 3600 + now.minute() * 60 + now.second()) / 86400.0 * 360.0);
    painter.drawConvexPolygon(hoursHand.data(), 3);
    painter.restore();
}
