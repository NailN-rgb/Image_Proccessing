#ifndef FILTER_H
#define FILTER_H
#include "image.h"
#include <QProgressBar>

class Filter
{
private:
    Image image;
    QVector<QVector<QColor>> get_fromMatrix(QVector<QVector<QColor>> &data, int height_indx, int width_indx, int points_count);
    QRgb get_gaussValue(QVector<QVector<QColor>> &pixels);
    QVector<QVector<QColor>> pre_filtration_data_changing(int pixel_radius);
    void sort(QVector<int> &sort_vector);
    int median(QVector<int> &vector);
    QRgb median_value(QVector<QVector<QColor>> &pixels);

    QRgb high_freq_filter(QVector<QVector<QColor>> &data);
    QRgb high_freq_filter2(QVector<QVector<QColor>> &data); // Пространственный фильтр верхних частот

    QVector<int> average_colors(QVector<QVector<QColor>> data);

    int get_bright_value(QColor clr);
    double get_two_points(QColor col_1, QColor col_2);
    double compute_lenght(double a, double b);
    QVector<QVector<QColor>> kMeans(QVector<QVector<QColor>> data, int clustersCount);
    int get_maxRes(int a, int b);

public:
    Filter(Image &image);
    QVector<QVector<QColor>> make_negativ(QVector<QVector<QColor>> data, QProgressBar *pb);
    QImage make_gauss(int pixel_radius, QProgressBar *pb);
    QImage make_median(int pixel_radius, QProgressBar *pb);
    QImage make_highFreqVar1(QProgressBar *pb);
    QImage make_highFreqVar2(QProgressBar *pb);
    QImage make_contrast(double contrValue, QProgressBar *pb);
    QImage make_brightness(double brightValue, QProgressBar *pb);
    QImage make_kanalsWork(int cnl, int numValue, QProgressBar *pb);
    QImage make_lightCorrection(QVector<QVector<QColor>> data, QProgressBar *pb);
    QImage make_segmentation_kMeans(int clustersCount, QProgressBar *pb);
    QImage make_binarization_gradient_method(QProgressBar *pb);
    QImage make_simpleBinarization(int binarization_value, QProgressBar *pb);
};

#endif // FILTER_H
