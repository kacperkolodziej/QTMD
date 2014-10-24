#include "headers/htmlviewer.hpp"
#include "ui_htmlviewer.h"

HtmlViewer::HtmlViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HtmlViewer)
{
    ui->setupUi(this);
}

HtmlViewer::~HtmlViewer()
{
    delete ui;
}

void HtmlViewer::setData(QString title, QUrl url)
{
    setWindowTitle(title);
    ui->lblTitle->setText(title);
    ui->browser->setSource(url);
    ui->browser->setOpenExternalLinks(true);
    ui->browser->setOpenLinks(true);
}

void HtmlViewer::on_btnClose_clicked()
{
    close();
}
