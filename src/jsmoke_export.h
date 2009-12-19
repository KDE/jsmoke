#ifndef JSMOKE_EXPORT_H
#define JSMOKE_EXPORT_H

#if defined(BUILD_JSMOKE)
#  define JSMOKE_EXPORT Q_DECL_EXPORT
#else
#  define JSMOKE_EXPORT Q_DECL_IMPORT
#endif

#endif
