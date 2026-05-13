/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableEmployees;
    QPushButton *btnEditEmployee;
    QPushButton *btnDeleteEmployee;
    QPushButton *btnAddEmployee;
    QPushButton *btnRefresh;
    QWidget *tab_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 0, 280, 330));
        tab_1 = new QWidget();
        tab_1->setObjectName("tab_1");
        verticalLayout = new QVBoxLayout(tab_1);
        verticalLayout->setObjectName("verticalLayout");
        tableEmployees = new QTableWidget(tab_1);
        tableEmployees->setObjectName("tableEmployees");

        verticalLayout->addWidget(tableEmployees);

        btnEditEmployee = new QPushButton(tab_1);
        btnEditEmployee->setObjectName("btnEditEmployee");

        verticalLayout->addWidget(btnEditEmployee);

        btnDeleteEmployee = new QPushButton(tab_1);
        btnDeleteEmployee->setObjectName("btnDeleteEmployee");

        verticalLayout->addWidget(btnDeleteEmployee);

        btnAddEmployee = new QPushButton(tab_1);
        btnAddEmployee->setObjectName("btnAddEmployee");

        verticalLayout->addWidget(btnAddEmployee);

        btnRefresh = new QPushButton(tab_1);
        btnRefresh->setObjectName("btnRefresh");

        verticalLayout->addWidget(btnRefresh);

        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnEditEmployee->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        btnDeleteEmployee->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        btnAddEmployee->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        btnRefresh->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QCoreApplication::translate("MainWindow", "Employees", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
