#ifndef METRONOME_H
#define METRONOME_H

#include <QtWidgets>
#include <QSoundEffect>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QTimer>
#include "beatvisualizer.h"

class Metronome : public QWidget
{
    Q_OBJECT

public:
    Metronome(QWidget *parent = nullptr);
    void setBeats(int beats);
    void setTempo(int tempo);
    void setVolume(int volume);
    void enableTriplets();
    void disableTriplets();

signals:
    void backToPresets();

private slots:
    void updateBeats(int value);
    void updateTempo(int value);
    void updateVolume(int value);
    void saveSettings();
    void togglePlayStop();
    void stop();
    void playNextBeat();
    void toggleTriplets();

private:
    int m_beats;
    int m_subdivisionCount;
    int m_subdivisionsPerBeat;
    int m_tempo;
    int m_volume;
    int m_currentBeat;

    QLabel *m_beatsLabel;
    QLabel *m_tempoLabel;
    QLabel *m_volumeLabel;

    QSlider *m_beatsSlider;
    QSlider *m_tempoSlider;
    QSlider *m_volumeSlider;

    QPushButton *m_backButton;
    QPushButton *m_saveButton;
    QPushButton *m_playStopButton;

    QPushButton *m_tripletButton;

    QSoundEffect *m_beatSound;
    QSoundEffect *m_accentBeatSound;

    QTimer *m_timer;

    BeatVisualizer *m_beatVisualizer;

    bool m_isPlaying;
    bool m_tripletsEnabled;
};

#endif // METRONOME_H
