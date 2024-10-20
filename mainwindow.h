#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "metronome.h"
#include "presetselection.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    QStackedWidget *m_stack;
    Metronome *m_metronome;
    PresetSelection *m_presetSelection;
    Ui::MainWindow *ui;

private slots:
    void onPresetSelected(int beats, int tempo, int volume, bool triplets);
    void onLoadFromFileRequested();
    void onBackToPresets();
};
#endif // MAINWINDOW_H
