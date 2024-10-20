#ifndef BEATVISUALIZER_H
#define BEATVISUALIZER_H

#include <QtWidgets>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>

class BeatVisualizer : public QWidget
{
    Q_OBJECT

public:
    explicit BeatVisualizer(QWidget *parent = nullptr);
    void setTotalBeats(int beats);
    void setCurrentBeat(int beat);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void toggledPlayPause();

private:
    int m_totalBeats;
    int m_currentBeat;
};

#endif // BEATVISUALIZER_H
