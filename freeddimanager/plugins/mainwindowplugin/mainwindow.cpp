/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2013 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program (COPYING.FREEMEDFORMS file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *  Main Developer: Eric MAEKER, <eric.maeker@gmail.com>                   *
 *  Contributors:                                                          *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#include "mainwindow.h"

#include <coreplugin/icore.h>
#include <coreplugin/isettings.h>
#include <coreplugin/icommandline.h>
#include <coreplugin/constants.h>
#include <coreplugin/translators.h>
#include <coreplugin/itheme.h>
#include <coreplugin/filemanager.h>
#include <coreplugin/constants_icons.h>
#include <coreplugin/constants_menus.h>
#include <coreplugin/modemanager/modemanager.h>
#include <coreplugin/actionmanager/mainwindowactions.h>
#include <coreplugin/actionmanager/mainwindowactionhandler.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/contextmanager/contextmanager.h>
#include <coreplugin/dialogs/plugindialog.h>
#include <coreplugin/dialogs/settingsdialog.h>
#include <coreplugin/dialogs/helpdialog.h>
#include <coreplugin/idocumentprinter.h>

#include <utils/log.h>
#include <utils/global.h>
#include <utils/randomizer.h>
#include <utils/stylehelper.h>
#include <utils/updatechecker.h>
#include <utils/widgets/fancyactionbar.h>
#include <utils/widgets/fancytabwidget.h>
#include <extensionsystem/pluginerrorview.h>
#include <extensionsystem/pluginview.h>
#include <extensionsystem/pluginmanager.h>
#include <translationutils/constants.h>
#include <translationutils/trans_patient.h>
#include <translationutils/trans_filepathxml.h>
#include <translationutils/trans_msgerror.h>
#include <translationutils/trans_current.h>

#include "ui_mainwindow.h"

#include <QString>
#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>
#include <QStringList>
#include <QCoreApplication>
#include <QDir>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDockWidget>
#include <QMenu>
#include <QDesktopWidget>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QToolBar>

using namespace MainWin;
using namespace Internal;
using namespace Trans::ConstantTranslations;

enum { WarnLogMessage = false };

// Getting the Core instances
static inline Utils::UpdateChecker *updateChecker() { return Core::ICore::instance()->updateChecker(); }
static inline Core::ICommandLine *commandLine() { return Core::ICore::instance()->commandLine(); }
static inline Core::ISettings *settings()  { return Core::ICore::instance()->settings(); }
static inline Core::ITheme *theme()  { return Core::ICore::instance()->theme(); }
static inline Core::ActionManager *actionManager() { return Core::ICore::instance()->actionManager(); }
static inline Core::ContextManager *contextManager() { return Core::ICore::instance()->contextManager(); }
static inline Core::IDocumentPrinter *printer() {return ExtensionSystem::PluginManager::instance()->getObject<Core::IDocumentPrinter>();}
static inline Core::FileManager *fileManager() { return Core::ICore::instance()->fileManager(); }
static inline Core::ModeManager *modeManager() { return Core::ICore::instance()->modeManager(); }

// SplashScreen Messagers
static inline void messageSplash(const QString &s) {theme()->messageSplashScreen(s); }
static inline void finishSplash(QMainWindow *w) {theme()->finishSplashScreen(w); }

//--------------------------------------------------------------------------------------------------------
//--------------------------------------- Constructor / Destructor ---------------------------------------
//--------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    Core::IMainWindow(parent),
    ui(0)
{
    setObjectName("MainWindow");
    messageSplash(tr("Creating Main Window"));
}

/**
 * Initialize the main window:
 * - creates global menus and actions
 * - connect actions to default slots
 * Returns \e true if all goes fine.
 * \sa Core::IMainWindow, Core::MainWindowActions, Core::MainWindowActionHandler
 */
bool MainWindow::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments);
    Q_UNUSED(errorString);
    // create menus
    createFileMenu();
    createFileNewSubMenu();
    Core::ActionContainer *fmenu = actionManager()->createMenu(Core::Constants::M_FILE_RECENTFILES);
    fmenu->setOnAllDisabledBehavior(Core::ActionContainer::Show);
    connect(fmenu->menu(), SIGNAL(aboutToShow()),this, SLOT(aboutToShowRecentFiles()));
    createEditMenu();
