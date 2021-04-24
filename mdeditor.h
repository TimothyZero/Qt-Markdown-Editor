#ifndef MDEDITOR_H
#define MDEDITOR_H

#include "htmlviewer.h"

#include <QJSEngine>
#include <QWidget>
#include <QDebug>
#include <QTextLayout>
#include <QTextBlock>

namespace Ui {
class MDEditor;
}

class MDEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MDEditor(QWidget *parent = 0);
    ~MDEditor();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MDEditor *ui;

    bool isEditing = true;
    bool allow = true;
    int editPos;
    int evalJS_count = 0;

    QJSEngine myJSEngine;
    QString markdown_html;

    HTMLViewer *docNote_preview;

private slots:
    void slot_test();
    void switchEditPreview();

    void on_header_btn_clicked();
    void on_bold_btn_clicked();
    void on_italic_btn_clicked();
    void on_strikethrough_btn_clicked();
    void on_underline_btn_clicked();
    void on_subscript_btn_clicked();
    void on_superscript_btn_clicked();
    void on_numberedlist_btn_clicked();
    void on_bulletedlist_btn_clicked();
    void on_quote_btn_clicked();
    void on_link_btn_clicked();
    void on_inlinecode_btn_clicked();
    void on_blockcode_btn_clicked();

private:
    void initJS();
    void loadJSFileRes(QJSEngine *jsEngine,QString res_filename);
    QJSValue evalJSProgram(QJSEngine *jsEngine,QString program);

    int getStartOfLine(int pos);
    int getEndOfLine(int pos);
    void AddTip(QString tip,bool keepSelection = true);
    void addBeforeAndAfter(QString tip,QString addition);
    void addBeforeAndAfter(QString tip,QString addition_before,QString addition_after,bool combine = true);
    void addStart(QString tip,QString addition,int max = 1);

    void ModifyHtmlTags();
    void ModifyHtmlTagsStyle();

public:
    void setTitle(QString title);

    void setTitleFont(QFont font);
    void setEditFont(QFont font);
    void setPreviewFont(QFont font);

    void setEditStyleSheet(QString stylesheet);
    void setPreviewStyleSheet(QString stylesheet);

    void setMarkdownText(QString text);
    QString getMarkdownText();
    QString getMarkdownHtml();

    void setTitleHide(bool hide);
    void setToolHide(bool hide);

    void toEditMode();
    void toPreviewMode();

};

#endif // MDEDITOR_H
