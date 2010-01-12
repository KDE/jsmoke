/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "marshall.h"
#include "global.h"
#include "marshallmacros.h"

#include <kaboutdata.h>
#include <kabstractwidgetjobtracker.h>
#include <kauthaction.h>
#include <kauthactionreply.h>
#include <kauthactionwatcher.h>
#include <kautosavefile.h>
#include <kcompositejob.h>
#include <kcoreconfigskeleton.h>
#include <kdialogjobuidelegate.h>
#include <kjob.h>
#include <kjobtrackerinterface.h>
#include <kjobuidelegate.h>
#include <kmimetype.h>
#include <kplugininfo.h>
#include <kprotocolinfo.h>
#include <kserviceaction.h>
#include <kservicegroup.h>
#include <kservice.h>
#include <kservicetype.h>
#include <ksycocaentry.h>
#include <ktcpsocket.h>
#include <ktimezone.h>
#include <kuiserverjobtracker.h>
#include <kuser.h>

// Q_DECLARE_METATYPE(KAboutLicense)
// Q_DECLARE_METATYPE(KAboutPerson)
Q_DECLARE_METATYPE(KAuth::Action)
Q_DECLARE_METATYPE(KAutoSaveFile*)
// Q_DECLARE_METATYPE(KCatalogName)
Q_DECLARE_METATYPE(KConfigSkeletonItem*)
Q_DECLARE_METATYPE(KCoreConfigSkeleton::ItemEnum::Choice)
Q_DECLARE_METATYPE(KCoreConfigSkeleton::ItemEnum::Choice2)
Q_DECLARE_METATYPE(KJob*)
Q_DECLARE_METATYPE(KLocale::DigitSet)
Q_DECLARE_METATYPE(KPluginInfo)
Q_DECLARE_METATYPE(KProtocolInfo::ExtraField)
Q_DECLARE_METATYPE(KServiceAction)
Q_DECLARE_METATYPE(KSharedPtr<KMimeType>)
Q_DECLARE_METATYPE(KSharedPtr<KService>)
Q_DECLARE_METATYPE(KSharedPtr<KServiceGroup>)
Q_DECLARE_METATYPE(KSharedPtr<KServiceType>)
Q_DECLARE_METATYPE(KSharedPtr<KSycocaEntry>)
Q_DECLARE_METATYPE(KSslCipher)
Q_DECLARE_METATYPE(KSslError)
Q_DECLARE_METATYPE(KTimeZone::LeapSeconds)
Q_DECLARE_METATYPE(KTimeZone::Phase)
Q_DECLARE_METATYPE(KTimeZone::Transition)
Q_DECLARE_METATYPE(KUser)
Q_DECLARE_METATYPE(KUserGroup)
Q_DECLARE_METATYPE(QByteArray)
Q_DECLARE_METATYPE(QDateTime)
// Q_DECLARE_METATYPE(QList<KAboutLicense>)
// Q_DECLARE_METATYPE(QList<KAboutPerson>)
Q_DECLARE_METATYPE(QList<KAuth::Action>)
Q_DECLARE_METATYPE(QList<KAutoSaveFile*>)
// Q_DECLARE_METATYPE(QList<KCatalogName>)
Q_DECLARE_METATYPE(QList<KConfigSkeletonItem*>)
Q_DECLARE_METATYPE(QList<KCoreConfigSkeleton::ItemEnum::Choice>)
Q_DECLARE_METATYPE(QList<KCoreConfigSkeleton::ItemEnum::Choice2>)
Q_DECLARE_METATYPE(QList<KJob*>)
Q_DECLARE_METATYPE(QList<KLocale::DigitSet>)
Q_DECLARE_METATYPE(QList<KPluginInfo>)
Q_DECLARE_METATYPE(QList<KProtocolInfo::ExtraField>)
Q_DECLARE_METATYPE(QList<KServiceAction>)
Q_DECLARE_METATYPE(QList<KSharedPtr<KMimeType> >)
Q_DECLARE_METATYPE(QList<KSharedPtr<KService> >)
Q_DECLARE_METATYPE(QList<KSharedPtr<KServiceGroup> >)
Q_DECLARE_METATYPE(QList<KSharedPtr<KServiceType> >)
Q_DECLARE_METATYPE(QList<KSharedPtr<KSycocaEntry> >)
Q_DECLARE_METATYPE(QList<KSslCipher>)
Q_DECLARE_METATYPE(QList<KSslError>)
Q_DECLARE_METATYPE(QList<KTimeZone::LeapSeconds>)
Q_DECLARE_METATYPE(QList<KTimeZone::Phase>)
Q_DECLARE_METATYPE(QList<KTimeZone::Transition>)
Q_DECLARE_METATYPE(QList<KUser>)
Q_DECLARE_METATYPE(QList<KUserGroup>)
Q_DECLARE_METATYPE(QList<QByteArray>)
Q_DECLARE_METATYPE(QList<QDateTime>)
Q_DECLARE_METATYPE(QList<QSslCertificate>)
Q_DECLARE_METATYPE(QSslCertificate)


