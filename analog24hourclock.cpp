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

Analog24HourClock::Analog24HourClock(QWidget* parent) :
	QWidget(parent)
{
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);

	setWindowTitle("24hr Analog Clock");
	setAutoFillBackground(true);
	QPalette colorPalette = palette();
	colorPalette.setColor(QPalette::Background, Qt::white);
	setPalette(colorPalette);
}

void Analog24HourClock::paintEvent(QPaintEvent*)
{
	const QTime now = QTime::currentTime();
	QPainter painter(this);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(width() / 2.0, height() / 2.0);

	// everything is based on a 400 x 400 display area
	const int sideLength = std::min(width(), height());
	const double displayWidth = 400.0;
	const double displayHeight = 400.0;
	painter.scale(sideLength / displayWidth, sideLength / displayHeight);

	painter.setPen(QColor(170, 170, 170));
	painter.setBrush(QColor(20, 10, 200, 190));
	painter.setFont(QFont("Droid Sans", 10));

	// draw the hours
	for (unsigned int i = 0; i < 24; ++i)
	{
		static const QRect hourRect(-32, -170, 64, 24);
		painter.save();
		painter.rotate(i * 15.0);

		if (i == static_cast<unsigned int>(now.hour()))
		{
			painter.setFont(QFont("Droid Sans", 10, QFont::Bold));
			painter.setPen(QColor(0, 0, 0));
		}

		painter.drawText(hourRect, Qt::AlignTop | Qt::AlignHCenter, QString::number(i));
		painter.restore();
	}

	// draw the digital clock
	static const QRect textRect(-50, -20, 100, 40);
	painter.save();
	painter.setFont(QFont("Droid Sans", 20, QFont::Bold));
	painter.setPen(QColor(0, 0, 0));
	painter.drawText(textRect, Qt::AlignCenter, now.toString("hh:mm"));
	painter.restore();

	// draw the seconds indicator
	for (unsigned int i = 0; i < 60; ++i)
	{
		painter.save();
		painter.rotate(i * 360.0 / 60.0);

		if (i == static_cast<unsigned int>(now.second()))
		{
			QPen secondsIndicatorPen(QColor(255, 0, 0));
			secondsIndicatorPen.setWidth(3);
			painter.setPen(secondsIndicatorPen);
		}

		if (i % 5 == 0)
			painter.drawLine(QPoint(0, -125), QPoint(0, -145));
		else
			painter.drawLine(QPoint(0, -125), QPoint(0, -140));

		painter.restore();
	}

	// draw minute indicator
	painter.save();
	QPen minuteIndicatorPen(QColor(10, 200, 20));
	minuteIndicatorPen.setWidth(3);
	painter.setPen(minuteIndicatorPen);
	painter.rotate((now.minute() * 60 + now.second()) / 3600.0 * 360.0);
	painter.drawLine(QPoint(0, -100), QPoint(0, -110));
	painter.restore();

	// draw hour indicator
	painter.save();
	QPen hourIndicatorPen(QColor(10, 20, 200));
	hourIndicatorPen.setWidth(3);
	painter.setPen(hourIndicatorPen);
	painter.setBrush(QColor(10, 20, 200, 127));
	painter.rotate((now.hour() * 3600 + now.minute() * 60 + now.second()) / 86400.0 * 360.0);
	painter.drawLine(QPoint(0, -75), QPoint(0, -85));
	painter.restore();
}
