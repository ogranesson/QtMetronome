#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "metronome.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    m_presetSelection = new PresetSelection(this);
    m_metronome = new Metronome(this);

    m_stack->addWidget(m_presetSelection);
    m_stack->addWidget(m_metronome);

    connect(m_presetSelection, &PresetSelection::presetSelected, this, &MainWindow::onPresetSelected);
    connect(m_presetSelection, &PresetSelection::loadFromFileRequested, this, &MainWindow::onLoadFromFileRequested);
    connect(m_metronome, &Metronome::backToPresets, this, &MainWindow::onBackToPresets);

    setWindowTitle(tr("Metronome"));
    resize(400, 300);
}

// the functions below are being implemented here instead of in the presetSelection class because
// the buttons are being clicked in presetSelection, but data has to be transferred to metronome
// buttons pressed in presetSelection emit signals, which are caught above

void MainWindow::onPresetSelected(int beats, int tempo, int volume, bool triplets)
{
    m_metronome->setBeats(beats);
    m_metronome->setTempo(tempo);
    m_metronome->setVolume(volume);
    if (triplets)
        m_metronome->enableTriplets();
    else
        m_metronome->disableTriplets();

    m_stack->setCurrentWidget(m_metronome);
}

void MainWindow::onLoadFromFileRequested()
{
    QString filePath = QFileDialog::getOpenFileName(this, QString("Load Settings"), "", QString("CSV Files (*.csv);;All Files (*)"));
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString header = stream.readLine();
            QString line = stream.readLine();
            QStringList values = line.split(",");
            if (values.size() == 4) {
                if (values[3].toInt() == 1) {
                    m_metronome->setBeats(values[0].toInt());
                    m_metronome->setTempo(values[1].toInt());
                    m_metronome->setVolume(values[2].toInt());
                    m_metronome->enableTriplets();
                } else {
                    m_metronome->setBufferBeats(values[0].toInt());
                    m_metronome->setTempo(values[1].toInt());
                    m_metronome->setVolume(values[2].toInt());
                    m_metronome->disableTriplets();
                }

                QMessageBox::information(this, QString("Success"), QString("Settings loaded successfully!"));
                m_stack->setCurrentWidget(m_metronome);
            }
            file.close();
        } else {
            QMessageBox::critical(this, QString("Error"), QString("Failed to load settings."));
        }
    }
}

void MainWindow::onBackToPresets()
{
    m_stack->setCurrentWidget(m_presetSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
}


