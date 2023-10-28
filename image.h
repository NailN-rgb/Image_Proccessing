#ifndef IMAGE_H
#define IMAGE_H
#include <QImage>
#include <QVector>

class Image
{
private:
    qint32 height;
    qint32 width;
    QVector<QVector<QColor>> MainData;
    QString path;
    QImage resultImage;
    QVector<QVector<QColor>> read_file(QString path);
    QImage current_image;

public:
    Image();
    Image(QString path);
    Image(qint32 height, qint32 width, QVector<QVector<QColor>> data, QString path);
    Image(QImage img);
    QImage get_picture( QVector<QVector<QColor>> data);
    void set_height(qint32 height);
    void set_width(qint32 width);
    void set_path(QString path);
    qint32 get_height();
    qint32 get_width();
    QVector<QVector<QColor>> get_data();
    void set_resultImage(QImage resultImage);
    QImage get_resultImage();
    void set_data(QVector<QVector<QColor>> Data);
    QVector<QVector<QColor>> setDataFromImage(QImage img);
    void set_currentImage(QImage img);
    QImage get_currentImage();
};

#endif // IMAGE_H
