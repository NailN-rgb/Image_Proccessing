#include "image.h"

Image::Image()
{

}

Image::Image(QString path)
{
    QImage img(path);
    this->height = img.height();
    this->width = img.width();
    QVector<QVector<QColor>> pixels(width, QVector<QColor>(height, qRgb(100,100,100)));
       for(qint32 i = 0; i < width; i++){
           for(qint32 j = 0; j < height; j++){
               pixels[i][j] = img.pixel(i,j);
           }
       }
       this->MainData = pixels;
}

Image::Image(qint32 height, qint32 width, QVector<QVector<QColor> > data, QString path)
{
    this->height = height;
    this->width = width;
    this->MainData = data;
    this->path = path;
}

Image::Image(QImage img)
{
    this->height=img.height();
    this->width=img.width();
    this->MainData=setDataFromImage(img);
}

QImage Image::get_picture(QVector<QVector<QColor>> data)
{
    QImage image(width,height,QImage::Format_ARGB32);
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            QRgb argb = qRgb(data[i][j].red()%0xff,data[i][j].green()%0xff,data[i][j].blue()%0xff);
            // 0xff - 255 in 16-system
            image.setPixel(i,j,argb);
        }
    }
    return image;
}

QVector<QVector<QColor> > Image::read_file(QString path)
{
    QImage img(path);
    qint32 height = img.height();
    qint32 width = img.width();
    QVector<QVector<QColor>> pixels(width, QVector<QColor>(height, qRgb(100,100,100)));
       for(qint32 i = 0; i < width; i++){
           for(qint32 j = 0; j < height; j++){
               pixels[i][j] = img.pixel(i,j);
           }
       }
       return pixels;
}

void Image::set_height(qint32 height)
{
    this->height = height;
}

void Image::set_width(qint32 width)
{
    this->width = width;
}

void Image::set_path(QString path)
{
    this->path = path;
}

qint32 Image::get_height()
{
    return this->height;
}

qint32 Image::get_width()
{
    return this->width;
}

QVector<QVector<QColor> > Image::get_data()
{
    return this->MainData;
}

void Image::set_resultImage(QImage resultImage)
{
    this->resultImage = resultImage;
}

QImage Image::get_resultImage()
{
    return this->resultImage;
}

void Image::set_data(QVector<QVector<QColor> > Data)
{
    MainData = Data;
}

QVector<QVector<QColor> > Image::setDataFromImage(QImage img)
{
    int height = img.height();
    int width = img.width();
    QVector<QVector<QColor>> pixels(width, QVector<QColor>(height, qRgb(100,100,100)));
       for(qint32 i = 0; i < width; i++){
           for(qint32 j = 0; j < height; j++){
               pixels[i][j] = img.pixel(i,j);
           }
       }
       return pixels;
}

void Image::set_currentImage(QImage img)
{
    this->current_image=img;
}

QImage Image::get_currentImage()
{
    return this->current_image;
}
