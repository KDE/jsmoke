# - Try to find SMOKE
#
# defines
#
# SMOKE_QTCORE_FOUND - smoke was found on the system
# SMOKE_INCLUDE_DIR - where to find the smoke headers
# SMOKE_QTCORE_LIBRARY - where to find the smoke library

# copyright (c) 2007 Arno Rehn arno@arnorehn.de
#
# Redistribution and use is allowed according to the terms of the GPL license.

FIND_PATH (SMOKE_INCLUDE_DIR smoke.h)
FIND_LIBRARY (SMOKE_QTCORE_LIBRARY smokeqtcore)

SET (SMOKE_QTCORE_FOUND FALSE)

IF (SMOKE_INCLUDE_DIR AND SMOKE_QTCORE_LIBRARY)
	SET (SMOKE_QTCORE_FOUND TRUE)
ENDIF (SMOKE_INCLUDE_DIR AND SMOKE_QTCORE_LIBRARY)

IF (SMOKE_QTCORE_FOUND)
	IF (NOT SmokeQtCore_FIND_QUIETLY)
		MESSAGE(STATUS "Found SmokeQtCore: ${SMOKE_QTCORE_LIBRARY}")
	ENDIF (NOT SmokeQtCore_FIND_QUIETLY)
ELSE (SMOKE_QTCORE_FOUND)
	IF (SmokeQtCore_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find SmokeQtCore")
	ENDIF (SmokeQt_FIND_REQUIRED)
ENDIF (SMOKE_QTCORE_FOUND)

MARK_AS_ADVANCED(SMOKE_INCLUDE_DIR SMOKE_QTCORE_LIBRARY)