//    createFormatMenu();
    createConfigurationMenu();
    createHelpMenu();

    Core::MainWindowActions actions;
    actions.setFileActions(
                Core::MainWindowActions::A_FileOpen |
                Core::MainWindowActions::A_FileSave |
                Core::MainWindowActions::A_FileSaveAs |
                Core::MainWindowActions::A_FilePrint |
                Core::MainWindowActions::A_FilePrintPreview |
                Core::MainWindowActions::A_FileQuit);
    actions.setConfigurationActions(
                Core::MainWindowActions::A_AppPreferences |
                Core::MainWindowActions::A_LanguageChange //|
                //            Core::MainWindowActions::A_ConfigureMedinTux
                );
    actions.setHelpActions(
                Core::MainWindowActions::A_AppAbout |
                Core::MainWindowActions::A_PluginsAbout |
                Core::MainWindowActions::A_AppHelp |
                Core::MainWindowActions::A_DebugDialog |
                Core::MainWindowActions::A_CheckUpdate //|
                //            Core::MainWindowActions::A_QtAbout
                );
    actions.createEditActions(false);
    createActions(actions);

    connectFileActions();
    connectConfigurationActions();
    connectHelpActions();

    // Create Mode stack
    m_modeStack = new Utils::FancyTabWidget(this);
    modeManager()->init(m_modeStack);

    return true;
}

/**
 * When all dependent plugins are initialized, start the second initialization:
 * - setup ui
 * - start update checker
 * - read settings
 */
void MainWindow::extensionsInitialized()
{
//    // Creating MainWindow UI
//    ui = new Internal::Ui::MainWindow();
//    ui->setupUi(this);
//    if (layout())
//        layout()->setMargin(0);

//    // Creating ToolBar
//    QToolBar *bar = new QToolBar(this);
//    bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//    bar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//    bar->setIconSize(QSize(32, 32));
//    QStringList uids;
////    uids  << Core::Constants::A_FILE_OPEN
////          << Core::Constants::A_FILE_SAVE
////          << Core::Constants::A_FILE_SAVEAS
////          << "--"
////          << Core::Constants::A_FILE_PRINT
////          << "->"
////             ;
//    foreach(const QString &uid, uids) {
//        if (uid=="--") {
//            bar->addSeparator();
//            continue;
//        } else if (uid=="->") {
//            QWidget *w = new QWidget(bar);
//            w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//            w->setLayout(new QHBoxLayout(w));
//            w->layout()->addItem(new QSpacerItem(10,10, QSizePolicy::Expanding, QSizePolicy::Expanding));
//            bar->addWidget(w);
//            continue;
//        } else {
//            Core::Command *cmd = actionManager()->command(Core::Id(uid));
//            if (cmd)
//                bar->addAction(cmd->action());
//        }
//    }
//    bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    setUnifiedTitleAndToolBarOnMac(true);
//    addToolBar(bar);

    // Start the update checker
    if (updateChecker()->needsUpdateChecking(settings()->getQSettings())) {
        settings()->setPath(Core::ISettings::UpdateUrl, Utils::Constants::FREEDDIMANAGER_UPDATE_URL);
        if (checkUpdate())
            settings()->setValue(Utils::Constants::S_LAST_CHECKUPDATE, QDate::currentDate());
    }

    //m_modeStack->insertTopWidget(patientCore()->patientBar());
    m_modeStack->statusBar()->hide();
    setCentralWidget(m_modeStack);

    createDockWindows();

    setWindowTitle(QString("%1 %2 - (c) %3").arg(qApp->applicationName()).arg(qApp->applicationVersion()).arg(tkTr(Trans::Constants::THE_FREEMEDFORMS_COMMUNITY)));
    setWindowIcon(theme()->icon(Core::Constants::ICONFREEDRC));

    connect(Core::ICore::instance(), SIGNAL(coreOpened()), this, SLOT(postCoreOpened()));
}

