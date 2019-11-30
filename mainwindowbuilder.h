/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include "FileIconProvider.h"

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QTreeView>
#include <QFileIconProvider>
#include <QDesktopWidget>
#include <qmarkdowntextedit.h>
#include <QSplitter>
#include <QScreen>
#include <QMetaObject>
#include <QLabel>
#include <QScrollArea>
#include <QStandardItemModel>
#include "settings/settings_def.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{

public:
    QMarkdownTextEdit *markdownEditor;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QHBoxLayout *bodyLayout;
    QSplitter *splitter;
    QTreeView *fileTree;
    QTreeView *tocTree;
    QStandardItemModel *tocModel;
    QFileSystemModel *fileTreeModel;
    // image viewer part
    QLabel *imageLabel;
    QScrollArea *imageScrollArea;
    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("MainWindow"));

        int lastWidth = DMSettings::getInt(KEY_LAST_WIN_WIDTH);
        int lastHeight = DMSettings::getInt(KEY_LAST_WIN_HEIGHT);

        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int windowWidth = screenGeometry.width();
        int windowHeight = screenGeometry.height();

        if (lastWidth > 0 && lastHeight > 0) {
            windowWidth = lastWidth;
            windowHeight = lastHeight;
        } else {
            float factor = windowWidth >= 1920 ? 0.618f : 0.8f;
            if (windowWidth <= 800) {
                factor = 1.0f;
            }
            windowWidth *= factor;
            windowHeight *= factor;
            DMSettings::setInt(KEY_LAST_WIN_WIDTH, windowWidth);
            DMSettings::setInt(KEY_LAST_WIN_HEIGHT, windowHeight);
        }

        // TODO: handle window size changed
        mainWindow->resize(windowWidth, windowHeight);

        bodyLayout = new QHBoxLayout;
        // 横向 flex 布局
        splitter = new QSplitter;
        fileTree = new QTreeView;
        tocTree = new QTreeView;
        tocModel = new QStandardItemModel(mainWindow);

        // 竖向 flex 布局
        auto leftPanelSplitter = new QSplitter(Qt::Vertical);
        leftPanelSplitter->addWidget(fileTree);
        leftPanelSplitter->setStretchFactor(0, 3);
        leftPanelSplitter->addWidget(tocTree);
        leftPanelSplitter->setStretchFactor(1, 2);

        QStringList headers;
        headers << mainWindow->tr("table of contents");

        tocModel->setHorizontalHeaderLabels(headers);

        //register the model
        tocTree->setModel(tocModel);
        tocTree->expandAll();
        QObject::connect(tocTree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), mainWindow, SLOT(handleTocClicked(const QItemSelection&,const QItemSelection&)));

        bodyLayout->addWidget(splitter);
        splitter->addWidget(leftPanelSplitter);
        splitter->setStretchFactor(0, 1);

        fileTreeModel = new QFileSystemModel(mainWindow);
        fileTreeModel->setIconProvider(new DMFileIconProvider);
        fileTreeModel->setReadOnly(false);
        QObject::connect(fileTreeModel, SIGNAL(fileRenamed(const QString &, const QString &, const QString &)),
                         mainWindow,
                         SLOT(handleFileRenamed(const QString &, const QString &, const QString &)));

        // Set filter
        fileTreeModel->setFilter(QDir::NoDotAndDotDot |
                                QDir::AllDirs | QDir::Files);

        // drag & drop
        fileTree->setDragEnabled(true);
//        fileTree->viewport()->setAcceptDrops(true);
        fileTree->setDropIndicatorShown(true);
        fileTree->setDragDropMode(QAbstractItemView::InternalMove);

        fileTree->setSortingEnabled(true);
        fileTree->sortByColumn(0, Qt::AscendingOrder);
        fileTree->setSelectionBehavior(QAbstractItemView::SelectItems);
        fileTree->setSelectionMode(QAbstractItemView::SingleSelection);
        fileTree->setModel(fileTreeModel);
        QObject::connect(fileTree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), mainWindow, SLOT(fileSelectionChanged(const QItemSelection&,const QItemSelection&)));
        fileTree->setContextMenuPolicy(Qt::CustomContextMenu);

        QObject::connect(fileTree, SIGNAL(customContextMenuRequested(const QPoint &)),
                mainWindow, SLOT(contextMenu(const QPoint &)));

        fileTree->setHeaderHidden(true);
        for (int i = 1; i < fileTreeModel->columnCount(); ++i) {
            fileTree->hideColumn(i);
        }

        QFont font = QFont();
        font.setPointSize(15);
        font.setFamily("Source Code Variable");
        fileTree->setFont(font);
        fileTree->setStyleSheet("QWidget {background-color:#FFFAE4; color:#434C5B; selection-background-color:#DAEFD0; selection-color:#1CA96B; }");
        tocTree->setFont(font);
        tocTree->setStyleSheet("QWidget {background-color:#FFFAE4; color:#434C5B; selection-background-color:#DAEFD0; selection-color:#1CA96B; }");

        // create markdown editor
        markdownEditor = new QMarkdownTextEdit(mainWindow);
        splitter->addWidget(markdownEditor);
        splitter->setStretchFactor(1, 3);

        // we handle drag & drop globally in MainWindow, so disable it here
        markdownEditor->viewport()->setAcceptDrops(false);

        imageLabel = new QLabel;
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);

        imageScrollArea = new QScrollArea;
        imageScrollArea->setBackgroundRole(QPalette::Dark);
        imageScrollArea->setWidget(imageLabel);
        imageScrollArea->setWidgetResizable(true);
        imageScrollArea->setVisible(false);
        splitter->addWidget(imageScrollArea);
        splitter->setStretchFactor(2, 3);

        // create center widget
        QWidget *centerWidget = new QWidget();
        centerWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centerWidget->setLayout(bodyLayout);

        mainWindow->setCentralWidget(centerWidget);

        menuBar = new QMenuBar(mainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, windowWidth, 22));
        mainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(mainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        mainWindow->setStatusBar(statusBar);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "SyncFolder", nullptr));
    } // retranslateUi
};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
