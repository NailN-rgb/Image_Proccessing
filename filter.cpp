#include "filter.h"
#include "iostream"

Filter::Filter(Image &im)
{
    this->image = im;
}

QVector<QVector<QColor> > Filter::make_negativ(QVector<QVector<QColor>> data, QProgressBar *pb)
{
    QVector<QVector<QColor>> tempData(image.get_width(),QVector<QColor>(image.get_height()));
    for (int i = 0; i < image.get_width(); i++){
        for (int j = 0; j < image.get_height(); j++){
            int red = 255 - (data[i][j].red() % 0xff);
            int green = 255 - (data[i][j].green() % 0xff);
            int blue = 255 - (data[i][j].blue() % 0xff);
            QColor color(red, green, blue, 255);
            tempData[i][j] = color;
        }
    }
    return tempData;
}

QRgb Filter::get_gaussValue(QVector<QVector<QColor>> &pixels)
{
    qint32 width = pixels.size();
    qint32 height = pixels.at(0).size();
    qint32 pixel_size = width * height;
    qint32 fullRed = 0;
    qint32 fullGreen = 0;
    qint32 fullBlue = 0;
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            fullRed += pixels[i][j].red();
            fullGreen += pixels[i][j].green();
            fullBlue += pixels[i][j].blue();
        }
    }

    qint32 redComp = (fullRed/pixel_size) % 255;
    qint32 greenComp = (fullGreen/pixel_size) % 255;
    qint32 blueComp = (fullBlue/pixel_size) % 255;

    QRgb color = qRgb(redComp, greenComp, blueComp);
    return color;
}

QVector<QVector<QColor>> Filter::get_fromMatrix(QVector<QVector<QColor>> &data, int height_indx, int width_indx, int points_count)
{
    QVector<QVector<QColor>> result(points_count, QVector<QColor>(points_count));
    for (int i = 0; i < points_count; i++){
        for (int j = 0; j < points_count; j++){
            try {
                result[i][j] = data[width_indx + i][height_indx + j];
            }  catch (...) {
                std::cout<<"idiot";
            }
        }
    }
    return result;
}

QImage Filter::make_gauss(int pixel_radius, QProgressBar *pb)
{
        //auto imData = image.get_data();
        QVector<QVector<QColor>> Data = pre_filtration_data_changing(pixel_radius);
        int dataWidth = Data.size();
        int dataHeight = Data.at(0).size();
        QImage result(dataWidth - 2 * pixel_radius, dataHeight - 2 * pixel_radius, QImage::Format_ARGB32);

        double Value = 100.0/(dataHeight*dataWidth);
        double count = 0;

        for(int i = pixel_radius; i < dataHeight - pixel_radius; i++){ //image
            for(int j = pixel_radius; j < dataWidth - pixel_radius; j++){
               auto value = get_fromMatrix(Data,i, j ,pixel_radius);
               QRgb rgb = get_gaussValue(value);
               result.setPixel(j - pixel_radius,i - pixel_radius, rgb);
               count += Value;
               pb->setValue((int)count);
            }
        }
        pb->setValue(100);
        return result;
}

QImage Filter::make_median(int pixel_radius, QProgressBar *pb)
{
    //auto imData = image.get_data();
    QVector<QVector<QColor>> data = pre_filtration_data_changing(pixel_radius);
    int data_width = data.size();
    int data_height = data.at(0).size();

    double Value = 100.0/(data_height * data_height);
    double count = 0;

    QImage result(data_width - 2 * pixel_radius, data_height - 2 * pixel_radius, QImage::Format_ARGB32);
    //QVector<QVector<QColor>> temp = data;
    for (int i = pixel_radius; i < data_height - pixel_radius; i++){
        for (int j = pixel_radius; j < data_width - pixel_radius; j++){
            auto medValue = get_fromMatrix(data, i,j,pixel_radius);
            QRgb rgb = median_value(medValue);
            result.setPixel(j-pixel_radius, i-pixel_radius, rgb);
            count += Value;
            pb->setValue((int)count);
        }
    }
    pb->setValue(100);
    return result;
}

