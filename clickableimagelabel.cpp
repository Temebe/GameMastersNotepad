#include "clickableimagelabel.h"

#include <QPixmap>
#include <QResizeEvent>

ClickableImageLabel::ClickableImageLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
    this->setMinimumSize(1,1);
    setScaledContents(false);
}

ClickableImageLabel::ClickableImageLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    this->setMinimumSize(1,1);
    setScaledContents(false);
}

int ClickableImageLabel::heightForWidth(int width) const
{
    return originalPixmap.isNull() ? this->height() : ((qreal)originalPixmap.height()*width)/originalPixmap.width();
}

QSize ClickableImageLabel::sizeHint() const
{
    int w = this->width();
    return QSize(w, heightForWidth(w));
}

QPixmap ClickableImageLabel::scaledPixmap() const
{
    return originalPixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void ClickableImageLabel::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit clicked();
}

void ClickableImageLabel::setPixmap(const QPixmap &pixmap)
{
    originalPixmap = pixmap;
    QLabel::setPixmap(scaledPixmap());
}

void ClickableImageLabel::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    if(!originalPixmap.isNull()) {
        QLabel::setPixmap(scaledPixmap());
    }

}
