#include "metronome.h"

Metronome::Metronome(QWidget *parent)
    : QWidget(parent), m_beats(4), m_bufferBeats(4), m_tempo(120), m_volume(50), m_currentBeat(0), m_isPlaying(false), m_tripletsEnabled(false)
{
    m_beatsLabel = new QLabel(QString("Beats: %1").arg(m_beats));
    m_tempoLabel = new QLabel(QString("Tempo: %1 BPM").arg(m_tempo));
    m_volumeLabel = new QLabel(QString("Volume: %1%").arg(m_volume));

    m_beatsSlider = new QSlider(Qt::Horizontal);
    m_beatsSlider->setMinimum(1);
    m_beatsSlider->setMaximum(12);
    m_beatsSlider->setValue(m_beats);
    connect(m_beatsSlider, &QSlider::valueChanged, this, &Metronome::updateBeats);

    m_tempoSlider = new QSlider(Qt::Horizontal);
    m_tempoSlider->setMinimum(40);
    m_tempoSlider->setMaximum(240);
    m_tempoSlider->setValue(m_tempo);
    connect(m_tempoSlider, &QSlider::valueChanged, this, &Metronome::updateTempo);

    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setMinimum(0);
    m_volumeSlider->setMaximum(100);
    m_volumeSlider->setValue(m_volume);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &Metronome::updateVolume);

    m_backButton = new QPushButton(QString("Back to presets"));
    connect(m_backButton, &QPushButton::clicked, this, &Metronome::backToPresets);

    m_saveButton = new QPushButton(QString("Save Settings"));
    connect(m_saveButton, &QPushButton::clicked, this, &Metronome::saveSettings);

    m_playStopButton = new QPushButton(QString("Play"));
    connect(m_playStopButton, &QPushButton::clicked, this, &Metronome::togglePlayStop);

    m_tripletButton = new QPushButton(QString("Triplets: Off"));
    connect(m_tripletButton, &QPushButton::clicked, this, &Metronome::toggleTriplets);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Metronome::playNextBeat);

    m_beatSound = new QSoundEffect(this);
    m_accentBeatSound = new QSoundEffect(this);

    m_beatSound->setSource(QUrl("qrc:/wav/beat.wav"));
    m_accentBeatSound->setSource(QUrl("qrc:/wav/accent_beat.wav"));

    m_beatSound->setVolume(static_cast<qreal>(m_volume) / 100);
    m_accentBeatSound->setVolume(static_cast<qreal>(m_volume) / 100);

    m_beatVisualizer = new BeatVisualizer(this);
    connect(m_beatVisualizer, &BeatVisualizer::toggledPlayPause, this, &Metronome::togglePlayStop);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_beatVisualizer);
    layout->addWidget(m_beatsLabel);
    layout->addWidget(m_beatsSlider);
    layout->addWidget(m_tempoLabel);
    layout->addWidget(m_tempoSlider);
    layout->addWidget(m_volumeLabel);
    layout->addWidget(m_volumeSlider);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_backButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_playStopButton);
    buttonLayout->addWidget(m_tripletButton);

    layout->addLayout(buttonLayout);
    setLayout(layout);
}

void Metronome::updateBeats(int value)
{
    if (m_tripletsEnabled == false) {
        m_beats = value;
        m_beatVisualizer->setTotalBeats(m_beats);
        m_beatsLabel->setText(QString("Beats: %1").arg(m_beats));
    } else {
        m_beats = 3;
        m_bufferBeats = value;
        m_beatVisualizer->setTotalBeats(m_bufferBeats);
        m_beatsLabel->setText(QString("Beats: %1").arg(m_bufferBeats));
    }
}

void Metronome::updateTempo(int value)
{
    if (m_tripletsEnabled == false) {
        m_tempo = value;
        m_tempoLabel->setText(QString("Tempo: %1 BPM").arg(m_tempo));
    } else {
        m_tempo = value * 3;
        m_tempoLabel->setText(QString("Tempo: %1 BPM").arg(value));
    }

    if (m_isPlaying) {
        m_timer->setInterval(60000 / m_tempo);
    }
}

void Metronome::updateVolume(int value)
{
    m_volume = value;
    m_volumeLabel->setText(QString("Volume: %1%").arg(m_volume));
    m_beatSound->setVolume(static_cast<qreal>(m_volume) / 100);
    m_accentBeatSound->setVolume(static_cast<qreal>(m_volume) / 100);
}

void Metronome::saveSettings()
{
    QString filePath = QFileDialog::getSaveFileName(this, QString("Save Settings"), "preset.csv", QString("CSV Files (*.csv);;All Files (*)"));
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << "Beats,Tempo,Volume,Triplets\n";
            if (m_tripletsEnabled == true) {
                stream << m_bufferBeats << "," << m_tempo/3 << "," << m_volume << "," << m_tripletsEnabled << "\n";
            } else {
                stream << m_beats << "," << m_tempo << "," << m_volume << "," << m_tripletsEnabled << "\n";
            }
            file.close();
            QMessageBox::information(this, QString("Success"), QString("Settings saved successfully!"));
        } else {
            QMessageBox::critical(this, QString("Error"), QString("Failed to save settings."));
        }
    }
}

void Metronome::togglePlayStop()
{
    if (m_isPlaying) {
        m_timer->stop();
        m_beatSound->stop();
        m_accentBeatSound->stop();
        m_playStopButton->setText(QString("Play"));
        m_isPlaying = false;
        m_currentBeat = 0;
    } else {
        m_currentBeat = 0;
        m_timer->setInterval(60000 / m_tempo);
        m_timer->start();
        m_playStopButton->setText(QString("Stop"));
        m_isPlaying = true;
    }
}

void Metronome::playNextBeat()
{
    if (!m_isPlaying)
        return;

    if (m_currentBeat == 0) {
        m_accentBeatSound->play();
    } else {
        m_beatSound->play();
    }

    m_beatVisualizer->setCurrentBeat(m_currentBeat);

    m_currentBeat = (m_currentBeat + 1) % m_beats;
}


void Metronome::toggleTriplets()
{
    if (!m_tripletsEnabled) {
        m_tempo = m_tempo * 3;
        m_bufferBeats = m_beats;
        m_beats = 3;
        m_timer->setInterval(60000 / m_tempo);
        m_tripletButton->setText(QString("Triplets: On"));
        m_tripletsEnabled = true;
    }
    else {
        m_tempo = m_tempo / 3;
        m_beats = m_bufferBeats;
        m_timer->setInterval(60000 / m_tempo);
        m_tripletButton->setText(QString("Triplets: Off"));
        m_tripletsEnabled = false;
    }
}

void Metronome::setBeats(int beats)
{
    m_beatsSlider->setValue(beats);
}

void Metronome::setBufferBeats(int beats)
{
    m_bufferBeats = beats;
    m_beats = 3;
    m_beatsSlider->setValue(m_bufferBeats);
}

void Metronome::setTempo(int tempo)
{
    m_tempoSlider->setValue(tempo);
}

void Metronome::setVolume(int volume)
{
    m_volumeSlider->setValue(volume);
}

void Metronome::enableTriplets()
{
    if (!m_tripletsEnabled) {
        toggleTriplets();
    }
}

void Metronome::disableTriplets()
{
    if (m_tripletsEnabled) {
        toggleTriplets();
    }
}
