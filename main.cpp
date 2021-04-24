#include "mdeditor.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    MDEditor w;


    w.setMarkdownText(u8R"(
成都市
骨骼
我让
你

[Baidu](https://www.baidu.com/)
[测试](https://m/)
# 哈哈哈哈

**粗体** *gggg* ++gffff++

*斜体*

~~删除线~~

++下划线++

H~2~O

2^10^

1. 有序列表


- 无序列表
  - 子列表 sublist
    - that is very easy to use

## Typographic replacements

Enable typographer option to see result.

(c) (C) (r) (R) (tm) (TM) (p) (P) +-

test.. test... test..... test?..... test!....

!!!!!! ???? ,,  -- ---

"Smartypants, double quotes" and 'single quotes'


## Emphasis

**This is bold text**

__This is bold text__

*This is italic text*

_This is italic text_

~~Strikethrough~~

[链接标题](链接地址)

this is inline code such as `QPainter` is the way to realize `paintevent` of `HTMLViewer`
`代码`高亮可以提示我们阅读重点

```
w.setTitle("Markdown Test");
w.setTitleFont(QFont(u8"Helvetica,方正悠黑简体 505L",16,57));
w.setEditStyleSheet("background-color:white;color:#333333;");
```
<table cellpadding="15">
<tr>
<td>123</td>
<td>123</td>
<td>123</td>
</tr>
</table>

                      )");

    w.setTitle("Markdown Test");
    w.setTitleFont(QFont(u8"Helvetica,方正悠黑简体 508R",16,57));
//    w.setTitleHide(true);

    QFont font(u8"Helvetica,方正悠黑简体 508R");
    font.setPixelSize(14);
    w.setEditFont(font);
    w.setPreviewFont(font);
//    w.setPreviewFont(QFont(u8"portada,方正舒体",14));

    w.setEditStyleSheet("background-color:white;color:#333333;");
    w.setPreviewStyleSheet("background-color:#FFFFFF;color:#333333;");
    w.setStyleSheet("QWidget#MDEditor{background-color:#F8F8F8;}");

    w.toPreviewMode();
    w.show();
    return app.exec();
}
