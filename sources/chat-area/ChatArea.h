
#pragma once

#include <QWidget>
#include <QDate>
#include <QTreeWidgetItem>

#include "OutgoingPostCreator.h"

namespace Ui {
class ChatArea;
}

class QListWidgetItem;
class QVBoxLayout;

namespace Mattermost {

class BackendFile;
class Backend;
class BackendChannel;
class BackendPost;
class BackendUser;
class ChannelItem;
class ChannelNewPosts;
class OutgoingAttachmentList;

class ChatArea: public QWidget {
	Q_OBJECT
public:
	explicit ChatArea (Backend& backend, BackendChannel& channel, ChannelItem* treeItem, QWidget *parent = nullptr);
	~ChatArea();
public:
	Ui::ChatArea* getUi ();
	Backend& getBackend ();
	BackendChannel& getChannel ();
	void appendChannelPost (BackendPost& post);
	void fillChannelPosts (const ChannelNewPosts& newPosts);
	void handleUserTyping (const BackendUser& user);

	/**
	 * Called when the chat area is being selected from the channels menu (so that it's contents is shown)
	 */
	void onActivate ();

	/**
	 * Called when the main Mattermost window is being activated (gains focus).
	 * Called only if the chat area is the currently active one (so that it's contents is visible)
	 */
	void onMainWindowActivate ();

	QVBoxLayout& getAttachmentListParentWidget ();

	void addFileToload (BackendFile* file);
private:
	void dragEnterEvent (QDragEnterEvent* event) override;
	void dragMoveEvent (QDragMoveEvent* event) override;
	void dropEvent (QDropEvent* event) override;

	void setUserAvatar (const BackendUser& user);
	void moveOnListTop ();
	void setUnreadMessagesCount (uint32_t count);
	void setTextEditWidgetHeight (int height);
public:
	Ui::ChatArea 					*ui;
	Backend& 						backend;
	BackendChannel& 				channel;
	ChannelItem* 					treeItem;
	std::vector<BackendFile*>		filesToLoad;
	QString 						lastReadPostId;
	OutgoingPostCreator				outgoingPostCreator;

	uint32_t						unreadMessagesCount;
	int 							texteditDefaultHeight;
	QDate							lastPostDate;
	bool							gettingOlderPosts;
};

} /* namespace Mattermost */

