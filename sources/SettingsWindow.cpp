/**
 * Copyright 2021, 2022 Lyubomir Filipov
 *
 * This file is part of Mattermost-QT.
 *
 * Mattermost-QT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mattermost-QT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Mattermost-QT. if not, see https://www.gnu.org/licenses/.
 */

#include "SettingsWindow.h"

#include <QFileDialog>
#include <QIntValidator>
#include <QStandardPaths>
#include "ui_SettingsWindow.h"
#include "Settings.h"

namespace Mattermost {

SettingsWindow::SettingsWindow(Settings& settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow),
	m_settings(settings)
{
	ui->setupUi(this);
	QValidator *imageSizeValidator = new QIntValidator(Settings::MIN_IMAGE_SIZE, Settings::MAX_IMAGE_SIZE, this);
	ui->imageMaxWidthValue->setValidator(imageSizeValidator);
	ui->imageMaxHeightValue->setValidator(imageSizeValidator);

	ui->downloadLocationValue->setText(m_settings.getDownloadLocation());
	ui->askLocationCheckBox->setChecked(m_settings.getDownloadAsk());
	ui->imageMaxWidthValue->setText(QString::number(m_settings.getImageMaxWidth()));
	ui->imageMaxHeightValue->setText(QString::number(m_settings.getImageMaxHeight()));

	ui->closeToTray->setChecked(m_settings.getCloseToTray());

	connect (ui->downloadLocationButton, &QPushButton::clicked, [this] {
		QDir defaultDir(ui->downloadLocationValue->text());

		if (!defaultDir.exists()) {
			defaultDir = QDir::home();
		}
		ui->downloadLocationValue->setText (QFileDialog::getExistingDirectory (this, "Select destination directory", defaultDir.absolutePath()));
	});
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::applyNewSettings ()
{
	m_settings.setDownloadLocation(ui->downloadLocationValue->text());
	m_settings.setDownloadAsk(ui->askLocationCheckBox->isChecked());
	m_settings.setImageMaxWidth(ui->imageMaxWidthValue->text().toInt());
	m_settings.setImageMaxHeight(ui->imageMaxHeightValue->text().toInt());
	m_settings.setCloseToTray(ui->closeToTray->isChecked());
}

} /* namespace Mattermost */