MainWindow::~MainWindow()
{
//    delete centralWidget();
//    setCentralWidget(0);
//    delete ui;
    if (Utils::Log::warnPluginsCreation())
        qWarning() << "MainWindow::~MainWindow()";
}

/**
 * When all plugins are correctly initialized and the Core plugin is opened,
 * start a third initialization.
 * - set the central widget of the main window
 * - translate UI, actions and menus
 * - resize window to default
 * - show and raise main window
 * \sa Core::ICore::coreOpened()
 */
// TEST
// END TEST
void MainWindow::postCoreOpened()
{
    if (Utils::Log::warnPluginsCreation())
        qWarning() << "MainWindow::postCoreOpened()";

    finishSplash(this);
    actionManager()->retranslateMenusAndActions();
    contextManager()->updateContext();
    raise();
    show();

//    manageIModeEnabledState();
    connect(modeManager(), SIGNAL(currentModeChanged(Core::IMode*)), this, SLOT(onCurrentModeChanged(Core::IMode*)), Qt::UniqueConnection);
//    modeManager()->activateMode(Core::Constants::MODE_PATIENT_SEARCH);

    readSettings(); // moved here because due to the toolbar presence, save/restoreGeometry is buggy
}

/**
  \brief Refresh the ui data refering to the patient
  \sa Core::Internal::CoreImpl::instance()->patient()
*/
void MainWindow::refreshPatient()
{
}

/**
  \brief Close the main window and the application
  \todo Add  ICoreListener
*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    LOG("Closing MainWindow");
    Core::ICore::instance()->requestSaveSettings();
    Core::ICore::instance()->coreIsAboutToClose();
    writeSettings();
    event->accept();
}

/** \brief Manages language changes : retranslate Ui and ActionManager. */
void MainWindow::changeEvent(QEvent *event)
{
    if (event->type()==QEvent::LanguageChange) {
//        ui->retranslateUi(this);
        actionManager()->retranslateMenusAndActions();
        refreshPatient();
    }
}

