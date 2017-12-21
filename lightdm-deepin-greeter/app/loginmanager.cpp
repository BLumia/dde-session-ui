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

#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>
#include <QDesktopWidget>
#include <QDebug>

#include "loginmanager.h"
#include "dbus/dbuslockservice.h"
#include "dbus/dbusaccounts.h"

#include <com_deepin_daemon_accounts_user.h>
#include <libintl.h>
#include <X11/Xlib-xcb.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include "otheruserinput.h"

#define LOCKSERVICE_PATH "/com/deepin/dde/LockService"
#define LOCKSERVICE_NAME "com.deepin.dde.LockService"

using DBusUser = com::deepin::daemon::accounts::User;

//Load the System cursor --begin
static XcursorImages*
xcLoadImages(const char *image, int size)
{
    QSettings settings(DDESESSIONCC::DEFAULT_CURSOR_THEME, QSettings::IniFormat);
    //The default cursor theme path
    qDebug() << "Theme Path:" << DDESESSIONCC::DEFAULT_CURSOR_THEME;
    QString item = "Icon Theme";
    const char* defaultTheme = "";
    QVariant tmpCursorTheme = settings.value(item + "/Inherits");
    if (tmpCursorTheme.isNull()) {
        qDebug() << "Set a default one instead, if get the cursor theme failed!";
        defaultTheme = "Deepin";
    } else {
        defaultTheme = tmpCursorTheme.toString().toLocal8Bit().data();
    }

    qDebug() << "Get defaultTheme:" << tmpCursorTheme.isNull()
             << defaultTheme;
    return XcursorLibraryLoadImages(image, defaultTheme, size);
}

static unsigned long loadCursorHandle(Display *dpy, const char *name, int size)
{
    if (size == -1) {
        size = XcursorGetDefaultSize(dpy);
    }

    // Load the cursor images
    XcursorImages *images = NULL;
    images = xcLoadImages(name, size);

    if (!images) {
        images = xcLoadImages(name, size);
        if (!images) {
            return 0;
        }
    }

    unsigned long handle = (unsigned long)XcursorImagesLoadCursor(dpy,
                          images);
    XcursorImagesDestroy(images);

    return handle;
}

static int set_rootwindow_cursor() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        qDebug() << "Open display failed";
        return -1;
    }

    Cursor cursor = (Cursor)XcursorFilenameLoadCursor(display, "/usr/share/icons/deepin/cursors/loginspinner");
    if (cursor == 0) {
        cursor = (Cursor)loadCursorHandle(display, "watch", 24);
    }
    XDefineCursor(display, XDefaultRootWindow(display),cursor);

    // XFixesChangeCursorByName is the key to change the cursor
    // and the XFreeCursor and XCloseDisplay is also essential.

    XFixesChangeCursorByName(display, cursor, "watch");

    XFreeCursor(display, cursor);
    XCloseDisplay(display);

    return 0;
}
// Load system cursor --end

LoginManager::LoginManager(QWidget* parent)
    : QFrame(parent),
      m_keyboardMonitor(KeyboardMonitor::instance())
{
    setAttribute(Qt::WA_TranslucentBackground);

    initUI();
    initData();
    initConnect();
    initDateAndUpdate();

    QFile file("/tmp/lastuser");
    if (file.open(QIODevice::ReadWrite)) {
        m_lastUser = file.readAll().trimmed();
        file.resize(0);
        qDebug() << "file remove: " << int(file.remove());
    }

    if (!m_lastUser.isEmpty()) {
        m_userWidget->setCurrentUser(m_lastUser);
        m_sessionWidget->switchToUser(m_lastUser);
    }

    m_keyboardMonitor->start(QThread::LowestPriority);
    QTimer::singleShot(0, this, &LoginManager::restoreNumlockStatus);

    const QString u = m_userWidget->currentUser();
    qDebug() << Q_FUNC_INFO << "current user: " << u;

    m_controlWidget->chooseToSession(m_sessionWidget->currentSessionName());

    // NOTE(kirigaya): avoid missing settings
    QTimer::singleShot(1000, this, [=] {
        const QString &user = m_userWidget->currentUser();
        updateUserLoginCondition(user);
        updateBackground(user);
    });
}

