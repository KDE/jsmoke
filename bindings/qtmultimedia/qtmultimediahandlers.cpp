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

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QVideoFrame>

Q_DECLARE_METATYPE(QAudioFormat::Endian)
Q_DECLARE_METATYPE(QAudioFormat::SampleType)
// Q_DECLARE_METATYPE(QList<QAudioDeviceInfo>)
Q_DECLARE_METATYPE(QList<QAudioFormat::Endian>)
Q_DECLARE_METATYPE(QList<QAudioFormat::SampleType>)
Q_DECLARE_METATYPE(QList<QByteArray>)
Q_DECLARE_METATYPE(QList<QVideoFrame::PixelFormat>)

namespace JSmoke {

// DEF_CONTAINER_MARSHALLER(QListQAudioDeviceInfo, QList<QAudioDeviceInfo>)
DEF_CONTAINER_MARSHALLER(QListQAudioFormatEndian, QList<QAudioFormat::Endian>)
DEF_CONTAINER_MARSHALLER(QListQAudioFormatSampleType, QList<QAudioFormat::SampleType>)
DEF_CONTAINER_MARSHALLER(QListQByteArray, QList<QByteArray>)
DEF_CONTAINER_MARSHALLER(QListQVideoFramePixelFormat, QList<QVideoFrame::PixelFormat>)

Marshall::TypeHandler QtMultimediaHandlers[] = {
//    { "QList<QAudioDeviceInfo>", marshall_QListQAudioDeviceInfo },
    { "QList<QAudioFormat::Endian>", marshall_QListQAudioFormatEndian },
    { "QList<QAudioFormat::SampleType>", marshall_QListQAudioFormatSampleType },
    { "QList<QByteArray>", marshall_QListQByteArray },
    { "QList<QVideoFrame::PixelFormat>", marshall_QListQVideoFramePixelFormat },
    { 0, 0 }
};

void registerQtMultimediaTypes(QScriptEngine * engine)
{
//    qScriptSmokeRegisterSequenceMetaType<QList<QAudioDeviceInfo> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QAudioFormat::Endian> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QAudioFormat::SampleType> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QByteArray> >(engine);
    qScriptSmokeRegisterSequenceMetaType<QList<QVideoFrame::PixelFormat> >(engine);

    return;
}

}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
