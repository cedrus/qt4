load(qttest_p4)
SOURCES         += tst_qdirmodel.cpp

wince*|symbian {
	addit.sources = dirtest\\test1\\*
	addit.path = dirtest\\test1
	tests.sources = test\\*
	tests.path = test
        sourceFile.sources = tst_qdirmodel.cpp
        sourceFile.path = .
	DEPLOYMENT += addit tests sourceFile
}

wince*: {
    DEFINES += SRCDIR=\\\"./\\\"
} else:symbian {
    TARGET.UID3 = 0xE0340003
    DEFINES += SYMBIAN_SRCDIR_UID=$$lower($$replace(TARGET.UID3,"0x",""))
} else {
    DEFINES += SRCDIR=\\\"$$PWD/\\\"
}

