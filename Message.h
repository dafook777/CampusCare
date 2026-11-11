
#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
using namespace std;

struct Message
{
    int messageId;
    int concernId; // 0 if this is a general admin message

    string senderId;
    string senderRole;

    string recipientId;
    string recipientRole;

    string messageText;
};

#endif
