/**
 * @file Settings.h
 * @brief Settings keys definition
 * @author Lyubomir Filipov
 * @date Dec 8, 2022
 *
 * Copyright 2021-2022 Lyubomir Filipov
 * Copyright 2023-2024 Mattermost-QT developers
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

#pragma once

#include <QSettings>
#include <QSize>

namespace Mattermost {

class Settings {
public:
    static constexpr int MAX_IMAGE_SIZE = 1000;
    static constexpr int MIN_IMAGE_SIZE = 10;

    static Settings& getInstance();

    // Download settings
    void setDownloadLocation(const QString& location);
    QString getDownloadLocation() const;

    void setDownloadAsk(bool ask);
    bool getDownloadAsk() const;

    // Image preview settings
    void setImageMaxWidth(int width);
    int getImageMaxWidth() const;

    void setImageMaxHeight(int height);
    int getImageMaxHeight() const;

    QSize getImageMaxSize() const;

    // Window settings
    void setWindowGeometry(const QByteArray& geometry);
    QByteArray getWindowGeometry() const;

    // User settings
    void setDomain(const QString& domain);
    QString getDomain() const;

    void setUsername(const QString& username);
    QString getUsername() const;

    void setToken(const QString& token);
    QString getToken() const;

    // Emoji settings
    void setEmojiFavorites(const QByteArray& favorites);
    QByteArray getEmojiFavorites() const;

    // Tray settings
    void setCloseToTray(bool close);
    bool getCloseToTray() const;

private:
    QSettings m_settings;
};

}