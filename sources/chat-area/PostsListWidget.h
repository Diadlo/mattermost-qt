/**
 * @file PostsListWidget.h
 * @brief
 * @author Lyubomir Filipov
 * @date Dec 27, 2021
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

#include <QTimer>
#include "ResizableListWidget.h"
#include "post/PostWidget.h"

namespace Mattermost {

class PostWidget;

namespace ItemType {
enum id {
	post,
	separator,
};
}

class PostsListWidget: public ResizableListWidget {
	Q_OBJECT
public:
	explicit PostsListWidget (QWidget* parent);
	~PostsListWidget ();
public:
	void insertPost (int position, PostWidget* postWidget);
	void insertPost (PostWidget* postWidget);
	PostWidget* findPost (const QString& postId);
	int findPostByIndex (const QString& postId, int startIndex);

	void scrollToUnreadPostsOrBottom ();
	void addDaySeparator (int daysAgo);
	void addDaySeparator (int insertPos, int daysAgo);
	void addNewMessagesSeparator ();
	void removeNewMessagesSeparator ();
	void removeNewMessagesSeparatorAfterTimeout (int timeoutMs);
	QListWidgetItem* getLastOwnPost () const;
	void initiatePostEdit (QListWidgetItem& postItem);
	void postEditFinished ();

	/**
	 * If the widget is near the bottom when being resized,
	 * keep it at the bottom
	 */
	void resizeToBottom ();
	Backend*						backend;
signals:
	void postEditInitiated (BackendPost& post);
	void scrolledToTop ();
private:
	QList<QListWidgetItem*> sortedSelectedItems () const;

	void copySelectedItemsToClipboard (PostWidget::FormatType formatType);
	void keyPressEvent (QKeyEvent* event)		override;
	void focusOutEvent (QFocusEvent* event)		override;
	void showContextMenu (const QPoint &pos);
private:
	QTimer							removeNewMessagesSeparatorTimer;
	QListWidgetItem*				newMessagesSeparator;
	QListWidgetItem*				lastOwnPost;
	QListWidgetItem*				currentEditedItem;
	bool							menuShown;
};

} /* namespace Mattermost */
