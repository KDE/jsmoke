/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

 *   Based on the PerlQt marshalling code by Ashley Winters

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

namespace QtScript {
	
void
marshall_basetype(Marshall *m)
{
    switch(m->type().elem()) {
        
    case Smoke::t_bool:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_bool = m->var().s_bool;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_bool = m->item().s_bool;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
    
    case Smoke::t_char:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_char = m->var().s_char;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_char = m->item().s_char;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
    
    case Smoke::t_uchar:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_uchar = m->var().s_uchar;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_uchar = m->item().s_uchar;
            break;
        default:
            m->unsupported();
            break;
        }
    break;
    
    case Smoke::t_short:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_short = m->var().s_short;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_short = m->item().s_short;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_ushort:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_ushort = m->var().s_ushort;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_ushort = m->item().s_ushort;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_int:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_int = m->var().s_int;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_int = m->item().s_int;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_uint:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_uint = m->var().s_uint;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_uint = m->item().s_uint;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_long:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_long = m->var().s_long;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_long = m->item().s_long;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_ulong:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_ulong = m->var().s_ulong;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_ulong = m->item().s_ulong;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_float:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_float = m->var().s_float;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_float = m->item().s_float;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_double:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_double = m->var().s_double;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_double = m->item().s_double;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_enum:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
            // m->item().s_enum = m->var().s_enum;
            break;
        case Marshall::ToQScriptValue:
            // m->var().s_enum = m->item().s_enum;
            break;
        default:
            m->unsupported();
            break;
        }
        break;
        
    case Smoke::t_class:
        switch(m->action()) {
        case Marshall::FromQScriptValue:
        {
        }
        break;
        
        case Marshall::ToQScriptValue:
        {
        }
        break;
        
        default:
            m->unsupported();
        }
        break;
    
    default:
        m->unsupported();
        break;
    }
}
    
static void marshall_void(Marshall * /*m*/) {}
static void marshall_unknown(Marshall *m) {
    m->unsupported();
}
    
TypeHandler QtScriptHandlers[] = {
    // { "bool*", marshall_boolR },

    { 0, 0 }
};

QHash<QString, TypeHandler *> TypeHandlers;

void InstallHandlers(TypeHandler * handler) {
    while (handler->name != 0) {
        TypeHandlers.insert(handler->name, handler);
        handler++;
    }
}

Marshall::HandlerFn getMarshallFn(const SmokeType &type) {
    if (type.elem()) {
        return marshall_basetype;
    }
    
    if (!type.name()) {
        return marshall_void;
    }
    
    TypeHandler * handler = TypeHandlers[type.name()];
    if (handler == 0 && type.isConst() && strlen(type.name()) > strlen("const ")) {
        handler = TypeHandlers[type.name() + strlen("const ")];
    }
    
    if (handler != 0) {
        return handler->fn;
    }

    return marshall_unknown;
}

}

// kate: space-indent on;
