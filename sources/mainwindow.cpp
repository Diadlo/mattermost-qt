/**
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

#include "mainwindow.h"

#include <QWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QFrame>
#include <QMenu>
#include <QHeaderView>
#include <QMenuBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QToolButton>
#include <QSplitter>
#include "channel-tree/ChannelTree.h"
#include "chat-area/ChatArea.h"
#include "backend/Backend.h"
#include "Settings.h"
#include "SettingsWindow.h"
#include "build-config.h"
#include "log.h"

namespace Mattermost {

MainWindow::MainWindow (QWidget *parent, QSystemTrayIcon& trayIcon, Backend& _backend)
:QMainWindow(parent)
,trayIcon (trayIcon)
,chooseEmojiDialog(Settings::getInstance(), this)
,backend (_backend)
,currentTeamRestoredFromSettings (false)
,doDeinit (false)
{
	LOG_DEBUG ("MainWindow create start");

	setupUi();
	m_channelList->setChatAreaStackedWidget (m_chatAreaStackedWidget);
	m_channelList->setFocus();

	createMenu ();

	const BackendUser& currentUser = backend.getLoginUser();

	if (currentUser.id.isEmpty()) {
		m_userIcon_label->clear();
		qCritical() << "Current User's ID is empty string";
		return;
	}

	connect (&currentUser, &BackendUser::onStatusChanged, [this, &currentUser] {
		m_statusLabel->setText (currentUser.status);
	});

	m_usernameLabel->setText (currentUser.username);

	connect (&currentUser, &BackendUser::onAvatarChanged, [this, &currentUser] {
		LOG_DEBUG ("Got User Image");
		QImage img = QImage::fromData (currentUser.avatar).scaled(42, 42, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		m_userIcon_label->setPixmap (QPixmap::fromImage(img));
	});

	/*
	 * Gets the LoginUser's image for the user icon
	 */
	backend.retrieveUserAvatar (currentUser.id);

	backend.retrieveTotalUsersCount ([this] (uint32_t) {
		backend.retrieveAllUsers ();
	});

	/*
	 * Register for signals
	 */
	//connect (m_channelList, &QTreeWidget::currentItemChanged, this, &MainWindow::channelListWidget_itemClicked);

	//getAllUsers is called from onShowEvent()
	connect (&backend, &Backend::onAllUsers, [this]() {
		/*
		 * Adds each team in which the LoginUser participates.
		 * The callback is called once for each team
		 */
		backend.retrieveOwnTeams ([this](BackendTeam& team) {
			m_channelList->addTeam (backend, team);
		});
	});

	/*
	 * After all team channels are received from the server, create tree items for the direct channels.
	 * For some reason the server duplicates the direct channels in each team.
	 * Here they are in a single list (The official Mattermost client shows them in each team, which IMHO looks like a total mess).
	 * So, all team channels have to be received in order to know all (unique) direct channels
	 *
	 * The list of users needs to be obtained too, because direct channels' names consist of user IDs,
	 * which need to be displayer ad user names
	 */
	connect (&backend, &Backend::onAllTeamChannelsPopulated, [this] {
		m_channelList->addFavoritesChannelsList(backend);
		m_channelList->addGroupChannelsList (backend);
		m_channelList->addDirectChannelsList (backend);

//		QSettings settings;
//		QString currentTeam (settings.value ("current_team", 0).toString());

		//Activate the same team that was active during the last session
//				if (teamChannelTree->team.id == currentTeam) {
//					LOG_DEBUG ("MainWindow activate team " << currentTeam);
//					//m_teamComboBox->setCurrentIndex (teamSeq);
//					//channelList.activateTeam (teamSeq);
//					currentTeamRestoredFromSettings = true;
//				}

		initializationComplete ();
	});

	connect (&backend, &Backend::onNewPost, [this] (BackendChannel& channel, const BackendPost& post) {
		this->messageNotify (channel, post);
	});

	connect (&backend, &Backend::onChannelViewed, [this] (const BackendChannel& channel) {
		if (channelsWithNewPosts.remove (&channel)) {
			setNotificationsCountVisualization (channelsWithNewPosts.size());
		}
	});

	/*
	 * onAddedToTeam comes after a WebSocket event, when the user is added to (new) team
	 */
	connect (&backend, &Backend::onAddedToTeam, [this](BackendTeam& team) {
		m_channelList->addTeam (backend, team);
	});

	/*
	 * On new post - set window and tray notifications
	 */
	connect (&backend, &Backend::onUnreadPostsAtStartup, this, &MainWindow::unreadMessagesNotify);

	LOG_DEBUG ("MainWindow signal register finish");

	//Restore saved window position and dimensions
	restoreGeometry(Settings::getInstance().getWindowGeometry());

	connect (qApp, &QApplication::aboutToQuit, this, &MainWindow::saveState);
	LOG_DEBUG ("MainWindow create finish");
}