template <class Container>
QScriptValue qScriptSmokeValueFromKSycocaEntrySequence(QScriptEngine *eng, const Container &cont)
{
    QScriptValue a = eng->newArray();
    Smoke::ModuleIndex classId;
    typename Container::const_iterator begin = cont.begin();
    typename Container::const_iterator end = cont.end();
    typename Container::const_iterator it;
    quint32 i;
    for (it = begin, i = 0; it != end; ++it, ++i) {
        switch ((*it)->sycocaType()) {
        case KST_KSycocaEntry:
            classId = qtcore_Smoke->findClass("KSycocaEntry");
            break;
        case KST_KService:
            classId = qtcore_Smoke->findClass("KService");
            break;
        case KST_KServiceGroup:
            classId = qtcore_Smoke->findClass("KServiceGroup");
            break;
        case KST_KServiceType:
            classId = qtcore_Smoke->findClass("KServiceType");
            break;
        case KST_KMimeType:
            classId = qtcore_Smoke->findClass("KMimeType");
            break;
        default:
            break;
        }
        
        a.setProperty(i, qScriptSmokeValueFromSequence_helper(eng, classId, (void *) &(*(*it).data())));
    }
    
    return a;
}


template<typename T>
int qScriptSmokeRegisterKSycocaEntrySequenceMetaType(
    QScriptEngine *engine,
    const QScriptValue &prototype = QScriptValue()
#ifndef qdoc
    , T * /* dummy */ = 0
#endif
)
{
    return qScriptRegisterMetaType<T>(engine, qScriptSmokeValueFromKSycocaEntrySequence,
                                      qScriptValueToSequence, prototype);
}

