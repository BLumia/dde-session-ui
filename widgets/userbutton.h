/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef USERBUTTON_H
#define USERBUTTON_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QtGui/QPaintEvent>
#include <QVBoxLayout>
#include <QObject>
#include <QLabel>
#include <QPropertyAnimation>
#include <com_deepin_daemon_accounts_user.h>

#include "useravatar.h"

using DBusUser = com::deepin::daemon::accounts::User;

static const int USER_ICON_WIDTH = 180;
static const int USER_ICON_HEIGHT = 180;

class UserButton:public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:
    UserButton(QWidget* parent=0);
    ~UserButton();

    enum AvatarSize {
        AvatarSmallSize = 90,
        AvatarLargerSize = 110,
    };

    bool selected() const;
    void setSelected(bool selected);
    const QString name() const;
    const QString avatar() const;
    const QString greeter() const;
    bool automaticLogin() const;
    const QStringList kbHistory();
    const QString kblayout();
    const QString displayName() const;
    const QStringList backgrounds() const;
    DBusUser *dbus() const;

signals:
    void imageClicked(QString nam);
    void opacityChanged();

public slots:
    void sendClicked();
    void setImageSize(const AvatarSize &avatarsize);
    void setButtonChecked(bool checked);

    void showButton();
    void hide(const int duration = 0);
    void move(const QPoint &position, const int duration = 0);
    void stopAnimation();

    double opacity();
    bool isChecked();
    void setOpacity(double opa);
    void setCustomEffect();
    void addTextShadowAfter();

    void updateUserName(const QString &username);
    void updateAvatar(const QString &avatar);
    void updateBackgrounds(const QStringList &list);
    void updateGreeterWallpaper(const QString &greeter);
    void updateDisplayName(const QString &displayname);
    void updateAutoLogin(bool autologin);
    void updateKbLayout(const QString &layout);
    void updateKbHistory(const QStringList &history);
    void setDBus(DBusUser *dbus);

protected:
    void paintEvent(QPaintEvent* event);
private:
    void initUI();
    void initConnect();
    void addTextShadow(bool isEffective);
    void updateUserDisplayName(const QString &name);

private:
    QString m_username;
    QString m_avatar;
    QStringList m_backgrounds;
    QString m_greeterWallpaper;
    QString m_kbLayout;
    QString m_displayName;
    bool m_isAutoLogin;
    QStringList m_kbHistory;
    DBusUser *m_dbus;

    bool m_selected = false;
    UserAvatar* m_userAvatar;
    QLabel* m_textLabel;
    QLabel *m_checkedMark;
    QHBoxLayout* m_buttonLayout;
    QHBoxLayout *m_nameLayout;
    QVBoxLayout* m_Layout;

    AvatarSize m_avatarsize = AvatarLargerSize;
    int m_borderWidth = 0;

    double m_opacity;

#ifndef DISABLE_ANIMATIONS
    QPropertyAnimation *m_moveAni;
    QPropertyAnimation* m_showAnimation;
    QPropertyAnimation* m_hideAnimation;
#endif

    QGraphicsOpacityEffect* m_opacityEffect;
};
#endif // UserButton