void LoginManager::updateWidgetsPosition()
{
    const int h = height();
    const int w = width();

    m_userWidget->setFixedWidth(w);
    m_userWidget->move(0, (h - m_userWidget->height()) / 2 - 95); // center and margin-top: -95px
    qDebug() << "Change Screens" << m_userWidget->isChooseUserMode;
    m_sessionWidget->setFixedWidth(w);
    m_sessionWidget->move(0, (h - m_sessionWidget->height()) / 2 - 70); // 中间稍往上的位置
    m_logoWidget->move(48, h - m_logoWidget->height() - 36); // left 48px and bottom 36px
    m_controlWidget->move(w - m_controlWidget->width(), h - m_controlWidget->height() - 36);
    m_passWdEdit->move((w - m_passWdEdit->width()) / 2, (h - m_passWdEdit->height()) / 2);
    m_requireShutdownWidget->setFixedWidth(w);
    m_requireShutdownWidget->setFixedHeight(300);
    m_requireShutdownWidget->move(0,  (h - m_requireShutdownWidget->height())/2 - 60);
}

void LoginManager::updateBackground(QString username)
{
    emit requestBackground(m_userWidget->getUserGreeterBackground(username));
}

void LoginManager::updateUserLoginCondition(QString username)
{
    m_loginButton->setVisible(false);
    m_passWdEdit->setVisible(false);

    QProcess p;
    p.start("groups", QStringList(username));
    p.waitForFinished();

    QString output = p.readAllStandardOutput().trimmed();
    QStringList tokens = output.split(":");
    if (tokens.length() > 1) {
        QStringList groups = tokens.at(1).split(" ");
        qDebug() << groups;

        if (groups.contains("nopasswdlogin")) {
            m_loginButton->show();
            m_loginButton->setFocus();
            m_userState = NoPassword;
        } else {
            m_greeter->authenticate(m_userWidget->currentUser());
            m_passWdEdit->show();
            m_userState = Password;
        }
    }
}

void LoginManager::startSession()
{
    qDebug() << "start session = " << m_sessionWidget->currentSessionName();

    // NOTE(kirigaya): It is not necessary to display the login animation.

    hide();

    const QStringList &wallpaper = m_userWidget->getUserDesktopBackground(m_userWidget->currentUser());

    if (!wallpaper.isEmpty()) {
        // NOTE(kirigaya): Login animation needs to be transitioned to the user's desktop wallpaper
        const QString &desktop = wallpaper.first();
        QUrl url(desktop);
        QPixmap pixmap(url.toLocalFile());

        if (pixmap.isNull())
            pixmap.load(desktop);

        emit requestBackground(pixmap);
    }

    QTimer::singleShot(1000, this, [=] {
        // NOTE(kirigaya): Login animation duration is 1s.
        m_userWidget->saveLastUser();

        m_greeter->startSessionSync(m_sessionWidget->currentSessionKey());
    });
}

void LoginManager::resizeEvent(QResizeEvent *e)
{
    updateWidgetsPosition();

    QWidget::resizeEvent(e);
}

void LoginManager::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (!m_requireShutdownWidget->isHidden()) {
            m_requireShutdownWidget->hide();
            m_userWidget->show();
            updateUserLoginCondition(m_userWidget->currentUser());
        }

        if (m_keybdArrowWidget->isHidden()) {
            m_keybdArrowWidget->hide();
        }
    }
}

