
#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class MessageWidget;
}

namespace Mattermost {

class Backend;
class BackendPost;
class MessageAttachmentList;
class ChatArea;

class MessageWidget: public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget (Backend& backend, BackendPost &post, QWidget *parent, ChatArea* chatArea);
    ~MessageWidget();
public:
    QString getMessageTimeString (uint64_t timestamp);
    QString formatMessageText (const QString& str);

    BackendPost&							post;
private:
    Ui::MessageWidget*						ui;
    std::unique_ptr<MessageAttachmentList>	attachments;
};

} /* namespace Mattermost */

