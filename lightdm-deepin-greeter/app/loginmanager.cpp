/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>
#include <QDesktopWidget>
#include <QDebug>

#include "loginmanager.h"
#include "loginframe.h"
#include "dbus/dbuslockservice.h"
#include "dbus/dbusaccounts.h"
#include "dbus/dbususer.h"

#include <X11/Xlib-xcb.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>

#define LOCKSERVICE_PATH "/com/deepin/dde/lock"
#define LOCKSERVICE_NAME "com.deepin.dde.lock"

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
    : QFrame(parent)
{
    recordPid();

    initUI();
    initData();
    initConnect();
    initDateAndUpdate();
}

LoginManager::~LoginManager()
{
}

void LoginManager::updateWidgetsPosition()
{
    const int height = this->height();
    const int width = this->width();

    m_userWidget->setFixedWidth(width);
    m_userWidget->move(0, (height - m_userWidget->height()) / 2 - 95); // center and margin-top: -95px
    qDebug() << "Change Screens" << m_userWidget->isChooseUserMode;
    m_sessionWidget->setFixedWidth(width);
    m_sessionWidget->move(0, (height - m_sessionWidget->height()) / 2 - 70); // 中间稍往上的位置
    m_logoWidget->move(48, height - m_logoWidget->height() - 36); // left 48px and bottom 36px
    m_switchFrame->move(width - m_switchFrame->width() - 20, height - m_switchFrame->height());
    m_requireShutdownWidget->setFixedWidth(width);
    m_requireShutdownWidget->setFixedHeight(300);
    m_requireShutdownWidget->move(0,  (height - m_requireShutdownWidget->height())/2 - 60);
}

void LoginManager::updateBackground(QString username)
{
    const QSettings settings("/var/lib/AccountsService/users/" + username, QSettings::IniFormat);
    const QString background = settings.value("User/GreeterBackground").toString();

    LoginFrame * frame = qobject_cast<LoginFrame*>(parent());
    frame->setBackground(background);
}

void LoginManager::updateUserLoginCondition(QString username)
{
    QProcess p;
    p.start("groups", QStringList(username));
    p.waitForFinished();

    QString output = p.readAllStandardOutput().trimmed();
    QStringList tokens = output.split(":");
    if (tokens.length() > 1) {
        QStringList groups = tokens.at(1).split(" ");
        qDebug() << groups;
        if (groups.contains("nopasswdlogin")) {
            m_passWdEdit->setFixedSize(0, 0);
            m_loginButton->show();

            return;
        }
    }

    m_passWdEdit->setFixedSize(m_passwdEditSize);
    m_loginButton->hide();
}

void LoginManager::keyPressEvent(QKeyEvent* e) {
    qDebug() << "qDebug loginManager:" << e->text();

    if (e->key() == Qt::Key_Escape) {
        if (!m_requireShutdownWidget->isHidden()) {
            m_requireShutdownWidget->hide();
            m_userWidget->show();
            if (!m_userWidget->isChooseUserMode) {
                m_passWdEdit->show();
            }
        }
#ifdef QT_DEBUG
        qApp->exit();
#endif
    }
}

void LoginManager::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (!m_requireShutdownWidget->isHidden()) {
            m_requireShutdownWidget->hide();
            m_userWidget->show();
            if (!m_userWidget->isChooseUserMode) {
                m_passWdEdit->show();
            }
        }

        if (m_keybdArrowWidget->isHidden()) {
            m_keybdArrowWidget->hide();
        }
    }
}

void LoginManager::leaveEvent(QEvent *)
{
    QList<QScreen *> screenList = qApp->screens();
    QPoint mousePoint = QCursor::pos();
    for (const QScreen *screen : screenList)
    {
        if (screen->geometry().contains(mousePoint))
        {
            const QRect &geometry = screen->geometry();
            setFixedSize(geometry.size());
            emit screenChanged(geometry);
            return;
        }
    }
}