QImage Filter::make_highFreqVar1(QProgressBar *pb)
{
    int pixelRadius = 3;
        QVector<QVector<QColor>> Data = pre_filtration_data_changing(pixelRadius);
        int dataWidth = Data.size();
        int dataHeight = Data.at(0).size();

        double Value = 100.0/(dataHeight * dataWidth);
        double count = 0;

        QImage result(dataWidth - 2 * pixelRadius, dataHeight - 2 * pixelRadius, QImage::Format_ARGB32);

        for(int i = pixelRadius; i < dataHeight - pixelRadius; ++i){ //image
            for(int j = pixelRadius; j < dataWidth - pixelRadius; ++j){
                auto value = get_fromMatrix(Data, i, j, pixelRadius);
               QRgb rgb = high_freq_filter(value);
               result.setPixel(j - pixelRadius,i - pixelRadius, rgb);
               count += Value;
               pb->setValue((int)count);
            }
        }
        pb->setValue(100);
        return result;
}

QImage Filter::make_highFreqVar2(QProgressBar *pb)
{
        int pixelRadius = 3;
        QVector<QVector<QColor>> Data = pre_filtration_data_changing(pixelRadius);
        int dataWidth = Data.size();
        int dataHeight = Data.at(0).size();

        double Value = 100.0/(dataHeight * dataWidth);
        double count = 0;

        QImage result(dataWidth - 2 * pixelRadius, dataHeight - 2 * pixelRadius, QImage::Format_ARGB32);

        for(int i = pixelRadius; i < dataHeight - pixelRadius; ++i){ //image
            for(int j = pixelRadius; j < dataWidth - pixelRadius; ++j){
                auto value = get_fromMatrix(Data, i, j, pixelRadius);
               QRgb rgb = high_freq_filter2(value);
               result.setPixel(j - pixelRadius,i - pixelRadius, rgb);
               count += Value;
               pb->setValue((int)count);
            }
        }
        pb->setValue(100);
        return result;
}

QImage Filter::make_contrast(double contrValue, QProgressBar *pb)
{
    //Пусть contrValue от 0 до 2
        auto Data = image.get_data();
        int height = Data.at(0).size();
        int width = Data.size();
        QImage result(width, height, QImage::Format_ARGB32);

        double value = 100.0/(height*width);
        double count = 0;

        for(int i = 0; i < width; ++i){
            for(int j = 0; j < height; ++j){

                 int red = Data[i][j].red() * contrValue;
                 int green = Data[i][j].green() * contrValue;
                 int blue = Data[i][j].blue() * contrValue;

                 if(red > 255){
                     red = 255;
                 }

                 if(green > 255){
                     green = 255;
                 }

                 if(blue > 255){
                     blue = 255;
                 }

                 result.setPixel(i,j, qRgb(red, green, blue));
                 count+= value;
                 pb->setValue((int)count);
            }
        }
        pb->setValue(100);
        return result;
}

QImage Filter::make_brightness(double brightValue, QProgressBar *pb)
{
    //Пусть brightValue от -255 до 255
        auto Data = image.get_data();
        int height = Data.at(0).size();
        int width = Data.size();

        QImage result(width, height, QImage::Format_ARGB32);

        double Value = 100.0/(height * width);
        double count = 0;
        for(int i = 0; i < width; ++i){
            for(int j = 0; j < height; ++j){
                 int red = Data[i][j].red() + brightValue;
                 int green = Data[i][j].green() + brightValue;
                 int blue = Data[i][j].blue() + brightValue;

                 if(red > 255){
                     red = 255;
                 }

                 if(green > 255){
                     green = 255;
                 }

                 if(blue > 255){
                     blue = 255;
                 }

                 if(red < 0){
                     red = 0;
                 }

                 if(green < 0){
                     green = 0;
                 }

                 if(blue < 0){
                     blue = 0;
                 }

                 result.setPixel(i,j, qRgb(red, green, blue));
                 count+= Value;
                 pb->setValue((int)count);
            }
        }
        pb->setValue(100);
        return result;
}

