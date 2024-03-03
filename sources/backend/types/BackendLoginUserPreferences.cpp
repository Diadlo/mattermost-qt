/**
 * @file BackendLoginUserPreferences.cpp
 * @brief
 * @author Lyubomir Filipov
 * @date Nov 28, 2021
 *
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

#include "BackendLoginUserPreferences.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <unordered_map>
#include <QVariant>
#include <sstream>
#include "log.h"

namespace Mattermost {

	BackendLoginUserPreferences::BackendLoginUserPreferences(const QJsonDocument& jsonObject)
	{
		static const std::unordered_map<QString, void(BackendLoginUserPreferences::*)(const QString&, const QString&)> preference_loaders =
		{
			{"favorite_channel", &BackendLoginUserPreferences::loadFavoriteChannel}
		};
		assert(jsonObject.isArray());

		for (const auto& entry : jsonObject.array())
		{
			const auto obj = entry.toObject();
			const auto category = obj.value("category").toString();
			const auto name = obj.value("name").toString();
			const auto value = obj.value("value").toString();
			
			auto it = preference_loaders.find(category);
			if (it != preference_loaders.end())
			{
				const auto loader = it->second;
				(this->*loader)(name, value);
			}
		}
	}
	const std::unordered_set<QString>& BackendLoginUserPreferences::getFavoriteChannels() const
	{
		return favoriteChannels;
	}
	void BackendLoginUserPreferences::loadFavoriteChannel(const QString& channel_name, const QString& value)
	{
		(void*)&value;
		favoriteChannels.insert(channel_name);
	}
};