namespace JSmoke {

Marshall::TypeHandler KDECoreHandlers[] = {
//     { "QList<KAboutLicense>", marshall_Container<QList<KAboutLicense> > },
//    { "QList<KAboutPerson>", marshall_Container<QList<KAboutPerson> > },
    { "QList<KAuth::Action>*", marshall_Container<QList<KAuth::Action> > },
    { "QList<KAuth::Action>&", marshall_Container<QList<KAuth::Action> > },
    { "QList<KAutoSaveFile*>", marshall_Container<QList<KAutoSaveFile*> > },
//    { "QList<KCatalogName>&", marshall_Container<QList<KCatalogName> > },
    { "QList<KConfigSkeletonItem*>", marshall_Container<QList<KConfigSkeletonItem*> > },
    { "QList<KCoreConfigSkeleton::ItemEnum::Choice2>", marshall_Container<QList<KCoreConfigSkeleton::ItemEnum::Choice2> > },
    { "QList<KCoreConfigSkeleton::ItemEnum::Choice2>&", marshall_Container<QList<KCoreConfigSkeleton::ItemEnum::Choice2> > },
    { "QList<KCoreConfigSkeleton::ItemEnum::Choice>", marshall_Container<QList<KCoreConfigSkeleton::ItemEnum::Choice> > },
    { "QList<KCoreConfigSkeleton::ItemEnum::Choice>&", marshall_Container<QList<KCoreConfigSkeleton::ItemEnum::Choice> > },
    { "QList<KJob*>&", marshall_Container<QList<KJob*> > },
    { "QList<KLocale::DigitSet>", marshall_Container<QList<KLocale::DigitSet> > },
    { "QList<KPluginInfo>", marshall_Container<QList<KPluginInfo> > },
    { "QList<KProtocolInfo::ExtraField>", marshall_Container<QList<KProtocolInfo::ExtraField> > },
    { "QList<KServiceAction>", marshall_Container<QList<KServiceAction> > },
    { "QList<KSharedPtr<KMimeType>>", marshall_Container<QList<KSharedPtr<KMimeType> > > },
    { "QList<KSharedPtr<KServiceGroup>>", marshall_Container<QList<KSharedPtr<KServiceGroup> > > },
    { "QList<KSharedPtr<KService>>", marshall_Container<QList<KSharedPtr<KService> > > },
    { "QList<KSharedPtr<KService>>&", marshall_Container<QList<KSharedPtr<KService> > > },
    { "QList<KSharedPtr<KServiceType>>", marshall_Container<QList<KSharedPtr<KServiceType> > > },
    { "QList<KSharedPtr<KSycocaEntry>>", marshall_Container<QList<KSharedPtr<KSycocaEntry> > > },
    { "QList<KSslCipher>", marshall_Container<QList<KSslCipher> > },
    { "QList<KSslCipher>&", marshall_Container<QList<KSslCipher> > },
    { "QList<KSslError>", marshall_Container<QList<KSslError> > },
    { "QList<KSslError>&", marshall_Container<QList<KSslError> > },
    { "QList<KTimeZone::LeapSeconds>", marshall_Container<QList<KTimeZone::LeapSeconds> > },
    { "QList<KTimeZone::LeapSeconds>&", marshall_Container<QList<KTimeZone::LeapSeconds> > },
    { "QList<KTimeZone::Phase>", marshall_Container<QList<KTimeZone::Phase> > },
    { "QList<KTimeZone::Phase>&", marshall_Container<QList<KTimeZone::Phase> > },
    { "QList<KTimeZone::Transition>", marshall_Container<QList<KTimeZone::Transition> > },
    { "QList<KTimeZone::Transition>&", marshall_Container<QList<KTimeZone::Transition> > },
    { "QList<KUserGroup>", marshall_Container<QList<KUserGroup> > },
    { "QList<KUser>", marshall_Container<QList<KUser> > },
    { "QList<QByteArray>", marshall_Container<QList<QByteArray> > },
    { "QList<QByteArray>&", marshall_Container<QList<QByteArray> > },
    { "QList<QDateTime>", marshall_Container<QList<QDateTime> > },
    { "QList<QSslCertificate>", marshall_Container<QList<QSslCertificate> > },
    { "QList<QSslCertificate>&", marshall_Container<QList<QSslCertificate> > },

    { 0, 0 }
};

void registerKDECoreTypes(QScriptEngine * engine)
{
//    qScriptSmokeRegisterSequenceMetaType<QList<KAboutLicense> >(engine);
//    qScriptSmokeRegisterSequenceMetaType<QList<KAboutPerson> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KAuth::Action> >(engine);
//    qScriptSmokeRegisterSequenceMetaType<QList<KCatalogName> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KCoreConfigSkeleton::ItemEnum::Choice2> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KCoreConfigSkeleton::ItemEnum::Choice> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KLocale::DigitSet> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KPluginInfo> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KProtocolInfo::ExtraField> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KServiceAction> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KSslCipher> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KSslError> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KTimeZone::LeapSeconds> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KTimeZone::Phase> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KTimeZone::Transition> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KUser> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<KUserGroup> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QByteArray> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QDateTime> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QSslCertificate> >(engine);

    qScriptSmokeRegisterPointerSequenceMetaType<QList<KAutoSaveFile*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KConfigSkeletonItem*> >(engine);
    qScriptSmokeRegisterPointerSequenceMetaType<QList<KJob*> >(engine);

    qScriptSmokeRegisterKSycocaEntrySequenceMetaType<QList<KSharedPtr<KMimeType> > >(engine);
    qScriptSmokeRegisterKSycocaEntrySequenceMetaType<QList<KSharedPtr<KService> > >(engine);
    qScriptSmokeRegisterKSycocaEntrySequenceMetaType<QList<KSharedPtr<KServiceGroup> > >(engine);
    qScriptSmokeRegisterKSycocaEntrySequenceMetaType<QList<KSharedPtr<KServiceType> > >(engine);
    qScriptSmokeRegisterKSycocaEntrySequenceMetaType<QList<KSharedPtr<KSycocaEntry> > >(engine);
    
    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
