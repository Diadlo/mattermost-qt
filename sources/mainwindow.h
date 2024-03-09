/**
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

#include <memory>
#include <QMainWindow>
#include <QSet>
#include "choose-emoji-dialog/ChooseEmojiDialogWrapper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QHBoxLayout;
class QVBoxLayout;
class QToolButton;
class QFrame;
class QLabel;
class QSystemTrayIcon;
class QTreeWidgetItem;
class QStackedWidget;
class QSplitter;

namespace Mattermost {

class ChannelTree;
class ChatArea;
class Backend;
class BackendChannel;
class BackendPost;
class BackendTeam;
class SettingsWindow;

class MainWindow: public QMainWindow {
	Q_OBJECT
public:
	MainWindow (QWidget *parent, QSystemTrayIcon& trayIcon, Backend& backend);
	~MainWindow();
public:
	void initializationComplete ();

	void changeEvent (QEvent* event) override;

	/**
	 * Called when the window's close buttons is clicked
	 * @param event event
	 */
	void closeEvent(QCloseEvent *event) override;

	/**
	 * Called when the window is about to be closed, either because of logout or reload action or quit action from the tray menu.
	 * In all cases it's state has to be saved
	 */
	void saveState ();

	/**
	 * Called when a new post is received while the Mattermost client is on
	 * @param channel channel
	 * @param post post
	 */
	void messageNotify (const BackendChannel& channel, const BackendPost& post);

	/**
	 * Called on Mattermost client startup, when there were new posts, while the client was not open
	 * @param channel channel
	 */
	void unreadMessagesNotify (const BackendChannel& channel);
	void setNotificationsCountVisualization (uint32_t notificationsCount);

	void moveEvent (QMoveEvent* event) override;
	void dragMoveEvent (QDragMoveEvent* event) override;
private:
	void createMenu ();
	void reload ();

	void setupUi();
	void setupTopLeftWidget();
	void retranslateUi();

private:
	QSystemTrayIcon&					trayIcon;
	ChooseEmojiDialogWrapper			chooseEmojiDialog;
	QSet<const BackendChannel*>			channelsWithNewPosts;
	Backend&							backend;
	bool								currentTeamRestoredFromSettings;
	QMenu*								mainMenu;
	SettingsWindow*						settingsWindow;
	bool								doDeinit;

	QSplitter *m_splitter;
    QAction *m_actionAbout;
    QGridLayout *m_gridLayout_2;
    QLabel *m_userIcon_label;
    QLabel *m_usernameLabel;
    QLabel *m_statusLabel;
    QToolButton *m_toolButton;

	// tl = top left
    QFrame *m_topLeft_frame;
    QHBoxLayout *m_tlHorizontalLayout;
    QVBoxLayout *m_tlVerticalLayout;

    Mattermost::ChannelTree *m_channelList;
    QStackedWidget *m_chatAreaStackedWidget;
    QMenuBar *m_menubar;
};

} /* namespace Mattermost */

