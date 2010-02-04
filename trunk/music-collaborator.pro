HEADERS += MCMainWindow.h \
    PreferencesDialog.h \
    LocalRepositoryModel.h \
    FileItem.h \
    FtpRepositoryModel.h \
    FtpFileItem.h \
    CompressionTask.h
SOURCES += MCMainWindow.cpp \
    main.cpp \
    PreferencesDialog.cpp \
    LocalRepositoryModel.cpp \
    FileItem.cpp \
    FtpRepositoryModel.cpp \
    FtpFileItem.cpp \
    CompressionTask.cpp
FORMS += MCMainWindow.ui \
    PreferencesDialog.ui
RESOURCES += Resources.qrc
QT += network
OTHER_FILES += Outline.txt