void LoginManager::initUI()
{
//    setFixedSize(qApp->desktop()->size());
    setObjectName("LoginManagerTool");

    m_controlWidget = new ControlWidget(this);

    m_sessionWidget = new SessionWidget(this);
    m_sessionWidget->setFixedHeight(200);
//    m_sessionWidget->setFixedWidth(qApp->primaryScreen()->geometry().width());
    m_sessionWidget->hide();
    m_logoWidget = new LogoWidget(this);
    m_userWidget = new UserWidget(this);

    m_userWidget->setObjectName("UserWidget");

    m_passWdEdit = new PassWdEdit(this);
    m_passWdEdit->setEnterBtnStyle(":/img/action_icons/login_normal.svg",
                                   ":/img/action_icons/login_hover.svg",
                                   ":/img/action_icons/login_press.svg");
    m_passWdEdit->setFocusPolicy(Qt::StrongFocus);
    m_passWdEdit->setVisible(false);

    m_loginButton = new QPushButton(this);
    m_loginButton->setText(tr("Login"));
    m_loginButton->setFixedSize(160, 36);
    m_loginButton->setVisible(false);
    m_loginButton->setFocusPolicy(Qt::StrongFocus);
    m_loginButton->setDefault(true);

    m_requireShutdownWidget = new ShutdownWidget(this);
    m_requireShutdownWidget->hide();
    m_passWdEditLayout = new QHBoxLayout;
    m_passWdEditLayout->setMargin(0);
    m_passWdEditLayout->setSpacing(0);
    m_passWdEditLayout->addStretch();
    m_passWdEditLayout->addWidget(m_passWdEdit);
    m_passWdEditLayout->addWidget(m_loginButton);
    m_passWdEditLayout->addStretch();

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addLayout(m_passWdEditLayout);

    m_otherUserInput = new OtherUserInput(this);
    m_otherUserInput->hide();
    m_Layout->addWidget(m_otherUserInput, 0, Qt::AlignCenter);
    m_Layout->addStretch();

//    QWidget *centralWidget = new QWidget;
//    centralWidget->setLayout(m_Layout);

//    setContent(centralWidget);
    setLayout(m_Layout);

    m_passWdEdit->updateKeyboardStatus();
    keyboardLayoutUI();

    m_controlWidget->setUserSwitchEnable(m_userWidget->count() > 1);
    m_controlWidget->setSessionSwitchEnable(m_sessionWidget->sessionCount() > 1);
#ifndef SHENWEI_PLATFORM
    updateStyle(":/skin/login.qss", this);
#endif
    set_rootwindow_cursor();
}

//void LoginManager::recordPid() {
//    qDebug() << "remember P i D" << qApp->applicationPid();

//    QFile tmpPidFile(QString("%1%2").arg("/tmp/").arg(".dgreeterpid"));

//    if (tmpPidFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
//        QTextStream pidInfo(&tmpPidFile);
//        pidInfo << qApp->applicationPid();

//        tmpPidFile.close();
//    } else {
//        qDebug() << "file open failed!";
//    }
//}

void LoginManager::initData() {
    m_greeter = new QLightDM::Greeter(this);
    if (!m_greeter->connectSync())
        qWarning() << "greeter connect fail !!!";
}