void MainWindow::setupTopLeftWidget()
{
	m_topLeft_frame = new QFrame();
	m_topLeft_frame->setFrameShape(QFrame::NoFrame);
	m_topLeft_frame->setFrameShadow(QFrame::Raised);
	m_topLeft_frame->setLineWidth(0);

	m_tlHorizontalLayout = new QHBoxLayout(m_topLeft_frame);
	m_tlHorizontalLayout->setSpacing(2);
	m_tlHorizontalLayout->setContentsMargins(2, 0, 0, 0);

	m_userIcon_label = new QLabel(m_topLeft_frame);
	QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(m_userIcon_label->sizePolicy().hasHeightForWidth());
	m_userIcon_label->setSizePolicy(sizePolicy);
	m_userIcon_label->setMinimumSize(QSize(42, 42));
	m_userIcon_label->setMaximumSize(QSize(42, 42));
	m_userIcon_label->setFrameShape(QFrame::Box);

	m_tlHorizontalLayout->addWidget(m_userIcon_label);

	m_tlVerticalLayout = new QVBoxLayout();
	m_usernameLabel = new QLabel(m_topLeft_frame);

	m_tlVerticalLayout->addWidget(m_usernameLabel);

	m_statusLabel = new QLabel(m_topLeft_frame);

	m_tlVerticalLayout->addWidget(m_statusLabel);
	m_tlHorizontalLayout->addLayout(m_tlVerticalLayout);

	m_toolButton = new QToolButton(m_topLeft_frame);
	m_toolButton->setObjectName("toolButton");
	m_toolButton->setIcon(QIcon(":/icons/burger"));
	m_toolButton->setPopupMode(QToolButton::InstantPopup);

	m_tlHorizontalLayout->addWidget(m_toolButton);
}

