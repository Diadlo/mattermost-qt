/**
 * @file BackendPoll.h
 * @brief Holds all data for a poll for a post.
 * @author Lyubomir Filipov
 * @date Oct 6, 2022
 *
 * Copyright 2021, 2022 Lyubomir Filipov
 *
 * This file is part of Mattermost-QT.
 *
 * Mattermost-QT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Mattermost-QT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Mattermost-QT; if not, see https://www.gnu.org/licenses/.
 */

#pragma once

#include <QJsonObject>
#include <QVector>

namespace Mattermost {

struct BackendPollOption {
	QString name;
	QString voters;
	QString	actionID;
};

class BackendPoll {
public:
	BackendPoll (const QString& pollID, const QJsonObject& jsonObject);
	~BackendPoll ();
private:
	void fillChoiceOptions (const QJsonArray& optionsJson);
	void fillPreviewOptions (const QJsonArray& optionsJson);
public:
	QString						id;
	QString						title;
	QString						text;
	QVector<BackendPollOption>	options;
	bool						hasEnded;

	bool						hasAdminPermissions;
};

} /* namespace Mattermost */
