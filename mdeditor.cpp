#include "htmlviewer.h"
#include "mdeditor.h"
#include "ui_mdeditor.h"

#include <QShortcut>
#include <QTextDocumentFragment>
#include <QTextFragment>

MDEditor::MDEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MDEditor)
{
    ui->setupUi(this);

//    setWindowFlags(Qt::FramelessWindowHint);//设置无标题
//    setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明

    QAction *Action_editMode = new QAction(tr("edit/preview"),this);
    Action_editMode->setShortcut(QKeySequence(tr("Ctrl+E")));
    connect(Action_editMode,SIGNAL(triggered(bool)),this,SLOT(switchEditPreview()));
    this->addAction(Action_editMode);

    QAction *Action_debug = new QAction(tr("Debug"),this);
    Action_debug->setShortcut(QKeySequence(tr("Ctrl+D")));
    connect(Action_debug,SIGNAL(triggered(bool)),this,SLOT(slot_test()));
    this->addAction(Action_debug);

    connect(ui->editMode_btn,SIGNAL(clicked(bool)),this,SLOT(switchEditPreview()));

    docNote_preview = new HTMLViewer;
    ui->scrollArea->setWidget(docNote_preview);
    docNote_preview->setObjectName("docNote_preview");
    docNote_preview->installEventFilter(this);

    initJS();
//    toPreviewMode();


}

MDEditor::~MDEditor()
{
    delete ui;
}

void MDEditor::initJS(){
    loadJSFileRes(&myJSEngine,":/res/js/markdown-it.js");
    loadJSFileRes(&myJSEngine,":/res/js/markdown-it-ins.js");
    loadJSFileRes(&myJSEngine,":/res/js/markdown-it-sub.js");
    loadJSFileRes(&myJSEngine,":/res/js/markdown-it-sup.js");
    loadJSFileRes(&myJSEngine,":/res/js/markdown-it-mark.js");

    evalJSProgram(&myJSEngine,"var md = markdownit({html: true,linkify: true,typographer: true})");
    evalJSProgram(&myJSEngine,"md.use(markdownitIns);");
    evalJSProgram(&myJSEngine,"md.use(markdownitSub);");
    evalJSProgram(&myJSEngine,"md.use(markdownitSup);");
    evalJSProgram(&myJSEngine,"md.use(markdownitMark);");

    myJSEngine.globalObject().setProperty("text", "++123++**kkkk**");
    QJSValue markdownText = myJSEngine.evaluate("md.render(text)");
    if(!markdownText.isError()) qDebug()<<markdownText.toString();
}

void MDEditor::loadJSFileRes(QJSEngine *jsEngine,QString res_filename){
    QFile requireJSFile(res_filename);
    qDebug()<<requireJSFile.fileName();
    if (requireJSFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        auto jsStr = QString::fromStdString(requireJSFile.readAll().toStdString());
        qDebug()<<jsStr.length();
        evalJSProgram(jsEngine,jsStr);
    }
}

QJSValue MDEditor::evalJSProgram(QJSEngine *jsEngine,QString program){
    QJSValue result = jsEngine->evaluate(program);
    ++evalJS_count;
    if (result.isError())
        qDebug()<< evalJS_count
                << "Uncaught exception at line"<< result.property("lineNumber").toInt()<< ":" << result.toString();
    return result;
}

