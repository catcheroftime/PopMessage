#include "PopupManage.h"

#include <QDebug>

PopupManage * PopupManage::m_popupManager = new PopupManage;
PopupManage::PopupManageGC PopupManage::popupManager_gc;

PopupManage::PopupManage()
{

}

PopupManage::~PopupManage()
{
    qDeleteAll(m_popupList);
    m_popupList.clear();
}

void PopupManage::addMessageShow(MessageShow *popup)
{
    m_popupList.append(popup);
}

void PopupManage::deleteMessageShow(MessageShow *popup)
{
    disconnect(popup, &MessageShow::sigClose, this, &PopupManage::deleteMessageShow);
    disconnect(popup, &MessageShow::sigClickUrl, this, &PopupManage::sigClickUrl);
    m_popupList.removeOne(popup);
    delete popup;
    popup = nullptr;
}

void PopupManage::notifyMessageShow()
{
    for(int i = 0; i < m_popupList.length();i++){
        m_popupList.at(i)->updatePosition();
    }
}

PopupManage *PopupManage::getInstance()
{
    return m_popupManager;
}

void PopupManage::setInfomation(QString titleInfo, QString msg, bool hidetitle)
{
    MessageShow * popup = new MessageShow();
    popup->setInfomation(titleInfo, msg, hidetitle);
    connect(popup, &MessageShow::sigClose, this, &PopupManage::deleteMessageShow);
    connect(popup, &MessageShow::sigClickUrl, this, &PopupManage::sigClickUrl);
    addMessageShow(popup);
    this->notifyMessageShow();

}

void PopupManage::setInfomation(QString titleInfo, QString msg, QString filepath, bool takeurl)
{
    MessageShow * popup = new MessageShow();
    popup->setInfomation(titleInfo, msg, filepath, takeurl);
    connect(popup, &MessageShow::sigClose, this, &PopupManage::deleteMessageShow);
    connect(popup, &MessageShow::sigClickUrl, this, &PopupManage::sigClickUrl);
    addMessageShow(popup);
    this->notifyMessageShow();
}
