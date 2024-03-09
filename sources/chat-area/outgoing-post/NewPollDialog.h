/**
 * @file NewPollDialog.h
 * @brief New Poll Dialog - shown when creating a poll
 * @author Lyubomir Filipov
 * @date Mar 15, 2022
 *
 * Copyright 2021-2023 Lyubomir Filipov
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

#include <QDialog>
#include "backend/types/BackendNewPollData.h"

namespace Ui {
class NewPollDialog;
}

namespace Mattermost {

class NewPollDialog: public QDialog {
    Q_OBJECT
public:
    explicit NewPollDialog (QWidget *parent, BackendNewPollData initialPollData);
    ~NewPollDialog();
public:
    BackendNewPollData getData ();
private:
    void validateInput ();
    void disableSendButton (const QString& tooltip);
private:
    Ui::NewPollDialog *ui;
};

} /* namespace Mattermost */
