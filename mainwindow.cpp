#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFileDialog>
#include "image.h"
#include <QMessageBox>
#include "filter.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyleSheet("QTabBar::tab:selected {\
                                       color: #fffcfc;\
                                       background-color: rgb(15,0,0);\
                                   }");
    connect(ui->btnStartProcessing,SIGNAL(clicked()),ui->label_new_picture,SLOT(setPixmap()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    Path = QFileDialog::getOpenFileName(this, "Выбрать файл", "C:/",
                                       "JPEG Image (*.jpg);; PNG Image (*.png);");

    /*QPixmap pix(Path);
    int w = ui->label_original_picture->width();
    int h = ui->label_original_picture->height();
    ui->label_original_picture->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
    Image image(Path);*/

    QImage img(Path);
    image = Image(Path);
    ui->label_original_picture->setPixmap(QPixmap::fromImage(img));
    ui->label_original_picture->resize(img.size());
    //image = Image(Path);
}


void MainWindow::on_btnStartProcessing_clicked()
{
    Filter filter(image);
    int indx = ui->comboBox_filters->currentIndex();

    switch(indx){
    case 0:{
        QImage median = filter.make_median(ui->spinBox->value(), ui->progressBar);
        //image = median;
        image.set_resultImage(median);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(median));
        ui->label_new_picture->resize(median.size());
        ui->statusbar->showMessage("Median filter applied successfully.");
    }
        break;
    case 1:{
        QImage negative = image.get_picture(filter.make_negativ(image.get_data(), ui->progressBar));
        image.set_resultImage(negative);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(negative));
        ui->label_new_picture->resize(negative.size());
        ui->statusbar->showMessage("Negative filter applied successfully.");
    }
        break;
    case 2:{
        QImage gaus = filter.make_gauss(ui->spinBox->value(), ui->progressBar);
        image.set_resultImage(gaus);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(gaus));
        ui->label_new_picture->resize(gaus.size());
        ui->statusbar->showMessage("Gaussian filter applied successfully.");
    }
        break;
    case 4:{
        QImage highFreq = filter.make_highFreqVar1( ui->progressBar);
        image.set_resultImage(highFreq);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(highFreq));
        ui->label_new_picture->resize(highFreq.size());
        ui->statusbar->showMessage("Hight-pass filter #1 applied successfully.");
    }
        break;
    case 5:{
        QImage highFreq = filter.make_highFreqVar2( ui->progressBar);
        image.set_resultImage(highFreq);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(highFreq));
        ui->label_new_picture->resize(highFreq.size());
        ui->statusbar->showMessage("Hight-pass filter #2 applied successfully.");
    }
        break;
    case 6:{
        QImage contrast = filter.make_contrast(ui->spinBox->value(),ui->progressBar);
        image.set_resultImage(contrast);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(contrast));
        ui->label_new_picture->resize(contrast.size());
        ui->statusbar->showMessage("Contrast filter applied successfully.");
    }
        break;
    case 7:{
            QImage brightness=filter.make_brightness(ui->spinBox->value(), ui->progressBar);
            image.set_resultImage(brightness);
            ui->label_new_picture->setPixmap(QPixmap::fromImage(brightness));
            ui->label_new_picture->resize(brightness.size());
            ui->statusbar->showMessage("Brightness filter applied successfully.");
    }
        break;
    case 8:{
        QImage kanals = filter.make_kanalsWork(ui->spinBox_rgb->value(),ui->spinBox->value(), ui->progressBar);  // исправить 1
        image.set_resultImage(kanals);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(kanals));
        ui->label_new_picture->resize(kanals.size());
        ui->statusbar->showMessage("Kanals filter applied successfully.");
    }
        break;
    case 9:{
        QImage lightColor = filter.make_lightCorrection(image.get_data(), ui->progressBar);
        image.set_resultImage(lightColor);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(lightColor));
        ui->label_new_picture->resize(lightColor.size());
        ui->statusbar->showMessage("Light filter applied successfully.");
    }
        break;
    case 10:{
        QImage segmentation_kMeans = filter.make_segmentation_kMeans(ui->spinBox->value(),ui->progressBar);
        image.set_resultImage(segmentation_kMeans);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(segmentation_kMeans));
        ui->label_new_picture->resize(segmentation_kMeans.size());
        ui->statusbar->showMessage("Segmentation kMeans filter applied successfully.");
    }
        break;
    case 11:{
        QImage binarization_gradient = filter.make_binarization_gradient_method(ui->progressBar);
        image.set_resultImage(binarization_gradient);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(binarization_gradient));
        ui->label_new_picture->resize(binarization_gradient.size());
        ui->statusbar->showMessage("Binarization gradient filter applied successfully.");
    }
        break;
    case 12:{
        QImage simple_binarization = filter.make_simpleBinarization(255,ui->progressBar);
        image.set_resultImage(simple_binarization);
        ui->label_new_picture->setPixmap(QPixmap::fromImage(simple_binarization));
        ui->label_new_picture->resize(simple_binarization.size());
        ui->statusbar->showMessage("Simple binarization filter applied successfully.");
    }
        break;
    default:
         QMessageBox::information(this,"Title","Sorry");
        break;
    }
}


