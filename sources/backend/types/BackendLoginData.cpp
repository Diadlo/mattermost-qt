/**
 * @file BackendLoginData.cpp
 * @brief
 * @author Lyubomir Filipov
 * @date Dec 5, 2021
 *
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

#include "BackendLoginData.h"
#include "Settings.h"

namespace Mattermost {

void BackendLoginData::loadFromSettings(const Settings& settings)
{
	domain = settings.getDomain();
	username = settings.getUsername();
	token = settings.getToken();
}

void BackendLoginData::saveToSettings(Settings& settings) const
{
	settings.setDomain(domain);
	settings.setUsername(username);
	settings.setToken(token);
}

bool BackendLoginData::areAllFieldsFilled () const
{
	return !domain.isEmpty()
			&& !token.isEmpty();
}

} /* namespace Mattermost */