QImage Filter::make_kanalsWork(int cnl, int numValue, QProgressBar *pb)
{
    //данный метод позволяет обратится к каждому каналу картинки и изменять его
        //пусть 1 - красный канал, 2 - зеленый, 3 - синий
        auto Data=image.get_data();
        int height = Data.at(0).size();
        int width = Data.size();

        QImage result(width, height, QImage::Format_ARGB32);

        double Value = 100.0/(height * width);
        double count = 0;

        if(cnl == 1){
            for(int i = 0; i < width; ++i){
                for(int j = 0; j < height; ++j){
                     int red = Data[i][j].red() + numValue;
                     if(red > 255){
                         red = 255;
                     }
                     if(red < 0){
                         red = 0;
                     }
                     count+= Value;
                     pb->setValue((int)count);
                     result.setPixel(i,j, qRgb(red, Data[i][j].green(), Data[i][j].blue()));
                }
            }

        }
        else if(cnl == 2){
            for(int i = 0; i < width; ++i){
                for(int j = 0; j < height; ++j){
                     int green = Data[i][j].green() + numValue;
                     if(green > 255){
                         green = 255;
                     }
                     if(green < 0){
                         green = 0;
                     }

                     count+= Value;
                     pb->setValue((int)count);
                     result.setPixel(i,j, qRgb(Data[i][j].red(), green, Data[i][j].blue()));
                }
            }
        }
        else if(cnl == 3){
            for(int i = 0; i < width; ++i){
                for(int j = 0; j < height; ++j){
                     int blue = Data[i][j].blue() + numValue;
                     if(blue > 255){
                         blue = 255;
                     }
                     if(blue < 0){
                         blue = 0;
                     }

                     count+= Value;
                     pb->setValue((int)count);
                     result.setPixel(i,j, qRgb(Data[i][j].red(), Data[i][j].green(), blue));
                }
            }
        }
        else{
            result = image.get_picture(Data);
        }
        pb->setValue(100);
        return result;
}

QImage Filter::make_lightCorrection(QVector<QVector<QColor> > data, QProgressBar *pb)
{
    int height = data.at(0).size();
    int width = data.size();

    QImage result(width,height,QImage::Format_ARGB32);
    QVector<int> averages = average_colors(data);
    //conception of GreyWorrld

    double Value = 100.0/(height * width);
    double count = 0;

       int Avg = (averages[0] + averages[1] + averages[2]) / 3;
       double redQuef = (double)averages[0] / Avg;
       double greenQuef = (double)averages[1]/ Avg;
       double blueQuef = (double)averages[2]/ Avg;

       for(int i = 0; i < width; ++i){
           for(int j = 0; j < height; ++j){
                QRgb clr = qRgb((data[i][j].red() / redQuef),
                                (data[i][j].green() / greenQuef),
                                (data[i][j].blue() / blueQuef));
                result.setPixel(i,j, clr);
                count+= Value;
                pb->setValue((int)count);
           }
       }
       pb->setValue(100);
       return result;
}

QImage Filter::make_segmentation_kMeans(int clustersCount, QProgressBar *pb)
{
    auto data = image.get_data();
    int height = data.at(0).size();
    int width = data.size();

        QImage result(width, height, QImage::Format_ARGB32);
        double Value = 100.0/(height * width);
        double count = 0;
        QVector<QVector<QColor>> k_Means = kMeans(data,clustersCount);

        for(int i = 0; i < width; ++i){
            for(int j = 0; j < height; ++j){
                QRgb argb = qRgb(k_Means[i][j].red() % 0xff, k_Means[i][j].green() % 0xff, k_Means[i][j].blue() % 0xff);
                result.setPixel(i,j, argb);
                count+= Value;
                pb->setValue((int)count);
            }
        }
        pb->setValue(100);
        return result;
}