void MainWindow::on_actionSave_triggered()
{
    QImage result;
//    if(image.get_resultImage().isNull()){
//        result = image.get_currentImage();
//    }
//    else{
        result = image.get_resultImage();
    //}
    QString fileName = QFileDialog::getSaveFileName(this, "Save as","New image","All Files (*.*);; JPEG Image (*.jpg);; PNG Image (*.png);" );
    if  (fileName.isEmpty()){
        return;
    }
    result.save(fileName);
}


void MainWindow::on_actionLight_triggered()
{
    ui->label_original_picture->setStyleSheet("background-color: rgb(214,211,201)");
    ui->menubar->setStyleSheet("background-color: rgb(214,211,201);color: rgb(0,0,0) ");
    ui->centralwidget->setStyleSheet("background-color: rgb(214,211,201)");
    ui->label_rgb->setStyleSheet("Color: rgb(0,0,0)");
    ui->labelDoWithNewImage->setStyleSheet("Color: rgb(0,0,0)");
    ui->label_advancedSettings->setStyleSheet("Color: rgb(0,0,0)");
    ui->label_selectFilter->setStyleSheet("Color: rgb(0,0,0)");
    ui->buttonChangeImages->setStyleSheet("background-color: rgb(18,190,199); color: rgb(0,0,0)");
    ui->comboBox_filters->setStyleSheet("Color: rgb(0,0,0)");
    ui->spinBox->setStyleSheet("Color: rgb(0,0,0)");
    ui->spinBox_rgb->setStyleSheet("color: rgb(0,0,0)");
    ui->textAboutFilter->setStyleSheet("color: rgb(0,0,0)");
}


void MainWindow::on_buttonChangeImages_clicked()
{
    if(image.get_resultImage().isNull()){
        QMessageBox::information(this,"Error","Before changing, you need to process the photo.");
    }
    else{
        QImage currentImage = image.get_resultImage();
        image = currentImage;
        image.set_resultImage(currentImage);
        ui->label_original_picture->setPixmap(QPixmap::fromImage(currentImage));
        ui->label_original_picture->resize(currentImage.size());
        ui->statusbar->showMessage("The change was successful.");
    }
}


void MainWindow::on_actionDark_triggered()
{
    ui->label_original_picture->setStyleSheet("background-color: rgb(80,80,80)");
    ui->menubar->setStyleSheet("background-color: rgb(80,80,80); color: rgb(255,255,255)");
    ui->centralwidget->setStyleSheet("background-color: rgb(80,80,80)");
    ui->label_rgb->setStyleSheet("Color: rgb(255,255,255)");
    ui->labelDoWithNewImage->setStyleSheet("Color: rgb(255,255,255)");
    ui->label_advancedSettings->setStyleSheet("Color: rgb(255,255,255)");
    ui->label_selectFilter->setStyleSheet("Color: rgb(255,255,255)");
    ui->spinBox->setStyleSheet("Color: rgb(255,255,255)");
    ui->comboBox_filters->setStyleSheet("Color: rgb(255,255,255)");
    ui->buttonChangeImages->setStyleSheet("background-color: rgb(53,176,94); color: rgb(255,255,255)");
    ui->spinBox_rgb->setStyleSheet("color: rgb(255,255,255)");
    ui->textAboutFilter->setStyleSheet("color: rgb(255,255,255)");
}


