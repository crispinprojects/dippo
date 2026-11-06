/*
 *
 * Copyright 2026 Alan Crispin <crispinalan@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://gnu.org>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mainwindow.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, m_engine(new Synthesizer())
, m_voiceDict(new Dictionary())
{
    // Create instances
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    auto *textInput = new QLineEdit(central);
    textInput->setPlaceholderText("Enter phrase");

    auto *button = new QPushButton("Speak", central);
    auto *message = new QLabel(central);

    layout->addWidget(textInput);
    layout->addWidget(button);
    layout->addWidget(message);

    setCentralWidget(central);
    setWindowTitle("Dippo Qt");
    resize(320, 150);

    // Lambda function to handle button click
    auto sayText = [this, textInput, message]() {
        QString speakStr = textInput->text().trimmed();

        if (speakStr.isEmpty()) {
            message->setText("Enter phrase");
        } else {
            qDebug() << "SpeakStr= " << speakStr;

            // Convert QString to std::string
            std::string processedText = speakStr.toStdString();

            processedText =preprocess_text(processedText);

            // Call the synthesizer's speak method
            m_engine->speak(processedText, m_voiceDict, 1.5f);
            message->setText("Speaking: " + speakStr);
        }
    };

    connect(button, &QPushButton::clicked, this, sayText);
}

MainWindow::~MainWindow()
{
    delete m_engine;       // Clean up dynamically allocated memory
    delete m_voiceDict;    // Clean up dynamically allocated memory
}
