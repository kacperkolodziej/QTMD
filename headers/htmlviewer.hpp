#ifndef HTMLVIEWER_HPP
#define HTMLVIEWER_HPP

#include <QDialog>

namespace Ui {
class HtmlViewer;
}

class HtmlViewer : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlViewer(QWidget *parent = 0);
    ~HtmlViewer();

    void setData(QString, QUrl);

private:
    Ui::HtmlViewer *ui;
};

#endif // HTMLVIEWER_HPP