QImage Filter::make_binarization_gradient_method(QProgressBar *pb)
{
    auto Data = image.get_data();
    int height = Data.at(0).size();
    int width = Data.size();

        QImage result(width, height, QImage::Format_ARGB32);
        double Value = 100.0/(height * width);
        double count = 0;
        QVector<QVector<QColor>> Temp = pre_filtration_data_changing(1);
        QVector<QVector<int>> G(width, QVector<int>(height));

        for(int i = 0; i < width; ++i){
            for(int j = 0; j < height; ++j){
                int Gi = get_bright_value(Temp[i+2][j]) - get_bright_value(Temp[i][j]);
                int Gj = get_bright_value(Temp[i][j + 2]) - get_bright_value(Temp[i][j]);
                G[i][j] = get_maxRes(Gi, Gj);
                count+= Value;
                pb->setValue((int)count);
            }
        }

        long long Gsum = 0;
        long long tempSumm = 0;

        for(int i = 0; i < width; ++i){
            for(int j = 0; j < height; ++j){
                tempSumm += G[i][j] * get_bright_value(Temp[i + 1][j + 1]);
                Gsum += G[i][j];
            }
        }

        int binarizationValue = tempSumm / Gsum;
        pb->setValue(100);
        return make_simpleBinarization(binarizationValue, pb);
}

QImage Filter::make_simpleBinarization(int binarization_value, QProgressBar *pb)
{
    auto Data = image.get_data();
    int height = Data.at(0).size();
    int width = Data.size();
    QImage result(width,height,QImage::Format_ARGB32);
    QRgb white = qRgb(255,255,255);
    QRgb black = qRgb(0,0,0);
    double Value = 100.0/(height * width);
    double count = 0;

       for(int i = 0; i < width; ++i){
           for(int j = 0; j < height; ++j){
               int res = Data[i][j].red() + Data[i][j].green() + Data[i][j].blue();
               count+= Value;
               pb->setValue((int)count);

               if(res >= binarization_value){
                   result.setPixel(i,j,white);
                   count+= Value;
                   pb->setValue((int)count);
               }
               else if(res < binarization_value){
                   result.setPixel(i,j,black);
                   count+= Value;
                   pb->setValue((int)count);
               }
           }
       }
       pb->setValue(100);
       return result;
}

QVector<QVector<QColor> > Filter::pre_filtration_data_changing(int pixel_radius)
{
    auto data = image.get_data();
    int dataWidth = data.size();
    int dataHeight = data.at(0).size();
    QVector<QVector<QColor>> Temp(dataWidth + 2 * pixel_radius, QVector<QColor>(dataHeight + 2 * pixel_radius));

      for(int i = 0; i < dataWidth; ++i){ //отражение изображения на вертикальные области
          for(int j = 0; j < pixel_radius; ++j){
          Temp[pixel_radius + i][j] = data[i][j];
          Temp[pixel_radius + i][dataHeight + pixel_radius + j] = data[i][dataHeight - pixel_radius + j];

          }
      }

      for(int i = 0; i < dataHeight; ++i){
          for(int j = 0; j < pixel_radius; ++j){//отражение изображения на горизонтальные  области
          Temp[j][pixel_radius + i] = data[j][i];
          Temp[dataWidth + pixel_radius + j][pixel_radius + i] = data[dataWidth - pixel_radius + j][i]; //array [width][height]
          }
      }

      for(int i = 0; i < dataWidth; ++i){ //копирование центральной части изображения
          for(int j = 0; j < dataHeight; ++j){
              Temp[pixel_radius + i][pixel_radius + j] = data[i][j];
          }
      }

      for(int i = 0; i < pixel_radius; ++i){ //заполнение углов
          for(int j = 0; j < pixel_radius; ++j){
              Temp[i][j] = data[i][j]; //if radius <= dataheight && datawidth
              Temp[dataWidth + pixel_radius + i - 1][j] = data[(dataWidth - pixel_radius + i - 1)][j];
              Temp[i][dataHeight + pixel_radius + j - 1] = data[i][dataHeight - pixel_radius + j - 1];
              Temp[dataWidth + pixel_radius + i - 1][dataHeight + pixel_radius + j - 1] = data[(dataWidth - pixel_radius + i - 1)][(dataHeight - pixel_radius + j - 1)];
          }
      }

      return Temp;
}