void MainWindow::setupUi()
{
	resize(800, 600);
	setWindowIcon(QIcon(":/icons/img/icon0.ico"));

	m_actionAbout = new QAction(this);
	m_splitter = new QSplitter(this);
	QWidget *leftPaneWidget = new QWidget(m_splitter);
	leftPaneWidget->setMinimumWidth(200);
	leftPaneWidget->setMaximumWidth(300);
	QVBoxLayout *leftPaneLayout = new QVBoxLayout(leftPaneWidget);
	m_splitter->addWidget(leftPaneWidget);
	m_splitter->setCollapsible(0, false);
	setupTopLeftWidget();
	leftPaneLayout->addWidget(m_topLeft_frame);

	m_channelList = new Mattermost::ChannelTree();
	QTreeWidgetItem *treeItem = new QTreeWidgetItem();
	treeItem->setText(1, QString::fromUtf8("2"));
	treeItem->setText(0, QString::fromUtf8("1"));
	m_channelList->setHeaderItem(treeItem);
	m_channelList->setContextMenuPolicy(Qt::CustomContextMenu);
	m_channelList->setStyleSheet(
		"QTreeWidget::branch:!has-children {\n"
		"	border: none;\n"
		"}\n"
		"\n"
		"QTreeWidget::branch:open:has-children {\n"
		"	image: url(:/img/arrow_expanded.png)\n"
		"}\n"
		"\n"
		"QTreeWidget::branch:closed:has-children {\n"
		"	image: url(:/img/arrow_collapsed.png)\n"
		"}\n"
	);
	m_channelList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_channelList->setIconSize(QSize(20, 20));
	m_channelList->setIndentation(12);
	m_channelList->setColumnCount(2);
	m_channelList->header()->setVisible(false);
	m_channelList->header()->setMinimumSectionSize(0);
	m_channelList->header()->setDefaultSectionSize(30);
	m_channelList->header()->setStretchLastSection(false);

	leftPaneLayout->addWidget(m_channelList);

	m_chatAreaStackedWidget = new QStackedWidget(m_splitter);

	m_splitter->addWidget(m_chatAreaStackedWidget);

	setCentralWidget(m_splitter);
	m_menubar = new QMenuBar(this);
	m_menubar->setGeometry(QRect(0, 0, 800, 32));
	setMenuBar(m_menubar);
	QWidget::setTabOrder(m_channelList, m_toolButton);

	retranslateUi();

	QMetaObject::connectSlotsByName(this);
}

void MainWindow::retranslateUi()
{
	this->setWindowTitle(tr("Mattermost", "Main window title"));
	m_actionAbout->setText(tr("About", "Action in menu"));
	m_statusLabel->clear();
	m_toolButton->setText("...");
}

MainWindow::~MainWindow()
{
}

static QString infoText (QString ("Version " PROJECT_VER "<br/>"
"An unofficial Mattermost Client, using the QT framework<br/>") +
R"(
<br/>
More information:<br/>
<a href='https://github.com/nuclear868/mattermost-qt'>https://github.com/nuclear868/mattermost-qt</a>
<br/>
<br/>
Mattermost QT Copyright 2021-2023 Lyubomir Filipov<br/>
<br/>
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.<br/>
<br/>
You should have received a copy of the GNU Lesser General Public License
along with Mattermost-QT. if not, see <a href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a>.<br/>
)");

void MainWindow::createMenu ()
{
	mainMenu = new QMenu (m_toolButton);

	QMenu* fileMenu = mainMenu->addMenu ("File");
	fileMenu->addAction ("Logout", [this] {

		backend.logout ([this] {
			doDeinit = true;
			QMainWindow::close ();
			LOG_DEBUG ("Logout done");
		});
	});

	mainMenu->addAction ("Settings", [this] {
		Settings& settings = Settings::getInstance();
		settingsWindow = new SettingsWindow(settings, this);

		connect (settingsWindow, &QDialog::accepted, [this] {

			if (QMessageBox::question (this, "Reload?", "In order to apply some settings, Mattermost has to be reloaded.\n"
					" Do you want to reload now? (If no, settings will be applied on the next startup)") == QMessageBox::Yes) {

				settingsWindow->applyNewSettings ();
			}
			reload ();
		});

		settingsWindow->show();
	});


	QMenu* helpMenu = mainMenu->addMenu ("Help");
	helpMenu->addAction ("About Mattermost", [this] {
		QMessageBox *msgBox = new QMessageBox (QMessageBox::Information,
				"About Mattermost", infoText);

		msgBox->setIconPixmap (windowIcon().pixmap (QSize (64, 64)));
		msgBox->setTextFormat(Qt::RichText);
		msgBox->setStandardButtons(QMessageBox::Ok);
		msgBox->setDefaultButton(QMessageBox::Ok);
		msgBox->open();
	});

	helpMenu->addAction ("About QT", [this] {
		QMessageBox::aboutQt (this, "About QT");
	});

	m_toolButton->setMenu(mainMenu);
}

void MainWindow::moveEvent (QMoveEvent*)
{

	ChatArea* currentPage = m_channelList->getCurrentPage();

	if (currentPage) {
		currentPage->onMove (pos());
	}
}