void MDEditor::setTitle(QString title)              {   ui->title->setText(title);                                  }
void MDEditor::setTitleFont(QFont font)             {   ui->title->setFont(font);                                   }
void MDEditor::setEditFont(QFont font)              {   ui->docNote_edit->setFont(font);                            }
void MDEditor::setPreviewFont(QFont font)           {   allow = true; docNote_preview->setFont(font);               }
void MDEditor::setEditStyleSheet(QString str)       {   ui->docNote_edit->setStyleSheet(str);                       }
void MDEditor::setPreviewStyleSheet(QString str)    {   docNote_preview->setStyleSheet(str);                        }
void MDEditor::setMarkdownText(QString text)        {   ui->docNote_edit->setPlainText(text);toPreviewMode();       }
QString MDEditor::getMarkdownText()                 {   return ui->docNote_edit->toPlainText();                     }
QString MDEditor::getMarkdownHtml()                 {   return docNote_preview->toHtml();                           }
void MDEditor::setToolHide(bool hide)               {   ui->tool_frame->show(); if(hide) ui->tool_frame->hide();    }
void MDEditor::setTitleHide(bool hide)              {   ui->header_frame->show();if(hide) ui->header_frame->hide(); }



bool MDEditor::eventFilter(QObject *watched, QEvent *event){
//    qDebug()<<watched->objectName();
    if(watched == docNote_preview){
        if(event->type() == QEvent::FontChange){
            if(!allow){
                qDebug("fontchange");
                return true;
            }
            allow = false;
        }
    }
    return QWidget::eventFilter(watched,event);

}


void MDEditor::slot_test(){
    qDebug()<<"slot_test";
    qDebug()<<docNote_preview->document()->toHtml();
//    docNote_preview->setHtml(docNote_preview->document()->toHtml().replace("-qt-list-indent: 2","-qt-list-indent: 2;text-indent:-20px"));
//    qDebug()<<docNote_preview->document()->blockCount();
//    QTextBlock textBlock = docNote_preview->document()->begin();
//    while(textBlock.isValid()){
//        qDebug("\n");
//        qDebug()<<textBlock.text();
//        qDebug()<<textBlock.layout()->lineAt(0).width();
//        qDebug()<<textBlock.layout()->lineAt(0).height();
//        qDebug()<<textBlock.layout()->position();
//        qDebug()<<textBlock.layout()->boundingRect(); //indent , ,width x height

//        textBlock = textBlock.next();
//    }
//    QTextCursor tc = ui->docNote_edit->textCursor();
//    qDebug()<<tc.position()<<tc.selectedText()<<tc.selectionStart()<<tc.selectionEnd();
//    ui->docNote_edit->copy();
//    qDebug()<<ui->docNote_edit->textCursor().selectedText();
}


void MDEditor::switchEditPreview(){
    if(isEditing){
        toPreviewMode();
    }else{
        toEditMode();
    }
}

void MDEditor::toEditMode(){
    ui->editMode_btn->setIcon(QIcon(":/res/img/view.png"));
    ui->scrollArea->hide();
    ui->docNote_edit->show();
    isEditing = true;

    ui->docNote_edit->setFocus();
    ui->docNote_edit->textCursor().setPosition(editPos);
}

void MDEditor::toPreviewMode(){
    editPos = ui->docNote_edit->textCursor().position();
    myJSEngine.globalObject().setProperty("text",ui->docNote_edit->toPlainText());
    QJSValue result = myJSEngine.evaluate("md.render(text)");
    if(!result.isError()){
        markdown_html = result.toString();
        qDebug()<<markdown_html;
        ModifyHtmlTags();
        ModifyHtmlTagsStyle();
        qDebug()<<markdown_html;
    }else{
        markdown_html = "error";
    }
    docNote_preview->setHtml(markdown_html);

    ui->editMode_btn->setIcon(QIcon(":/res/img/edit.png"));
    ui->scrollArea->show();
    ui->docNote_edit->hide();
    isEditing = false;
}

void MDEditor::ModifyHtmlTags(){
    markdown_html.replace("ins","u");
}

