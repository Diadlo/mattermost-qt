/**
 * @file PinnedPostsList.cpp
 * @brief Shows a list of pinned posts for a channel
 * @author Lyubomir Filipov
 * @date Apr 1, 2023
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

#include <QWidget>
#include <list>
#include "backend/types/BackendPost.h"

namespace Ui {
class PinnedPostsList;
}

namespace Mattermost {

class PostWidget;

class PinnedPostsList: public QWidget {
	Q_OBJECT
public:
	explicit PinnedPostsList (QWidget *parent);
	~PinnedPostsList();
public:
	void addPost (PostWidget* postWidget);
private:
	Ui::PinnedPostsList *ui;
};

} /* namespace Mattermost */
