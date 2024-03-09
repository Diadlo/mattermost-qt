/**
 * @file Storage.h
 * @brief
 * @author Lyubomir Filipov
 * @date Jan 8, 2022
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

#include <QMap>
#include <map>
#include <QSharedPointer>
#include "backend/types/BackendUser.h"
#include "backend/types/BackendTeam.h"
#include "backend/types/BackendDirectChannelsTeam.h"
#include "backend/types/BackendLoginUserPreferences.h"
#include <QJsonDocument>

namespace Mattermost {

class Storage {
public:
	Storage ();
	virtual ~Storage ();
public:
	void reset ();

	BackendTeam* getTeamById (const QString& teamID);
	const BackendTeam* getTeamById (const QString& teamID) const;

	BackendChannel* getChannelById (const QString& channelID);
	BackendChannel* getDirectChannelByUserId (const QString& userID) const;

	const std::map<QString, BackendUser>& getAllUsers () const;

	BackendUser* getUserById (const QString& userID);
	const BackendUser* getUserById (const QString& userID) const;

	const std::shared_ptr<BackendLoginUserPreferences>& getUserPreferences() const;
	/**
	 * Get user's display name by given user ID.
	 * If no user is found, the user ID is returned.
	 * @param userID user ID
	 * @param explainLoginUser if user is same as loginUser, the "(you)" string is appended to the name
	 * @return user display name
	 */
	QString getUserDisplayNameByUserId (const QString& userID, bool explainLoginUser = false) const;

	BackendTeam* addTeam (const QJsonObject& json);

	BackendChannel* addChannel (BackendTeam& team, const QJsonObject& json);
	BackendChannel* addTeamChannel (BackendTeam& team, const QJsonObject& json);
	BackendChannel* addDirectChannel (const QJsonObject& json);
	BackendChannel* addGroupChannel (const QJsonObject& json);

	BackendUser* addUser (const QJsonObject& json, bool isLoggedInUser = false);
	void setUserPreferences(const QJsonDocument& doc);

	void eraseTeam (const QString& teamID);

	void eraseChannel (BackendChannel& channel);

	void printTeams ();
public:
	std::map<QString, BackendTeam>					teams;
	BackendDirectChannelsTeam						directChannels;
	BackendDirectChannelsTeam						groupChannels;
	QMap<QString, BackendChannel*> 					channels;
	QMap<QString, BackendChannel*> 					directChannelsByUser;
	std::map<QString, BackendUser>					users;
	BackendUser*									loginUser;
	std::shared_ptr<BackendLoginUserPreferences>    loginUserPreferences;
	BackendUser*									matterpollUser;
	uint32_t										totalUsersCount;
};

} /* namespace Mattermost */