void LoginManager::initConnect()
{
    connect(m_controlWidget, &ControlWidget::requestSwitchUser, this, &LoginManager::chooseUserMode);
    connect(m_controlWidget, &ControlWidget::requestSwitchUser, m_userWidget, &UserWidget::expandWidget, Qt::QueuedConnection);
    connect(m_controlWidget, &ControlWidget::requestShutdown, this, &LoginManager::showShutdownFrame);
    connect(m_controlWidget, &ControlWidget::requestSwitchSession, this, &LoginManager::chooseSessionMode);

    connect(m_sessionWidget, &SessionWidget::sessionChanged, this, &LoginManager::choosedSession);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, m_controlWidget, &ControlWidget::chooseToSession, Qt::QueuedConnection);

    connect(m_passWdEdit, &PassWdEdit::submit, this, [=] {
        m_accountStr = m_userWidget->currentUser();
        m_passwdStr = m_passWdEdit->getText();
        m_greeter->authenticate(m_accountStr);

        // NOTE(kirigaya): set auth user and respond password need some time!
        QTimer::singleShot(100, this, SLOT(login()));
    });
    connect(m_userWidget, &UserWidget::userChanged, [&](const QString username) {

        qDebug()<<"selected user: " << username;
        qDebug()<<"previous selected user: " << m_sessionWidget->currentSessionOwner();

        qDebug() << username << m_sessionWidget->currentSessionOwner();

        updateUserLoginCondition(username);

        if (username == m_sessionWidget->currentSessionOwner())
            return;

        // goto previous lock
        if (m_userWidget->getLoggedInUsers().contains(username))
        {
            QProcess *process = new QProcess;
            connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), process, &QProcess::deleteLater);
            process->start("dde-switchtogreeter " + username);

            m_userWidget->setCurrentUser(m_sessionWidget->currentSessionOwner());

            return;
        }

        updateBackground(username);

        m_sessionWidget->switchToUser(username);
    });

    connect(m_greeter, &QLightDM::Greeter::showPrompt, this, &LoginManager::prompt);
    connect(m_greeter, &QLightDM::Greeter::showMessage, this, &LoginManager::message);
    connect(m_greeter, &QLightDM::Greeter::authenticationComplete, this, &LoginManager::authenticationComplete);

    connect(m_passWdEdit, &PassWdEdit::updateKeyboardStatus, this, &LoginManager::keyboardLayoutUI);
    connect(m_passWdEdit, &PassWdEdit::keybdLayoutButtonClicked, this, &LoginManager::keybdLayoutWidgetPosit);

    connect(m_requireShutdownWidget, &ShutdownWidget::shutDownWidgetAction, this, &LoginManager::setShutdownAction);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginManager::login);

    connect(m_keyboardMonitor, &KeyboardMonitor::numlockStatusChanged, this, &LoginManager::saveNumlockStatus);

    connect(m_otherUserInput, &OtherUserInput::submit, this, [=] {
        m_accountStr = m_otherUserInput->account();
        m_passwdStr = m_otherUserInput->passwd();
        m_greeter->authenticate(m_accountStr);

        // NOTE(kirigaya): set auth user and respond password need some time!
        QTimer::singleShot(100, this, SLOT(login()));
    });

    connect(m_userWidget, &UserWidget::otherUserLogin, this, [=] {
        m_otherUserInput->show();
        m_passWdEdit->hide();
        m_loginButton->hide();
        m_sessionWidget->hide();
        m_userWidget->hide();
        m_requireShutdownWidget->hide();
        m_greeter->authenticate("");
    });

    connect(m_userWidget, &UserWidget::currentUserBackgroundChanged,
            this, static_cast<void (LoginManager::*)(const QString &) const>(&LoginManager::requestBackground));
}

void LoginManager::initDateAndUpdate() {
    /*Update the m_passWdEdit UI after get the current user
    *If the current user owns multi-keyboardLayouts,
    *then the button to choose keyboardLayout need show
    */

    //Get the session of current user
    m_sessionWidget->switchToUser(m_userWidget->currentUser());
    //To control the expanding the widgets of all the user list(m_userWidget)
    expandUserWidget();

    //The dbus is used to control the power actions
    m_login1ManagerInterface =new DBusLogin1Manager("org.freedesktop.login1", "/org/freedesktop/login1", QDBusConnection::systemBus(), this);
    if (!m_login1ManagerInterface->isValid()) {
        qDebug() <<"m_login1ManagerInterface:" << m_login1ManagerInterface->lastError().type();
    }
}

void LoginManager::expandUserWidget() {
//    int expandState = m_utilFile->getExpandState();
//    qDebug() << "expandState:" << expandState;
//    if (expandState == 1) {
//        qDebug() << "expandState:" << expandState;
//        QMetaObject::invokeMethod(m_controlWidget, "requestSwitchUser", Qt::QueuedConnection);
//    }

//    m_utilFile->setExpandState(0);
}

