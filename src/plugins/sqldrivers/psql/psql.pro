TARGET	 = qsqlpsql

HEADERS		= ../../../sql/drivers/psql/qsql_psql.h
SOURCES		= main.cpp \
		  ../../../sql/drivers/psql/qsql_psql.cpp

unix|win32-g++*: {
    !isEmpty(QT_LFLAGS_PSQL) {
        !contains(QT_CONFIG, system-zlib): QT_LFLAGS_PSQL -= -lz
        !static:LIBS *= $$QT_LFLAGS_PSQL
        QMAKE_CXXFLAGS *= $$QT_CFLAGS_PSQL
    }
    !contains(LIBS, .*pq.*):LIBS *= -lpq
}

win32:!win32-g++*:!contains(LIBS, .*pq.* ) LIBS *= -llibpq -lws2_32 -ladvapi32

include(../qsqldriverbase.pri)