void MainWindow::on_comboBox_filters_currentIndexChanged(int index)
{
    switch (index) {
        case 0:
        ui->textAboutFilter->setText("\n\nВы выбрали МЕДИАННЫЙ фильтер!\n"
                                     "\nВ качестве дополнительных параметров необходимо отправить радиус обработки"
                                     " - параметр, отвечающий за степень размытия данного фильтра. (Рек. 1)");
        break;
    case 1:
        ui->textAboutFilter->setText("\n\nВы выбрали фильтр НЕГАТИВ!\n"
                                     "\nОн не требует дополнительных параметров. Фильтр инвертирует значение пикселя");
        break;
    case 2:
        ui->textAboutFilter->setText("\n\nВы выбрали фильтр НИЖНИХ ЧАСТОТ!\n"
                                     "\nФильтр сглаживает изображение. Требует дополнительный параметр - радиус"
                                     "обработки. (Рек. 1)");
        break;
    case 3:
        ui->textAboutFilter->setText("\n\nФильтр находится в отпуске!");
        break;
    case 4:
        ui->textAboutFilter->setText("\n\nВы выбрали фильтр ВЫСОКИХ ЧАСТОТ! (Реализация 1)\n\n"
                                     "Фильтр используется для выявления границ сегментов изображения. Дополнительные "
                                     "параметры не требуются.");
        break;
    case 5:
        ui->textAboutFilter->setText("\n\nВы выбрали фильтр ВЫСОКИХ ЧАСТОТ! (Реализация 2)\n\n"
                                     "Фильтр используется для выявления границ сегментов изображения. Дополнительные "
                                     "параметры не требуются.");
        break;
    case 6:
        ui->textAboutFilter->setText("\n\nВы выбрали СРЕДСТВО ИЗМЕНЕНИЯ КОНТРАСТНОСТИ!\n\n"
                                     "Входной параметр - число, отвечающее за степень изменения яркости пикселя. (Рек. 0-2)");
        break;
    case 7:
        ui->textAboutFilter->setText("\n\nВы выбрали СРЕДСТВО ИЗМЕНЕНИЯ ЯРКОСТИ!\n\n"
                                     "Входной параметр - число, отвечающее за изменения яркости пикселя. (Рек. (-255,255))");
        break;
    case 8:
        ui->textAboutFilter->setText("\n\nВы выбрали СРЕДСТВО ДЛЯ РАБОТЫ С ПОТОКАМИ!\n\n"
                                     "Входные параметры:\n"
                                     "Номер потока (по умолчанию: 1 - красный, "
                                     "2 - зеленый, 3 - синий)\n"
                                     "Второй параметр - значение изменения характеристик выбранного канала.(0-255)");
        break;
    case 9:
        ui->textAboutFilter->setText("\n\nВы выбрали СРЕДСТВО ДЛЯ АВТОМАТИЧЕСКОЙ ЦВЕТОКОРРЕКЦИИ!\n\n"
                                     "Используется концепция Gray World. ");
        break;
    case 10:
        ui->textAboutFilter->setText("\n\nВы выбрали АДАПТИВНОЕ СРЕДСТВО ДЛЯ СЕГМЕНТАЦИИ ИЗОБРАЖЕНИЙ!\n\n"
                                     "Входной параметр - количество фрагментов изображения объединенных по принципу"
                                     "близости цвета. (При большом значении параметра программа может работать некоррекктно.)");
        break;
    case 11:
        ui->textAboutFilter->setText("\n\nВы выбрали СРЕДСТВО ДЛЯ АВТОМАТИЧЕСКОЙ КОНВЕРТАЦИИ В ЧБ МЕТОДОМ БИНАРИЗАЦИИ!\n\n"
                                     "Входных параметров нет");
        break;
    case 12:
        ui->textAboutFilter->setText("\n\nВы выбрали СРЕДСТВО ДЛЯ КОНВЕРТАЦИИ В ЧБ МЕТОДОМ БИНАРИЗАЦИИ!\n\n"
                                     "Входной параметр - порог бинаризации (ограничитель бинаризации изображения по уровню яркости). (Рек. 0-765)");
        break;
    }
}