void LoginManager::message(QString text, QLightDM::Greeter::MessageType type)
{
    qDebug() << "pam message: " << text << type;

    if (text == "Verification timed out") {
        m_isThumbAuth = true;
        m_passWdEdit->setMessage(tr("Please enter your password manually if fingerprint password timed out"));
        return;
    }

    switch (type) {
    case QLightDM::Greeter::MessageTypeInfo:
        if (m_isThumbAuth)
            break;

        m_passWdEdit->setMessage(QString(dgettext("fprintd", text.toLatin1())));
        break;
    case QLightDM::Greeter::MessageTypeError:
        qWarning() << "error message from lightdm: " << text;
        if (text == "Failed to match fingerprint") {
            m_passWdEdit->setAlert(true, tr("Failed to match fingerprint"));
            m_passWdEdit->setMessage("");
        }
        break;
    default:
        break;
    }
}

void LoginManager::prompt(QString text, QLightDM::Greeter::PromptType type)
{
    qDebug() << "pam prompt: " << text << type;

    // Don't show password prompt from standard pam modules since
    // we'll provide our own prompt or just not.
    const QString msg = text == "Password: " ? "" : text;

    switch (type)
    {
    case QLightDM::Greeter::PromptTypeSecret:
        if (m_isThumbAuth)
            return;

        if (msg.isEmpty() && !m_passwdStr.isEmpty())
            m_greeter->respond(m_passwdStr);

        if (!msg.isEmpty())
            m_passWdEdit->setMessage(msg);
        break;
    case QLightDM::Greeter::PromptTypeQuestion:
        // trim the right : in the message if exists.
        m_passWdEdit->setMessage(text.replace(":", ""));
        break;
    default:
        break;
    }
}

void LoginManager::authenticationComplete()
{
    qDebug() << "authentication complete, authenticated " << m_greeter->isAuthenticated();

    if (!m_greeter->isAuthenticated() && m_userState == Password) {
        m_passWdEdit->selectAll();
        m_passWdEdit->setAlert(true, tr("Wrong Password"));
        return;
    }

    QTimer::singleShot(100, this, SLOT(startSession()));
}

void LoginManager::login()
{
    m_isThumbAuth = false;

    if(!m_requireShutdownWidget->isHidden()) {
        qDebug() << "SHUTDOWN";
        m_requireShutdownWidget->shutdownAction();
        return;
    }

    if (!m_sessionWidget->isHidden()) {
        qDebug() << "SESSIONWIDGET";
//        m_sessionWidget->chooseSession();
        return;
    }
    if (m_userWidget->isChooseUserMode && !m_userWidget->isHidden()) {
        m_userWidget->chooseButtonChecked();
        qDebug() << "lineEditGrabKeyboard";
        return;
    }

    if (m_passwdStr.isEmpty() && m_userState == Password)
        return;

    if (m_greeter->isAuthenticated()) {
        startSession();
    } else if (m_greeter->inAuthentication() && m_userState == Password) {
        m_greeter->respond(m_passwdStr);
    } else {
        authenticate();
    }
}

void LoginManager::authenticate()
{
    if (m_greeter->inAuthentication())
        m_greeter->cancelAuthentication();

    m_greeter->authenticate(m_accountStr);

    qDebug() << "start authentication of user: " << m_greeter->authenticationUser();
}

void LoginManager::chooseUserMode()
{
    m_passWdEdit->hide();
    m_loginButton->hide();
    m_sessionWidget->hide();
    m_userWidget->show();
    m_requireShutdownWidget->hide();
    m_otherUserInput->hide();
}

void LoginManager::chooseSessionMode()
{
    m_sessionWidget->show();
    m_userWidget->hide();
    m_passWdEdit->hide();
    m_loginButton->hide();
    m_otherUserInput->hide();
    m_requireShutdownWidget->hide();
    if (!m_keybdArrowWidget->isHidden()) {
        m_keybdArrowWidget->hide();
    }
}

void LoginManager::choosedSession() {
    m_requireShutdownWidget->hide();
    m_sessionWidget->hide();
    m_userWidget->show();

    updateUserLoginCondition(m_userWidget->currentUser());

    if (!m_keybdArrowWidget->isHidden()) {
        m_keybdArrowWidget->hide();
    }
}