void MDEditor::ModifyHtmlTagsStyle(){
    markdown_html.replace("<a href","<a style='color:#428BCA' href");

//    markdown_html.replace("<p>","<p style='white-space:pre-wrap'>");

    markdown_html.replace("<blockquote>",QString("<blockquote style='margin-left:%1px;'>").arg(QUOTE_INDENT_WIDTH));

    markdown_html.replace("<pre><code>",QString(u8"<pre style='background-color:%1;margin-left:%2px;'><code style='font-family:Source Code Pro,方正悠黑简体 508R;'>").arg(CODE_BG).arg(CODE_X));
    markdown_html.replace("\n</code></pre>","</code></pre>");

    markdown_html.replace("<code>",u8"<code style='background-color:#F9F2F4;color:#c7254e;white-space:pre-wrap;font-family:Source Code Pro,方正悠黑简体 508R;'> ");
    markdown_html.replace(QRegExp("</code>((?!</pre)[^>]*)")," </code>\\1");
}

///////////////////////////////////////////////////////////////////

int MDEditor::getStartOfLine(int pos){
    QTextCursor tc = ui->docNote_edit->textCursor();
    int pos_start = tc.selectionStart();
    int pos_end = tc.selectionEnd();

    tc.setPosition(pos);
    tc.movePosition(QTextCursor::StartOfLine);
    int res = tc.position();

    tc.setPosition(pos_start);
    tc.setPosition(pos_end,QTextCursor::KeepAnchor);
    return res;
}

int MDEditor::getEndOfLine(int pos){
    QTextCursor tc = ui->docNote_edit->textCursor();
    int pos_start = tc.selectionStart();
    int pos_end = tc.selectionEnd();

    tc.setPosition(pos);
    tc.movePosition(QTextCursor::EndOfLine);
    int res = tc.position();

    tc.setPosition(pos_start);
    tc.setPosition(pos_end,QTextCursor::KeepAnchor);
    return res;
}

void MDEditor::AddTip(QString tip,bool keepSelection){
    QTextCursor tc = ui->docNote_edit->textCursor();
    int pos_start = tc.selectionStart();
    tc.insertText(tip);
    if(keepSelection)
        tc.setPosition(pos_start,QTextCursor::KeepAnchor);
    else
        tc.setPosition(pos_start,QTextCursor::MoveAnchor);
    ui->docNote_edit->setTextCursor(tc);
}

void MDEditor::addBeforeAndAfter(QString tip,QString addition){
    addBeforeAndAfter(tip,addition,addition);
}

void MDEditor::addBeforeAndAfter(QString tip,QString addition_before,QString addition_after,bool combine){ //,bool keepSelection
    if(!isEditing) return;
    ui->docNote_edit->setFocus();

    QTextCursor tc = ui->docNote_edit->textCursor();
    int pos_start = tc.selectionStart();
    int pos_end = tc.selectionEnd();
    QString selectedText;
    if(pos_start == pos_end){
        selectedText = tip; // 提示文本
    }else{
        selectedText = tc.selection().toPlainText();
        if(combine) selectedText.replace("\n",""); //合并跨行选取文本
    }
    tc.clearSelection();

    int n1 = addition_before.length();
    int n2 = addition_after.length();
    int m = selectedText.length();

    tc.setPosition(pos_start);
    tc.movePosition(QTextCursor::Left,QTextCursor::KeepAnchor,n1);
    QString start_left = tc.selection().toPlainText();
    tc.clearSelection();

    tc.setPosition(pos_end);
    tc.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,n2);
    QString end_right = tc.selection().toPlainText();
    tc.clearSelection();

//    qDebug()<<start_left<<selectedText<<end_right;
//    qDebug()<<start_left.length()<<selectedText.length()<<end_right.length();
//    qDebug()<<pos_start<<pos_end;
    bool Redo = (start_left == addition_before && end_right == addition_after);
    if(Redo){
        tc.setPosition(pos_start - n1);
        tc.setPosition(pos_end + n2,QTextCursor::KeepAnchor);
        if(selectedText == tip){
            tc.insertText("");
            m = 0;
        }else{
            tc.insertText(selectedText);
        }
        tc.setPosition(pos_start - n1);
        tc.setPosition(pos_start - n1 + m,QTextCursor::KeepAnchor);
    }else{
        tc.setPosition(pos_start);
        tc.setPosition(pos_end,QTextCursor::KeepAnchor);
        tc.insertText(addition_before + selectedText + addition_after);
        tc.setPosition(pos_start + n1);
        tc.setPosition(pos_start + n1 + m,QTextCursor::KeepAnchor);
    }
    ui->docNote_edit->setTextCursor(tc);
}

