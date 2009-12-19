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
#include <kplugininfo.h>
#include <kprotocolinfo.h>
#include <kserviceaction.h>
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
Q_DECLARE_METATYPE(QList<KCoreConfigSkeleton::ItemEnum::Choice2>)
Q_DECLARE_METATYPE(QList<KCoreConfigSkeleton::ItemEnum::Choice>)
Q_DECLARE_METATYPE(QList<KJob*>)
Q_DECLARE_METATYPE(QList<KLocale::DigitSet>)
Q_DECLARE_METATYPE(QList<KPluginInfo>)
Q_DECLARE_METATYPE(QList<KProtocolInfo::ExtraField>)
Q_DECLARE_METATYPE(QList<KServiceAction>)
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

namespace JSmoke {

// DEF_CONTAINER_MARSHALLER( QListKAboutLicense, QList<KAboutLicense> )
// DEF_CONTAINER_MARSHALLER( QListKAboutPerson, QList<KAboutPerson> )
DEF_CONTAINER_MARSHALLER( QListKAuthAction, QList<KAuth::Action> )
DEF_CONTAINER_MARSHALLER( QListKAutoSaveFile, QList<KAutoSaveFile*> )
// DEF_CONTAINER_MARSHALLER( QListKCatalogName, QList<KCatalogName> )
DEF_CONTAINER_MARSHALLER( QListKConfigSkeletonItem, QList<KConfigSkeletonItem*> )
DEF_CONTAINER_MARSHALLER( QListKCoreConfigSkeletonItemEnumChoice, QList<KCoreConfigSkeleton::ItemEnum::Choice> )
DEF_CONTAINER_MARSHALLER( QListKCoreConfigSkeletonItemEnumChoice2, QList<KCoreConfigSkeleton::ItemEnum::Choice2> )
DEF_CONTAINER_MARSHALLER( QListKJob, QList<KJob*> )
DEF_CONTAINER_MARSHALLER( QListKLocaleDigitSet, QList<KLocale::DigitSet> )
DEF_CONTAINER_MARSHALLER( QListKPluginInfo, QList<KPluginInfo> )
DEF_CONTAINER_MARSHALLER( QListKProtocolInfoExtraField, QList<KProtocolInfo::ExtraField> )
DEF_CONTAINER_MARSHALLER( QListKServiceAction, QList<KServiceAction> )
DEF_CONTAINER_MARSHALLER( QListKSslCipher, QList<KSslCipher> )
DEF_CONTAINER_MARSHALLER( QListKSslError, QList<KSslError> )
DEF_CONTAINER_MARSHALLER( QListKTimeZoneLeapSeconds, QList<KTimeZone::LeapSeconds> )
DEF_CONTAINER_MARSHALLER( QListKTimeZonePhase, QList<KTimeZone::Phase> )
DEF_CONTAINER_MARSHALLER( QListKTimeZoneTransition, QList<KTimeZone::Transition> )
DEF_CONTAINER_MARSHALLER( QListKUser, QList<KUser> )
DEF_CONTAINER_MARSHALLER( QListKUserGroup, QList<KUserGroup> )
DEF_CONTAINER_MARSHALLER( QListQByteArray, QList<QByteArray> )
DEF_CONTAINER_MARSHALLER( QListQDateTime, QList<QDateTime> )
DEF_CONTAINER_MARSHALLER( QListQSslCertificate, QList<QSslCertificate> )

Marshall::TypeHandler KDECoreHandlers[] = {
//     { "QList<KAboutLicense>", marshall_QListKAboutLicense },
//    { "QList<KAboutPerson>", marshall_QListKAboutPerson },
    { "QList<KAuth::Action>&", marshall_QListKAuthAction },
    { "QList<KAuth::Action>*", marshall_QListKAuthAction },
    { "QList<KAutoSaveFile*>", marshall_QListKAutoSaveFile },
//    { "QList<KCatalogName>&", marshall_QListKCatalogName },
    { "QList<KConfigSkeletonItem*>", marshall_QListKConfigSkeletonItem },
    { "QList<KCoreConfigSkeleton::ItemEnum::Choice2>", marshall_QListKCoreConfigSkeletonItemEnumChoice2 },
    { "QList<KCoreConfigSkeleton::ItemEnum::Choice2>&", marshall_QListKCoreConfigSkeletonItemEnumChoice2 },
    { "QList<KCoreConfigSkeleton::ItemEnum::Choice>", marshall_QListKCoreConfigSkeletonItemEnumChoice },
    { "QList<KCoreConfigSkeleton::ItemEnum::Choice>&", marshall_QListKCoreConfigSkeletonItemEnumChoice },
    { "QList<KJob*>&", marshall_QListKJob },
    { "QList<KLocale::DigitSet>", marshall_QListKLocaleDigitSet },
    { "QList<KPluginInfo>", marshall_QListKPluginInfo },
    { "QList<KProtocolInfo::ExtraField>", marshall_QListKProtocolInfoExtraField },
    { "QList<KServiceAction>", marshall_QListKServiceAction },
    { "QList<KSslCipher>", marshall_QListKSslCipher },
    { "QList<KSslCipher>&", marshall_QListKSslCipher },
    { "QList<KSslError>", marshall_QListKSslError },
    { "QList<KSslError>&", marshall_QListKSslError },
    { "QList<KTimeZone::LeapSeconds>", marshall_QListKTimeZoneLeapSeconds },
    { "QList<KTimeZone::LeapSeconds>&", marshall_QListKTimeZoneLeapSeconds },
    { "QList<KTimeZone::Phase>", marshall_QListKTimeZonePhase },
    { "QList<KTimeZone::Phase>&", marshall_QListKTimeZonePhase },
    { "QList<KTimeZone::Transition>", marshall_QListKTimeZoneTransition },
    { "QList<KTimeZone::Transition>&", marshall_QListKTimeZoneTransition },
    { "QList<KUser>", marshall_QListKUser },
    { "QList<KUserGroup>", marshall_QListKUserGroup },
    { "QList<QByteArray>", marshall_QListQByteArray },
    { "QList<QByteArray>&", marshall_QListQByteArray },
    { "QList<QDateTime>", marshall_QListQDateTime },
    { "QList<QSslCertificate>", marshall_QListQSslCertificate },
    { "QList<QSslCertificate>&", marshall_QListQSslCertificate },
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

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