void Filter::sort(QVector<int> & sort_vector)
{
    for(int i = 0; i < sort_vector.size(); i++)
       {
           for (int j = sort_vector.size() - 1; j > i; j--)
           {
               if (sort_vector[j - 1] > sort_vector[j]) {
                   double x = sort_vector[j - 1];
                   sort_vector[j - 1] = sort_vector[j];
                   sort_vector[j] = x;
               }
           }
    }
}

int Filter::median(QVector<int> &vector)
{
    if (vector.size() % 2 != 0)
       {
           return vector[(vector.size() - 1) / 2];
       }
       else
       {
           return (vector[(vector.size()/2)-1] + vector[vector.size() / 2]) / 2;
       }
}

QRgb Filter::median_value(QVector<QVector<QColor>> &pixels)
{
    int width = pixels.size();
    int height = pixels.at(0).size();
    QVector<int> fullRedVector, fullGreenVector, fullBlueVector;
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            fullRedVector.push_back(pixels[i][j].red());
            fullGreenVector.push_back(pixels[i][j].green());
            fullBlueVector.push_back(pixels[i][j].blue());
        }
    }
    sort(fullRedVector);
    sort(fullGreenVector);
    sort(fullBlueVector);

    int redComp = median(fullRedVector);
    int greenComp = median(fullGreenVector);
    int blueComp = median(fullBlueVector);

    QRgb color = qRgb(redComp,greenComp,blueComp);
    return color;
}

QRgb Filter::high_freq_filter(QVector<QVector<QColor> > &data)
{
    int pixelRadius = 3;
    int filter[3][3] = {{0, -1, 0},{-1, 4, -1},{0, -1, 0}};
    int fullred = 0;
    int fullgreen = 0;
    int fullblue = 0;
    for(int i = 0; i < pixelRadius; ++i){
            for(int j = 0; j < pixelRadius; ++j){
                if(filter[i][j] != 0){
                fullred += data[i][j].red() * filter[i][j];
                fullgreen += data[i][j].green() * filter[i][j];
                fullblue += data[i][j].blue() * filter[i][j];
            }
            }
        }
    if(fullred < 0)
          fullred = 255 + fullred;

      if(fullgreen < 0)
          fullgreen = 255 + fullgreen;

      if(fullblue < 0)
          fullblue = 255 + fullblue;

      return qRgb(fullred, fullgreen, fullblue);
}

QRgb Filter::high_freq_filter2(QVector<QVector<QColor> > &data)
{
    int pixelRadius = 3;
    int filter[3][3] = {{0, -1, 0}, {-1, 4 , -1 }, {0, -1, 0}};

    int fullred = 0;
    int fullgreen = 0;
    int fullblue = 0;

        for(int i = 0; i < pixelRadius; ++i){
            for(int j = 0; j < pixelRadius; ++j){
                if(filter[i][j] != 0){
                fullred += data[i][j].red() * filter[i][j];
                fullgreen += data[i][j].green() * filter[i][j];
                fullblue += data[i][j].blue() * filter[i][j];
            }
            }
        }

        if(fullred < 0)
            fullred = fabs(fullred);

        if(fullgreen < 0)
            fullgreen = fabs(fullgreen);

        if(fullblue < 0)
            fullblue = fabs(fullblue);

        return qRgb(fullred, fullgreen, fullblue);
}

QVector<int> Filter::average_colors(QVector<QVector<QColor> > data)
{
    int fullred = 0, fullgreen = 0, fullblue= 0;
    int height = data.at(0).size();
    int width = data.size();

        for(int i = 0; i < width; ++i){
            for(int j = 0; j < height; ++j){
                 fullred += data[i][j].red();
                 fullgreen += data[i][j].green();
                 fullblue += data[i][j].blue();
            }
        }

        QVector<int> result;
        result.push_back(fullred/(height * width));
        result.push_back(fullgreen/(height * width));
        result.push_back(fullblue/(height * width));
        return result;
}

