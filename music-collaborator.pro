HEADERS += MCMainWindow.h \
    PreferencesDialog.h \
    LocalRepositoryModel.h \
    FileItem.h \
    FtpRepositoryModel.h \
    FtpFileItem.h
SOURCES += MCMainWindow.cpp \
    main.cpp \
    PreferencesDialog.cpp \
    LocalRepositoryModel.cpp \
    FileItem.cpp \
    FtpRepositoryModel.cpp \
    FtpFileItem.cpp
FORMS += MCMainWindow.ui \
    PreferencesDialog.ui
RESOURCES += Resources.qrc
QT += network