void MDEditor::addStart(QString tip,QString addition,int max){
    if(!isEditing) return;
    ui->docNote_edit->setFocus();

    QTextCursor tc = ui->docNote_edit->textCursor();
    int pos_start = tc.selectionStart();
    int pos_end = tc.selectionEnd();
    QString selectedText;
    int total_start,total_end;
    if(getStartOfLine(pos_start) == getEndOfLine(pos_end)){ // at new line
        selectedText = tip; // 提示文本
    }else{
        tc.setPosition(getStartOfLine(pos_start));
        tc.setPosition(getEndOfLine(pos_end),QTextCursor::KeepAnchor);
        selectedText = tc.selection().toPlainText().replace("\n",""); //合并跨行选取文本
        total_start = tc.selectionStart();
        total_end = tc.selectionEnd();
    }
    tc.clearSelection();

    int n1 = addition.length();

    qDebug()<<pos_start<<pos_end<<total_start<<total_end<<selectedText;

    int count = -1;
    QString next;
    tc.setPosition(total_start);
    do{
        ++count;
        tc.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,n1);
        next = tc.selection().toPlainText();
        tc.clearSelection();
        qDebug()<<"this"<<next<<"count"<<count;
    }while(next == addition);
    if(count >= max){
        tc.setPosition(total_start);
        tc.setPosition(total_end,QTextCursor::KeepAnchor);
        selectedText.replace(QRegExp(QString("^[%1]{0,5}[%1 ]").arg(addition)),"");
        tc.insertText(selectedText);
    }else{
        tc.setPosition(total_start);
        tc.setPosition(total_end,QTextCursor::KeepAnchor);
        if(!next.startsWith(" ")) selectedText.replace(QRegExp(QString("^([%1]{0,5})").arg(addition)),"\\1 ");
        tc.insertText(addition + selectedText);
    }

    ui->docNote_edit->setTextCursor(tc);
}

void MDEditor::on_header_btn_clicked()          {   addStart(tr(u8"标题"),"#",4);                             }
void MDEditor::on_bold_btn_clicked()            {   addBeforeAndAfter(tr(u8"粗体"),"**");                     }
void MDEditor::on_italic_btn_clicked()          {   addBeforeAndAfter(tr(u8"斜体"),"_");                      }
void MDEditor::on_strikethrough_btn_clicked()   {   addBeforeAndAfter(tr(u8"删除线"),"~~");                   }
void MDEditor::on_underline_btn_clicked()       {   addBeforeAndAfter(tr(u8"下划线"),"++");                   }
void MDEditor::on_subscript_btn_clicked()       {   addBeforeAndAfter(tr(u8"下标"),"~");                      }
void MDEditor::on_superscript_btn_clicked()     {   addBeforeAndAfter(tr(u8"上标"),"^");                      }
void MDEditor::on_numberedlist_btn_clicked()    {   addStart(tr(u8"有序列表"),"1.");                           }
void MDEditor::on_bulletedlist_btn_clicked()    {   addStart(tr(u8"无序列表"),"-");                            }
void MDEditor::on_quote_btn_clicked()           {   addStart(tr(u8"引用"),">",3);                             }
void MDEditor::on_link_btn_clicked()            {   addBeforeAndAfter("",tr(u8"[标题]"),tr(u8"(http://)"));   }
void MDEditor::on_inlinecode_btn_clicked()      {   addBeforeAndAfter(tr(u8"代码"),"`");                      }
void MDEditor::on_blockcode_btn_clicked()       {   addBeforeAndAfter(tr(u8"代码块"),"\n```\n","\n```",false);}