int Filter::get_bright_value(QColor clr)
{
    return clr.red()+clr.green()+clr.blue();
}

double Filter::get_two_points(QColor col_1, QColor col_2)
{
    return sqrt(pow((col_1.red() - col_2.red()),2) + pow((col_1.green() - col_2.green()), 2) + pow((col_1.blue() - col_2.blue()), 2));
}

double Filter::compute_lenght(double a, double b)
{
    return (a+b)/2;
}

QVector<QVector<QColor> > Filter::kMeans(QVector<QVector<QColor> > data, int clustersCount)
{
    int height = data.at(0).size();
        int width = data.size();
        QVector<QVector<QColor>> result(width, QVector<QColor>(height));

        int pixelNum = width * height;

        QVector<QColor> Centrals(clustersCount);

        //centers identify
        srand((unsigned)time(NULL));

        for(int i = 0; i < clustersCount; i++){
            QColor clr(data[0 + rand() % width][0 + rand() % height]);
            Centrals[i] = clr;
        }


        QVector<int> thisIter(pixelNum, -1);
        QVector<int> lastIter(pixelNum, 0);

        int maxIterations = 20; //check
        int iter = 0;

        while(true){
            for(int i = 0; i < width; ++i){
                for(int j = 0; j < height; ++j){

                    double* mas = new double[clustersCount];
                    //QVector<double> mas(clustersNum);
                    for(int k = 0; k < clustersCount; k++){
                        *(mas + k) = get_two_points(data[i][j], Centrals[k]);
                    }

                    double minDistance = *mas;
                    int m = 0;

                    for(int k = 0; k < clustersCount; k++){
                        if(minDistance > *(mas + k)){
                            minDistance = *(mas + k);
                            m = k;
                        }
                    }

                    Centrals[m].setRed(compute_lenght(data[i][j].red(), Centrals[m].red()));
                    Centrals[m].setGreen(compute_lenght(data[i][j].green(), Centrals[m].green()));
                    Centrals[m].setBlue(compute_lenght(data[i][j].blue(), Centrals[m].blue()));

                }
            }

            int *mass = new int[pixelNum];
            for(int i = 0; i < width; ++i){
                for(int j = 0; j < height; ++j){
                    double *mas = new double[clustersCount];

                    for(int k = 0; k < clustersCount; k++){
                        *(mas + k) = get_two_points(data[i][j], Centrals[k]);
                    }

                    double minDistance = *mas;
                    int m = 0;

                    for(int k = 0; k < clustersCount; k++){
                        if(minDistance > *(mas + k)){
                            minDistance = *(mas + k);
                            m = k;
                        }
                    }

                    int temp;
                    if(i == 0){
                        temp = j;
                    }
                    else{
                        temp = (i-1) * height + j;
                    }

                    mass[temp] = m;
                }
            }


            for(int i = 0; i < width; ++i){
                for(int j = 0; j < height; ++j){
                    int temp;
                    if(i == 0){
                        temp = j;
                    }
                    else{
                        temp = (i-1) * height + j;
                    }
                    thisIter[temp] = *(mass + temp);
                }
            }

            delete[] mass;

            iter++;

            if(thisIter == lastIter || iter > maxIterations){
                break;
            }

            lastIter = thisIter;
        }

        QVector<QVector<int>> Temp(width, QVector<int>(height));

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                int temp;
                if(i == 0){
                    temp = j;
                }
                else{
                    temp = (i-1) * height + j;
                }

                Temp[i][j] = thisIter[temp];

                result[i][j].setRed(Centrals[Temp[i][j]].red());
                result[i][j].setGreen(Centrals[Temp[i][j]].green());
                result[i][j].setBlue(Centrals[Temp[i][j]].blue());
            }
        }

        return result;
}

int Filter::get_maxRes(int a, int b)
{
    if(a >= b){
            return a;
        }
        else{
            return b;
        }
}























