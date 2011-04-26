/*
 *   Copyright 2009-2011 by Richard Dale <richard.j.dale@gmail.com>

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

#ifndef JSMOKE_DEBUG_H
#define JSMOKE_DEBUG_H

#include <QtCore/qglobal.h>

#include "jsmoke_export.h"

namespace JSmoke {
    namespace Debug {
        extern uint DoDebug;

        enum QtDebugChannel {
            None = 0x00,
            MethodMatches = 0x01,
            Properties = 0x02,
            Calls = 0x04,
            GC = 0x08,
            Virtual = 0x10,
            Verbose = 0x20
        };
    }
}

#endif // JSMOKE_DEBUG_H