void LoginManager::initUI()
{
    setFixedSize(qApp->desktop()->size());
    setObjectName("LoginManagerTool");

    m_sessionWidget = new SessionWidget(this);
    m_sessionWidget->hide();
    m_logoWidget = new LogoWidget(this);
    m_switchFrame = new SwitchFrame(this);
    m_userWidget = new UserWidget(this);

    m_userWidget->setObjectName("UserWidget");
    m_passWdEdit = new PassWdEdit(this);
    m_passWdEdit->setEnterBtnStyle(":/img/login_normal.png", ":/img/login_normal.png", ":/img/login_press.png");
    m_passWdEdit->setFocusPolicy(Qt::StrongFocus);
    m_passWdEdit->setFocus();
    m_passwdEditSize = m_passWdEdit->size();

    m_loginButton = new QPushButton(this);
    m_loginButton->setText(tr("Login"));
    m_loginButton->setStyleSheet("QPushButton {"
                                  "border: none;"
                                  "background: url(:/img/login_unpwd_normal.png);"
                                  "}"
                                  "QPushButton:pressed {"
                                  "border: none;"
                                  "background: url(:/img/login_unpwd_press.png);"
                                  "}");
    m_loginButton->setFixedSize(160, 36);
    m_loginButton->hide();

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
    m_Layout->addStretch();
    setLayout(m_Layout);
    showFullScreen();

    m_passWdEdit->updateKeyboardStatus();
    keyboardLayoutUI();
    leaveEvent(nullptr);

    m_switchFrame->setUserSwitchEnable(m_userWidget->count() > 1);
#ifndef SHENWEI_PLATFORM
    updateStyle(":/skin/login.qss", this);
#endif
    set_rootwindow_cursor();

    updateBackground(m_userWidget->currentUser());
    updateUserLoginCondition(m_userWidget->currentUser());
}

void LoginManager::recordPid() {
    qDebug() << "remember P i D" << qApp->applicationPid();

    QFile tmpPidFile(QString("%1%2").arg("/tmp/").arg(".dgreeterpid"));

    if (tmpPidFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QTextStream pidInfo(&tmpPidFile);
        pidInfo << qApp->applicationPid();

        tmpPidFile.close();
    } else {
        qDebug() << "file open failed!";
    }
}

void LoginManager::initData() {
    m_greeter = new QLightDM::Greeter(this);
    if (!m_greeter->connectSync())
        qWarning() << "greeter connect fail !!!";
}

void LoginManager::initConnect()
{
    connect(m_switchFrame, &SwitchFrame::triggerSwitchUser, this, &LoginManager::chooseUserMode);
    connect(m_switchFrame, &SwitchFrame::triggerSwitchUser, m_userWidget, &UserWidget::expandWidget, Qt::QueuedConnection);

    connect(m_switchFrame, &SwitchFrame::triggerPower, this, &LoginManager::showShutdownFrame);
    connect(m_switchFrame, &SwitchFrame::triggerSwitchSession, this, &LoginManager::chooseSessionMode);
    connect(m_passWdEdit, &PassWdEdit::submit, this, &LoginManager::login);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, this, &LoginManager::choosedSession);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, m_switchFrame, &SwitchFrame::chooseToSession, Qt::QueuedConnection);

    connect(m_userWidget, &UserWidget::userChanged,
            [&](const QString username){
        qDebug()<<"IIIIIIIIIIII Change to "<<username;
        qDebug()<<"IIIIIIIIIIII Session User "<< m_sessionWidget->lastSelectedUser();
        if (username != m_sessionWidget->lastSelectedUser()) {
            // goto greeter
            QProcess *process = new QProcess;
            process->start("dde-switchtogreeter " + username);
            process->waitForFinished();
            if (process->exitCode() == 0) {
                exit(0);
            }
            process->deleteLater();
        }

        m_sessionWidget->switchToUser(username);
        m_passWdEdit->show();
    #ifndef SHENWEI_PLATFORM
//        m_passWdEdit->updateKeybordLayoutStatus(username);
    #endif
        m_passWdEdit->setFocus();
        updateBackground(username);
        updateUserLoginCondition(username);
    });

    connect(m_greeter, &QLightDM::Greeter::showPrompt, this, &LoginManager::prompt);
    connect(m_greeter, &QLightDM::Greeter::authenticationComplete, this, &LoginManager::authenticationComplete);

    connect(m_passWdEdit, &PassWdEdit::updateKeyboardStatus, this, &LoginManager::keyboardLayoutUI);
    connect(m_passWdEdit, &PassWdEdit::keybdLayoutButtonClicked, this, &LoginManager::keybdLayoutWidgetPosit);
    connect(m_passWdEdit, &PassWdEdit::leftKeyPressed, this, &LoginManager::leftKeyPressed);
    connect(m_passWdEdit, &PassWdEdit::rightKeyPressed, this, &LoginManager::rightKeyPressed);

    connect(m_passWdEdit, &PassWdEdit::focusIn, [this]{
        if (!m_keybdArrowWidget->isHidden()) {
           m_keybdArrowWidget->hide();
        }});

    connect(m_requireShutdownWidget, &ShutdownWidget::shutDownWidgetAction, this, &LoginManager::setShutdownAction);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginManager::login);
}

