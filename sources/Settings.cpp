/**
 * Copyright 2024 Mattermost-QT developers
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

#include "Settings.h"
#include <QSettings>
#include <QString>
#include <QStandardPaths>

static constexpr const char* DOWNLOAD_LOCATION = "config/downloadLocation";
static constexpr const char* DOWNLOAD_ASK = "config/downloadAsk";
static constexpr const char* DOWNLOAD_IMAGE_MAX_WIDTH = "config/imageMaxWidth";
static constexpr const char* DOWNLOAD_IMAGE_MAX_HEIGHT = "config/imageMaxHeight";

namespace Mattermost {

Settings& Settings::getInstance() {
    static Settings instance;
    return instance;
}

void Settings::setDownloadLocation(const QString& location) {
    m_settings.setValue(DOWNLOAD_LOCATION, location);
    m_settings.sync();
}


QString Settings::getDownloadLocation() const {
	QString defaultDownloadDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    return m_settings.value(DOWNLOAD_LOCATION, defaultDownloadDir).toString();
}

void Settings::setDownloadAsk(bool ask) {
    m_settings.setValue(DOWNLOAD_ASK, ask);
    m_settings.sync();
}
bool Settings::getDownloadAsk() const {
    return m_settings.value(DOWNLOAD_ASK, false).toBool();
}

void Settings::setImageMaxWidth(int width) {
    m_settings.setValue(DOWNLOAD_IMAGE_MAX_WIDTH, width);
    m_settings.sync();
}

int Settings::getImageMaxWidth() const {
    return m_settings.value(DOWNLOAD_IMAGE_MAX_WIDTH, 400).toInt();
}

void Settings::setImageMaxHeight(int height) {
    m_settings.setValue(DOWNLOAD_IMAGE_MAX_HEIGHT, height);
    m_settings.sync();
}

int Settings::getImageMaxHeight() const {
    return m_settings.value(DOWNLOAD_IMAGE_MAX_HEIGHT, 400).toInt();
}

QSize Settings::getImageMaxSize() const {
    return QSize(getImageMaxWidth(), getImageMaxHeight());
}

void Settings::setWindowGeometry(const QByteArray& geometry) {
    m_settings.setValue("geometry", geometry);
    m_settings.sync();
}

QByteArray Settings::getWindowGeometry() const {
    return m_settings.value("geometry").toByteArray();
}

void Settings::setDomain(const QString& domain) {
    m_settings.setValue("domain", domain);
    m_settings.sync();
}

QString Settings::getDomain() const {
    return m_settings.value("domain").toString();
}

void Settings::setUsername(const QString& username) {
    m_settings.setValue("username", username);
    m_settings.sync();
}

QString Settings::getUsername() const {
    return m_settings.value("username").toString();
}

void Settings::setToken(const QString& token) {
    m_settings.setValue("token", token);
    m_settings.sync();
}

QString Settings::getToken() const {
    return m_settings.value("token").toString();
}

void Settings::setEmojiFavorites(const QByteArray& favorites) {
    m_settings.setValue("emoji_favorites", favorites);
    m_settings.sync();
}

QByteArray Settings::getEmojiFavorites() const {
    return m_settings.value("emoji_favorites").toByteArray();
}

void Settings::setCloseToTray(bool close) {
    m_settings.setValue("closeToTray", close);
    m_settings.sync();
}

bool Settings::getCloseToTray() const {
    return m_settings.value("closeToTray", false).toBool();
}

} /* namespace Mattermost */