void MainWindow::dragMoveEvent (QDragMoveEvent*)
{
	qDebug() << "dragMove " << mapToGlobal(pos());
}

void MainWindow::reload ()
{
	QTimer::singleShot(0, [this] {
		backend.reset();
		doDeinit = true;
		QMainWindow::close ();
	});
}

void MainWindow::changeEvent (QEvent* event)
{
	QWidget::changeEvent(event);

	if (event->type() == QEvent::ActivationChange) {
		if (isActiveWindow()) {
			//qDebug() << "Activated";

			ChatArea* currentPage = m_channelList->getCurrentPage();

			if (currentPage) {
				currentPage->onMainWindowActivate ();

				if (channelsWithNewPosts.remove (&currentPage->getChannel())) {
					setNotificationsCountVisualization (channelsWithNewPosts.size());
				}
			}

		} else {
			//qDebug() << "Deactivated";
		}
	} else {
		qDebug() << event->type();
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	qDebug() << "closeEvent";

	if (doDeinit) {
		qDebug() << "QMainWindow closeEvent";
		saveState ();
		return QMainWindow::closeEvent (event);
	}

	if (Settings::getInstance().getCloseToTray()) {
		hide();
		event->ignore();
	}
}

void MainWindow::initializationComplete ()
{
	LOG_DEBUG ("MainWindow initialization comlete");

	/*
	 * No team was restored from settings. Either there was no team saved, or the saved team
	 * was deleted. In all cases, activate the first team
	 */
	if (!currentTeamRestoredFromSettings) {
	//	m_teamComboBox->setCurrentIndex (0);
		//channelList.activateTeam (0);
	}
}

void MainWindow::messageNotify (const BackendChannel& channel, const BackendPost& post)
{
	//do not receive notifications for your own messages ;)
	if (post.author && post.author->id == backend.getLoginUser().id) {
		return;
	}

	/**
	 * If the Mattermost window is active (has focus) and the current channel is active,
	 * do not add notifications. We assume that the user is watching the chat window
	 */
	if (isActiveWindow() && m_channelList->isChannelActive (channel)) {
		return;
	}

	//Add a desktop notification
	QString title;

	if (channel.type == BackendChannel::directChannel) {
		title = post.getDisplayAuthorName () + " messaged you";
	} else {
		title = post.getDisplayAuthorName () + " posted in '" + channel.display_name + "'";
	}

	trayIcon.showMessage (title, post.message, QSystemTrayIcon::Information);
	qApp->alert (nullptr, 0);

	//update the count of new channels in the taskbar and tray icon
	channelsWithNewPosts.insert(&channel);
	setNotificationsCountVisualization (channelsWithNewPosts.size());
}

void MainWindow::unreadMessagesNotify (const BackendChannel& channel)
{
	//update the count of new channels in the taskbar and tray icon
	channelsWithNewPosts.insert(&channel);
	setNotificationsCountVisualization (channelsWithNewPosts.size());
}

void MainWindow::setNotificationsCountVisualization (uint32_t notificationsCount)
{
	//set the count in the window's taskbar element
	if (notificationsCount == 0) {
		setWindowTitle (qApp->applicationName());
	} else {
		setWindowTitle ("(" + QString::number (channelsWithNewPosts.size()) + ") " + qApp->applicationName());
	}

	//set the count in the tray icon
	notificationsCount = std::min (notificationsCount, 6u);
	QString iconName (":/icons/img/icon" + QString::number(notificationsCount) + ".ico");
	trayIcon.setIcon(QIcon(iconName));
}

void MainWindow::saveState ()
{
	LOG_DEBUG ("MainWindow saveState");
	Settings::getInstance().setWindowGeometry(saveGeometry());
//	settings.setValue ("current_team", channelList.getCurrentTeamId());
//	if (currentPage) {
//		settings.setValue ("current_channel", currentPage->getChannel().id);
//	}
}

} /* namespace Mattermost */




