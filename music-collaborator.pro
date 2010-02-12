HEADERS += MCMainWindow.h \
    PreferencesDialog.h \
    RepositoryModel.h \
    FileItem.h \
    CompressionTask.h \
    RevisionInfo.h
SOURCES += MCMainWindow.cpp \
    main.cpp \
    PreferencesDialog.cpp \
    RepositoryModel.cpp \
    FileItem.cpp \
    CompressionTask.cpp \
    RevisionInfo.cpp
FORMS += MCMainWindow.ui \
    PreferencesDialog.ui
RESOURCES += Resources.qrc
QT += network
OTHER_FILES += Outline.txt
