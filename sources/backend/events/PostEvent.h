/**
 * @file PostEvent.h
 * @brief
 * @author Lyubomir Filipov
 * @date Dec 31, 2021
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

#include <QString>
#include <QJsonObject>

namespace Mattermost {

class PostEvent {
public:
	PostEvent (const QJsonObject& data, const QJsonObject& broadcast);
	virtual ~PostEvent ();
public:
	QString		teamId;
	QString		channelId;
	QJsonObject	postObject;
	bool		set_online;
};

} /* namespace Mattermost */