void LoginManager::initDateAndUpdate() {
    /*Update the m_passWdEdit UI after get the current user
    *If the current user owns multi-keyboardLayouts,
    *then the button to choose keyboardLayout need show
    */
#ifndef SHENWEI_PLATFORM
//    m_passWdEdit->updateKeybordLayoutStatus(m_userWidget->currentUser());
#endif
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
    m_utilFile = new UtilFile(this);
    int expandState = m_utilFile->getExpandState();
    qDebug() << "expandState:" << expandState;
    if (expandState == 1) {
        qDebug() << "expandState:" << expandState;
        QMetaObject::invokeMethod(m_switchFrame, "triggerSwitchUser", Qt::QueuedConnection);
    }

    m_utilFile->setExpandState(0);
}

void LoginManager::prompt(QString text, QLightDM::Greeter::PromptType type)
{
    Q_UNUSED(text);
    switch (type)
    {
    case QLightDM::Greeter::PromptTypeSecret:   m_greeter->respond(m_passWdEdit->getText());     break;
    default:;
    }
}

void LoginManager::authenticationComplete()
{
    qDebug() << "authenticationComplete";
    m_userWidget->hideLoadingAni();

    if (!m_greeter->isAuthenticated()) {
        m_authFailureCount++;
        if (m_authFailureCount < UtilFile::GetAuthLimitation()) {
            m_passWdEdit->setAlert(true, tr("Wrong Password"));
        } else {
            m_authFailureCount = 0;
            m_passWdEdit->setReadOnly(true);
            m_passWdEdit->setEnabled(false);
            m_passWdEdit->setAlert(true, tr("Please retry after 10 minutes"));
        }

        return;
    }

    DBusLockService m_lockInter(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this);
    m_lockInter.ExitLock(m_userWidget->currentUser(), m_passWdEdit->getText());

    QTimer::singleShot(100, [&] {
        qDebug() << "session = " << m_sessionWidget->currentSessionName();
        qDebug() << "start session: " << m_greeter->startSessionSync(m_sessionWidget->currentSessionKey());
    });
}

void LoginManager::login()
{
    if(!m_requireShutdownWidget->isHidden()) {
        qDebug() << "SHUTDOWN";
        m_requireShutdownWidget->shutdownAction();
        return;
    }

    if (!m_sessionWidget->isHidden()) {
        qDebug() << "SESSIONWIDGET";
        m_sessionWidget->chooseSession();
        return;
    }
    if (m_userWidget->isChooseUserMode && !m_userWidget->isHidden()) {
        m_userWidget->chooseButtonChecked();
        m_passWdEdit->getFocusTimer->start();
        qDebug() << "lineEditGrabKeyboard";
        return;
    }

    const QString &username = m_userWidget->currentUser();

    if (!m_passWdEdit->isVisible())
        return;

    m_userWidget->showLoadingAni();

    if (m_greeter->inAuthentication())
        m_greeter->cancelAuthentication();

    //m_passWdEdit->setAlert(true, "asd");

    //save user last choice
    m_sessionWidget->saveUserLastSession(m_userWidget->currentUser());
    m_userWidget->saveLastUser();

    //const QString &username = m_userWidget->currentUser();
    m_greeter->authenticate(username);
    qDebug() << "choose user: " << username;
    qDebug() << "auth user: " << m_greeter->authenticationUser();
}

void LoginManager::chooseUserMode()
{
    m_passWdEdit->hide();
    m_loginButton->hide();
    m_sessionWidget->hide();
    m_userWidget->show();
    m_requireShutdownWidget->hide();
}

void LoginManager::chooseSessionMode()
{
    m_sessionWidget->show();
    m_userWidget->hide();
    m_passWdEdit->hide();
    m_loginButton->hide();
    m_requireShutdownWidget->hide();
    if (!m_keybdArrowWidget->isHidden()) {
        m_keybdArrowWidget->hide();
    }
}

void LoginManager::choosedSession() {
    m_requireShutdownWidget->hide();
    m_sessionWidget->hide();
    m_userWidget->show();
    if (m_userWidget->isChooseUserMode) {
        m_passWdEdit->hide();
    } else {
        m_passWdEdit->show();
    }
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
}

