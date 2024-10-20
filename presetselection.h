#ifndef PRESETSELECTION_H
#define PRESETSELECTION_H

#include <QtWidgets>

class PresetSelection : public QWidget
{
    Q_OBJECT
public:
    explicit PresetSelection(QWidget *parent = nullptr);

signals:
    void presetSelected(int beats, int tempo, int volume, bool triplets);
    void loadFromFileRequested();
};

#endif // PRESETSELECTION_H
