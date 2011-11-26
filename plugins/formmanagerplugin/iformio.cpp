/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2011 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
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
 *   Main Developper : Eric MAEKER, <eric.maeker@gmail.com>                *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/

/**
  \class Form::IFormIO
  This object interface is the in/out form object. It will be used to read patient files in order to
  construct patient files structure.
*/

/** \fn virtual QString Form::IFormIO::name() const
  Return the name of Form IO engine.
*/

/** \fn virtual void Form::IFormIO::muteUserWarnings(bool state);
  For debugging purpose, mute/unmute console warnings.
*/

/** \fn virtual bool Form::IFormIO::setFileName(const QString &absFileName);
  Set the filename to process, if IO device is a file. Return \e true if the IO engine
  can read this file according to its name and existence on the disk.
*/

/** \fn virtual QStringList Form::IFormIO::fileFilters() const;
  Return the file filters that the IO engine can read. Eg: <em>*.xml</em>
*/

/** \fn virtual QString Form::IFormIO::managedFileExtension() const;
  Return the file extension managed by the IO engine.
*/

/** \fn virtual bool Form::IFormIO::canReadFile() const;
  If the file is accessible to the IO engine, start the read process. Return \e true if the
  IO engine can read the file. \sa Form::IFormIO::setFileName()
*/

/** \fn virtual void toTreeWidget(QTreeWidget *tree, const QString &lang = Trans::Constants::ALL_LANGUAGE) const;
  If the file is accessible to the IO device and is readable, read its description and populate a
  QTreeWidget with it. Form::IFormIO::canReadFile() must be called first.
*/

/** \fn virtual Form::FormMain *Form::IFormIO::loadForm();
  Load the form, recreate the form tree and return the empty root Form::FormMain.
  Form::IFormIO::canReadFile() must be called first.
*/

/** \fn virtual bool Form::IFormIO::saveForm(QObject *treeRoot);
  Save a form (not its data).
*/

/** \fn virtual QString Form::IFormIO::lastError() const;
  Return the last error encountered wy the IO engine.
*/

#include "iformio.h"

#include <utils/versionnumber.h>
#include <utils/genericupdateinformation.h>
#include <translationutils/constanttranslations.h>

#include <QDate>
#include <QDebug>

using namespace Form;
using namespace Trans::ConstantTranslations;


FormIOQuery::FormIOQuery() :
    m_type(DefaultForms | CompleteForms),
    m_ForceFile(false),
    m_AllForms(false),
    m_AllDescr(false),
    m_GetShots(true)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////  FormIODescription /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
FormIODescription::FormIODescription() :
    Utils::GenericDescription(),
    m_reader(0)
{}

FormIODescription::~FormIODescription()
{
}

QVariant FormIODescription::data(const int ref, const QString &lang) const
{
    if (ref == TypeName) {
        if (data(IsCompleteForm).toBool()) {
            return QCoreApplication::translate("FormIODescription", "Complete form");
        } else if (data(IsSubForm).toBool()) {
            return QCoreApplication::translate("FormIODescription", "Sub-form");
        } else if (data(IsPage).toBool()) {
            return QCoreApplication::translate("FormIODescription", "Page only");
        }
        return QVariant();
    }
    return Utils::GenericDescription::data(ref, lang);
}

bool FormIODescription::setData(const int ref, const QVariant &value, const QString &lang)
{
    return Utils::GenericDescription::setData(ref, value, lang);
}

void FormIODescription::toTreeWidget(QTreeWidget *tree) const
{
    /** \todo code here */
    tree->clear();
    tree->setColumnCount(2);
    QFont bold;
    bold.setBold(true);

    QTreeWidgetItem *general = new QTreeWidgetItem(tree, QStringList() << tkTr(Trans::Constants::INFORMATIONS));
    general->setFont(0, bold);
    new QTreeWidgetItem(general, QStringList() << "Uuid" << data(FormIODescription::UuidOrAbsPath).toString());
    new QTreeWidgetItem(general, QStringList() << tkTr(Trans::Constants::AUTHOR) << data(FormIODescription::Author).toString());
    new QTreeWidgetItem(general, QStringList() << QCoreApplication::translate("Forms", "License") << data(FormIODescription::GlobalLicense).toString());
    new QTreeWidgetItem(general, QStringList() << tkTr(Trans::Constants::DESCRIPTION) << data(FormIODescription::ShortDescription).toString());

    QTreeWidgetItem *version = new QTreeWidgetItem(tree, QStringList() << tkTr(Trans::Constants::VERSION));
    version->setFont(0, bold);
    new QTreeWidgetItem(version, QStringList() << tkTr(Trans::Constants::VERSION) << data(FormIODescription::Version).toString());
    new QTreeWidgetItem(version, QStringList() << QCoreApplication::translate("Forms", "Creation date") << data(FormIODescription::CreationDate).toDate().toString(QLocale().dateFormat(QLocale::ShortFormat)));
    if (!data(FormIODescription::LastModificationDate).isNull())
        new QTreeWidgetItem(version, QStringList() << QCoreApplication::translate("Forms", "Last modification date") << data(FormIODescription::LastModificationDate).toDate().toString(QLocale().dateFormat(QLocale::ShortFormat)));

    tree->expandAll();
    tree->resizeColumnToContents(0);
    tree->resizeColumnToContents(1);
}


QDebug operator<<(QDebug dbg, const Form::FormIODescription &c)
{
    QString tmp = "FormIODescription(";
    QStringList attribs;
    if (c.data(Form::FormIODescription::IsCompleteForm).toBool())
        attribs << "isComplete";
    else if (c.data(Form::FormIODescription::IsSubForm).toBool())
        attribs << "isSub";
    else if (c.data(Form::FormIODescription::IsPage).toBool())
        attribs << "isPage";
    if (c.reader())
        attribs << "reader: " + c.reader()->name();
    attribs << "uid: " + c.data(Form::FormIODescription::UuidOrAbsPath).toString();
    attribs << "author: " + c.data(Form::FormIODescription::Author).toString();
    attribs << "country: " + c.data(Form::FormIODescription::Country).toString();
//    attribs << "languages: " + c.data(Form::FormIODescription::AvailableLanguages).toString();
    attribs << "v: " + c.data(Form::FormIODescription::Version).toString();
    attribs << "compat: " + c.data(Form::FormIODescription::FreeMedFormsCompatVersion).toString();
    attribs << "cdate: " + c.data(Form::FormIODescription::CreationDate).toString();
    attribs << "ludate: " + c.data(Form::FormIODescription::LastModificationDate).toString();
    attribs << "cat: " + c.data(Form::FormIODescription::Category).toString();
    attribs << "spe: " + c.data(Form::FormIODescription::Specialties).toString();
    attribs << "license: " + c.data(Form::FormIODescription::GlobalLicense).toString();
    attribs << "icon: " + c.data(Form::FormIODescription::GeneralIcon).toString();
    attribs << "web: " + c.data(Form::FormIODescription::WebLink).toString();
    attribs << "shotpath: " + c.data(Form::FormIODescription::ScreenShotsPath).toString();
    attribs << c.data(Form::FormIODescription::ShortDescription).toString();

    dbg.nospace() << tmp << attribs.join("; ");
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const Form::FormIODescription *c)
{
    if (!c) {
        dbg.nospace() << "FormIODescription(0x0)";
        return dbg.space();
    }
    return operator<<(dbg, *c);
}