void LoginManager::keyboardLayoutUI() {
    //keyboardList is the keyboardLayout list of current user
    QStringList keyboardList;
//    keyboardList = m_passWdEdit->keyboardLayoutList;
    /*xkbParse is used to parse the xml file,
    * get the details info of keyboardlayout
    */
    xkbParse = new XkbParser(this);
//    QStringList keyboardListContent =  xkbParse->lookUpKeyboardList(keyboardList);
    QString currentKeybdLayout;
    QStringList keyboardList1;

    DBusAccounts *accounts = new DBusAccounts("com.deepin.daemon.Accounts", "/com/deepin/daemon/Accounts", QDBusConnection::systemBus(), this);
    for (auto u : accounts->userList())
    {
        DBusUser *user = new DBusUser("com.deepin.daemon.Accounts", u, QDBusConnection::systemBus(), this);

        if (user->userName() == m_userWidget->currentUser())
        {
            keyboardList = user->historyLayout();
            currentKeybdLayout = user->layout();
            break;
        }
        user->deleteLater();
    }

//    keyboardList = m_keyboardLayoutInterface->userLayoutList();
//    QString currentKeybdLayout = m_keyboardLayoutInterface->currentLayout();

    int index = 0;
    for (int i = 0; i < keyboardList.length(); i++) {
        if (keyboardList[i] == currentKeybdLayout) {
            index = i;
            break;
        }
//        m_keybdLayoutWidget->setListItemChecked(i);
//        QDBusPendingReply<QString> tmpValue =  m_keyboardLayoutInterface->GetLayoutDesc(keyboardList[i]);
//        tmpValue.waitForFinished();

////        m_keybdInfoMap.insert(keyboardList[i], tmpValue);
    }

    qDebug() << keyboardList;
    keyboardList1 << xkbParse->lookUpKeyboardList(keyboardList);
    qDebug() << "QStringList" << keyboardList1;

    m_keybdLayoutWidget = new KbLayoutWidget(keyboardList1);
    m_keybdLayoutWidget->setListItemChecked(index);
//    m_passwordEdit->updateKeybdLayoutUI(keybdLayoutDescList);


    m_keybdArrowWidget = new DArrowRectangle(DArrowRectangle::ArrowTop, this);
    m_keybdArrowWidget->setBackgroundColor(QColor(0, 0, 0, 78));
    m_keybdArrowWidget->setBorderColor(QColor(0, 0, 0, 100));
    m_keybdArrowWidget->setArrowX(13);
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

void LoginManager::setCurrentKeybdLayoutList(QString keyboard_value) {
    qDebug() << "setCurrentKeybdLayoutList";

    QString keyboard_key = xkbParse->lookUpKeyboardKey(keyboard_value);
    qDebug() << "parse:" << keyboard_value << keyboard_value;
    m_passWdEdit->utilSettings->setCurrentKbdLayout(m_userWidget->currentUser(),keyboard_key);

}

void LoginManager::keybdLayoutWidgetPosit() {
    if (m_keybdArrowWidget->isHidden()) {
        m_keybdArrowWidget->show(m_passWdEdit->x() + 123, m_passWdEdit->y() + m_passWdEdit->height() - 15);
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
            if (m_userWidget->isChooseUserMode) {
                m_passWdEdit->hide();
            } else {
                m_passWdEdit->show();
            }
            m_sessionWidget->hide();
        break;}
        default:;
    }
}

void LoginManager::leftKeyPressed() {
    if (!m_userWidget->isHidden() && m_passWdEdit->isHidden()) {
        m_userWidget->leftKeySwitchUser();
    }
//    if (!m_userWidget->isHidden() && !m_passWdEdit->isHidden() &&
//            m_passWdEdit->getText().isEmpty()) {
//        m_userWidget->leftKeySwitchUser();
//    }
    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->leftKeySwitch();
    }
    if (!m_sessionWidget->isHidden()) {
        m_sessionWidget->leftKeySwitch();
    }
}

void LoginManager::rightKeyPressed() {
    if (!m_userWidget->isHidden() && m_passWdEdit->isHidden()) {
        m_userWidget->rightKeySwitchUser();
    }
//   if (!m_userWidget->isHidden() && !m_passWdEdit->isHidden() &&
//            m_passWdEdit->getText().isEmpty()) {
//        m_userWidget->rightKeySwitchUser();
//    }
    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->rightKeySwitch();
    }
    if (!m_sessionWidget->isHidden()) {
        m_sessionWidget->rightKeySwitch();
    }
}