void LoginManager::showShutdownFrame() {
    qDebug() << "showShutdownFrame!";
    m_userWidget->hide();
    m_passWdEdit->hide();
    m_loginButton->hide();
    m_sessionWidget->hide();
    m_requireShutdownWidget->show();
    m_otherUserInput->hide();
}

void LoginManager::keyboardLayoutUI() {
    m_passWdEdit->updateKeybdLayoutUI(m_userWidget->getUserKBHistory(m_userWidget->currentUser()));
    m_keybdLayoutWidget = new KbLayoutWidget;
    m_keybdLayoutWidget->updateButtonList(m_userWidget->getUserKBHistory(m_userWidget->currentUser()));
    m_keybdLayoutWidget->setDefault(m_userWidget->getUserKBLayout(m_userWidget->currentUser()));

    m_keybdArrowWidget = new DArrowRectangle(DArrowRectangle::ArrowTop, this);
    m_keybdArrowWidget->setBackgroundColor(QColor::fromRgbF(1, 1, 1, 0.15));
    m_keybdArrowWidget->setBorderColor(Qt::transparent);
    m_keybdArrowWidget->setArrowX(17);
    m_keybdArrowWidget->setArrowWidth(12);
    m_keybdArrowWidget->setArrowHeight(6);
    m_keybdArrowWidget->setMargin(1);

    m_keybdArrowWidget->setContent(m_keybdLayoutWidget);
    m_keybdLayoutWidget->setParent(m_keybdArrowWidget);
    m_keybdLayoutWidget->show();
    m_keybdArrowWidget->move(m_passWdEdit->x() + 123, m_passWdEdit->y() + m_passWdEdit->height() - 15);

    m_keybdArrowWidget->hide();

    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, this, &LoginManager::setCurrentKeybdLayoutList);
    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, m_keybdArrowWidget, &DArrowRectangle::hide);
    connect(m_userWidget, &UserWidget::chooseUserModeChanged, m_passWdEdit, &PassWdEdit::recordUserPassWd);
}

void LoginManager::setCurrentKeybdLayoutList(QString keyboard_value)
{
    m_userWidget->setUserKBlayout(m_userWidget->currentUser(), keyboard_value);
}

void LoginManager::keybdLayoutWidgetPosit() {
    if (m_keybdArrowWidget->isHidden()) {
        const QPoint p(m_keybdArrowWidget->rect().x() + 45, m_keybdArrowWidget->rect().y() + 21);

        m_keybdArrowWidget->setCornerPoint(p);

        const int x = m_passWdEdit->x() + m_keybdArrowWidget->width() / 2 - 22;

        m_keybdArrowWidget->show(x, m_passWdEdit->y() + m_passWdEdit->height() - 15);

    } else {
        m_keybdArrowWidget->hide();
    }
}

void LoginManager::setShutdownAction(const ShutdownWidget::Actions action) {

    switch (action) {
        case ShutdownWidget::RequireShutdown: { m_login1ManagerInterface->PowerOff(true); break;}
        case ShutdownWidget::RequireRestart: { m_login1ManagerInterface->Reboot(true); break;}
        case ShutdownWidget::RequireSuspend: { m_login1ManagerInterface->Suspend(true);

            m_requireShutdownWidget->hide();
            m_userWidget->show();
            m_sessionWidget->hide();
            updateUserLoginCondition(m_userWidget->currentUser());
        break;}
        default:;
    }
}

void LoginManager::saveNumlockStatus(const bool &on)
{
    qDebug() << "save numlock statuc to " << on;
    QSettings settings;
    settings.beginGroup("NumlockStatus");
    settings.setValue(m_userWidget->currentUser(), on);
    settings.endGroup();
}

void LoginManager::restoreNumlockStatus()
{
    bool value = false;

    QSettings settings;
    settings.beginGroup("NumlockStatus");
    value = settings.value(m_userWidget->currentUser(), m_keyboardMonitor->isNumlockOn()).toBool();
    settings.endGroup();

    qDebug() << "restore numlock status to " << value;
    m_keyboardMonitor->setNumlockStatus(value);
}

