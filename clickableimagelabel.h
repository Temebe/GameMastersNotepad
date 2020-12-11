#ifndef CLICKABLEIMAGELABEL_H
#define CLICKABLEIMAGELABEL_H

#include <QLabel>
#include <QObject>

// Huge thanks to phyatt for making scaling possible
// https://stackoverflow.com/a/22618496

class ClickableImageLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableImageLabel(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ClickableImageLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void setOriginalPixmap(const QPixmap &pixmap);
    virtual int heightForWidth( int width ) const override;
    virtual QSize sizeHint() const override;
    QPixmap scaledPixmap() const;

public slots:
    void mousePressEvent(QMouseEvent *event) override;
    void setPixmap(const QPixmap &pixmap);
    void resizeEvent(QResizeEvent *event) override;

signals:
    void clicked();

private:
    QPixmap originalPixmap;
};

#endif // CLICKABLEIMAGELABEL_H
