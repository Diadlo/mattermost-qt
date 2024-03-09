/**
 * Copyright 2021-2023 Lyubomir Filipov
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

#pragma once

#include <QWidget>
#include <QTemporaryFile>

namespace Ui {
class AttachedBinaryFile;
}

namespace Mattermost {

class Backend;
class BackendFile;
class Settings;

class AttachedBinaryFile: public QWidget {
    Q_OBJECT

public:
    explicit AttachedBinaryFile(Settings& settings, Backend& backend, const BackendFile& file, QWidget *parent = nullptr);
    ~AttachedBinaryFile();
private:
    void setFileMimeIcon (const QString& filename);
private:
    Ui::AttachedBinaryFile* ui;
    QTemporaryFile          tempFile;
    QString                 downloadedPath;
    Settings&               m_settings;
};

} /* namespace Mattermost */

