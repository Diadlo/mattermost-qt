/**
 * @file ChannelListForTeam.cpp
 * @brief
 * @author Lyubomir Filipov
 * @date Dec 27, 2021
 */

#include "ChannelListForTeam.h"

#include <QMenu>
#include "ChannelItemWidget.h"
#include "ChannelItem.h"
#include "chat-area/ChatArea.h"
#include "backend/Backend.h"

namespace Mattermost {

ChannelListForTeam::ChannelListForTeam (QTreeWidget& parent, Backend& backend, const QString& name, const QString& teamId)
:QTreeWidgetItem (&parent, QStringList() << name)
,backend (backend)
,teamId (teamId)
{
	QFont font;
	font.setBold (true);
	font.setPixelSize (14);
	setFont (0, font);

	setExpanded (true);
}

ChannelListForTeam::~ChannelListForTeam () = default;

void ChannelListForTeam::addChannel (BackendChannel& channel, QWidget *parent)
{
	ChannelItemWidget* itemWidget = new ChannelItemWidget (parent);
	itemWidget->setLabel (channel.display_name);

	ChannelItem* item = new ChannelItem (this, itemWidget);

	chatAreas.emplace_back(std::make_unique<ChatArea> (backend, channel, item, parent));
	ChatArea* chatArea = chatAreas.back ().get();

	item->setData(0, Qt::UserRole, QVariant::fromValue (chatArea));

	treeWidget()->setItemWidget (item, 0, itemWidget);
}

void ChannelListForTeam::showContextMenu (const QPoint& pos)
{
	// Create menu and insert some actions
	QMenu myMenu;

	myMenu.addAction ("ChannelListForTeam", [] {
		qDebug() << "ChannelListForTeam ";
	});

	myMenu.exec (pos);
}

} /* namespace Mattermost */

