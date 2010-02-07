HEADERS += MCMainWindow.h \
    PreferencesDialog.h \
    RepositoryModel.h \
    FileItem.h \
    FtpRepositoryModel.h \
    FtpFileItem.h \
    CompressionTask.h
SOURCES += MCMainWindow.cpp \
    main.cpp \
    PreferencesDialog.cpp \
    RepositoryModel.cpp \
    FileItem.cpp \
    FtpRepositoryModel.cpp \
    FtpFileItem.cpp \
    CompressionTask.cpp
FORMS += MCMainWindow.ui \
    PreferencesDialog.ui
RESOURCES += Resources.qrc
QT += network
OTHER_FILES += Outline.txt