/** \brief Populate recent files menu */
void MainWindow::aboutToShowRecentFiles()
{
    Core::ActionContainer *aci = actionManager()->actionContainer(Core::Constants::M_FILE_RECENTFILES);
    aci->clear();

    foreach (const QString &fileName, fileManager()->recentFiles()) {
        QAction *action = aci->menu()->addAction(fileName);
        action->setData(fileName);
        connect(action, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    aci->setOnAllDisabledBehavior(Core::ActionContainer::Show);
}

/** \brief Opens a recent file. This solt must be called by a recent files' menu's action. */
void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;
    QString fileName = action->data().toString();
    if (!fileName.isEmpty()) {
        readFile(fileName);
    }
}

void MainWindow::updateCheckerEnd(bool)
{
    delete statusBar();
}

/** Reads main window's settings */
void MainWindow::readSettings()
{
    settings()->restoreState(this);
    fileManager()->getRecentFilesFromSettings();
    fileManager()->getMaximumRecentFilesFromSettings();
    fileManager()->setCurrentFile(QString::null);
    Utils::StyleHelper::setBaseColor(Utils::StyleHelper::DEFAULT_BASE_COLOR);
}

/** \brief Writes main window's settings */
void MainWindow::writeSettings()
{
    settings()->saveState(this);
    fileManager()->saveRecentFiles();
    settings()->sync();
}

/** \obsolete */
void MainWindow::createStatusBar()
{
//    statusBar()->showMessage( tkTr(Trans::Constants::READY), 2000 );
}

/** When the current Core::IMode is beeing updated, check the patient visibility */
void MainWindow::onCurrentModeChanged(Core::IMode *newMode)
{
    Q_ASSERT(newMode);
//    if (!newMode)
//        return;
//    if (newMode->patientBarVisibility())
//        patient()->showPatientBar();
//    else
//        patient()->hidePatientBar();
}

bool MainWindow::newFile()
{
    return true;
}

/** Open the preferences dialog */
bool MainWindow::applicationPreferences()
{
    Core::SettingsDialog dlg(this);
    dlg.exec();
    return true;
}

/** \brief Runs the MedinTux configurator */
bool MainWindow::configureMedintux()
{
//    Internal::configureMedinTux();
    return true;
}

bool MainWindow::saveAsFile()
{
    // get filename
    QString fileName = QFileDialog::getSaveFileName(this, tkTr(Trans::Constants::SAVE_FILE),
                                QDir::homePath(),
                                tkTr(Core::Constants::FREEDRC_FILEFILTER));
    if (fileName.isEmpty())
        return false;

    bool ok = saveFileContent(fileName);
    if (ok) {
        fileManager()->addToRecentFiles(fileName);
        fileManager()->setCurrentFile(fileName);
    }
    return ok;
}

bool MainWindow::saveFile()
{
    if (fileManager()->currentFile().isEmpty())
        return saveAsFile();
    return saveFileContent(fileManager()->currentFile());
}

bool MainWindow::openFile()
{
    QString f = QFileDialog::getOpenFileName(this,
                                             tkTr(Trans::Constants::OPEN_FILE),
                                             QDir::homePath(),
                                             tkTr(Core::Constants::FREEDRC_FILEFILTER) );
    if (f.isEmpty())
        return false;
//        QString f = QString("%1/%2/%3")
//                .arg(settings()->path(Core::ISettings::BundleResourcesPath))
//                .arg("textfiles/freedrctest")
//                .arg("cr_2013_09.cr");
    readFile(f);
    fileManager()->addToRecentFiles(f);
    fileManager()->setCurrentFile(f);
    return true;
}

/**
 * Write XML the content of the CR to the file \e fileName.
 * File name must be an absolute file path.
 */
bool MainWindow::saveFileContent(const QString &fileName)
{
    // Create extra-xml content
//    QDomDocument doc;
//    QDomElement pe = doc.createElement(::XML_PATIENT_NAME);
//    QDomText pet = doc.createTextNode(_headerWidget->patientName->text());
//    pe.appendChild(pet);
//    doc.appendChild(pe);

//    // Get full content XML
//    const QList<ConsultResult> list = _crTreeModel->consultResultList();
//    QString xml = ConsultResult::listToXml(list, doc.toString());

//    // Save to file
//    return Utils::saveStringToFile(xml, fileName, Utils::Overwrite, Utils::DontWarnUser);
    return true;
}

/**
 * Read the XML content of the CR \e fileName.
 * File name must be an absolute file path.
 */
void MainWindow::readFile(const QString &fileName)
{
//    QString extra;
//    QList<ConsultResult> list = ConsultResult::fromXml(Utils::readTextFile(fileName), &extra);
//    extra = extra.simplified();
//    if (!extra.isEmpty()) {
//        QDomDocument doc;
//        QString error;
//        int line = 0;
//        int col = 0;
//        if (!doc.setContent(extra, &error, &line, &col)) {
//            LOG_ERROR(tkTr(Trans::Constants::ERROR_1_LINE_2_COLUMN_3).arg(error).arg(line).arg(col));
//            LOG_ERROR(QString("Unable to read extra-xml content of file: %1").arg(fileName));
//        } else {
//            QDomElement extra = doc.firstChildElement(Constants::XML_EXTRA_TAG);
//            QDomElement patient = extra.firstChildElement(::XML_PATIENT_NAME);
//            _headerWidget->patientName->setText(patient.text());
//        }
//    }
//    _crTreeModel->setCrList(list);
}

void MainWindow::createDockWindows()
{
//    QDockWidget *dock = m_TemplatesDock = new QDockWidget(tkTr(Trans::Constants::TEMPLATES), this);
//    dock->setObjectName("templatesDock");
//    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//    dock->setWidget(new Templates::TemplatesView(dock));
//    addDockWidget(Qt::RightDockWidgetArea, dock);
//    QMenu *menu = actionManager()->actionContainer(Core::Constants::M_TEMPLATES)->menu();
//    menu->addAction(dock->toggleViewAction());
}

