#include "presetselection.h"

PresetSelection::PresetSelection(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout;

    // Example presets
    QPushButton *preset1 = new QPushButton("Preset 1: 4/4, 120 BPM, 60% volume, triplets off");
    connect(preset1, &QPushButton::clicked, this, [this]() { // since the signal has parameters, we use a lambda function
        emit presetSelected(4, 120, 50, false);
    });

    QPushButton *preset2 = new QPushButton("Preset 2: 3/4, 90 BPM, 50% volume, triplets off");
    connect(preset2, &QPushButton::clicked, this, [this]() {
        emit presetSelected(3, 90, 50, false);
    });

    QPushButton *preset3 = new QPushButton("Preset 3: 4/4, 75 BPM, 50% volume, triplets on");
    connect(preset3, &QPushButton::clicked, this, [this]() {
        emit presetSelected(4, 75, 50, true);
    });

    QPushButton *loadFromFileButton = new QPushButton("Load from File...");
    connect(loadFromFileButton, &QPushButton::clicked, this, &PresetSelection::loadFromFileRequested); // since the signal has no parameters, no lambda function needed

    layout->addWidget(preset1);
    layout->addWidget(preset2);
    layout->addWidget(preset3);
    layout->addStretch();
    layout->addWidget(loadFromFileButton);

    setLayout(layout);
}
