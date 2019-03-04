#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
/*!
    \class MainWidget
    \brief The MainWidget class represents the main widget for the application.

    It consists of three main parts: the input box for "painting" the graph,
    the output box for the correct representation and a "list" of settings
    for the graphical representation.

 */
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // MAINWIDGET_H
