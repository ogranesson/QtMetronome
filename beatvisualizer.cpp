#include "beatvisualizer.h"

BeatVisualizer::BeatVisualizer(QWidget *parent)
    : QWidget(parent), m_totalBeats(4), m_currentBeat(0)
{
    setMinimumHeight(50);
    setCursor(Qt::PointingHandCursor);
}

void BeatVisualizer::setTotalBeats(int beats)
{
    m_totalBeats = beats;
    update();
}

void BeatVisualizer::setCurrentBeat(int beat)
{
    m_currentBeat = beat;
    update();
}

void BeatVisualizer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    int squareWidth = width() / m_totalBeats;
    int squareHeight = height();

    for (int i = 0; i < m_totalBeats; ++i) {
        QRect rect(i * squareWidth, 0, squareWidth, squareHeight);
        if (i == m_currentBeat) {
            painter.fillRect(rect, Qt::green);
        } else {
            painter.fillRect(rect, Qt::gray);
        }
        painter.drawRect(rect);
    }
}

void BeatVisualizer::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit toggledPlayPause();
}
