#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    /*Format of file
     * nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,
     *
     * where nn is a number in hexa decimal form
     * */
    file1Name = QFileDialog::getOpenFileName(this,tr("Open Data File"),"C:\\ ", tr("txt Files (*.txt)"));
    ui->lineEdit->setText(file1Name);

//    data=QString(file1->readAll());//*
    //data.append(file1->read)
//    ui->plainTextEdit->setPlainText(data);//*
    //ui->plainTextEdit->setText(data);
}

void MainWindow::on_pushButton_2_clicked()
{
    QFile *file1=new QFile(file1Name);
    file1->open(QIODevice::ReadOnly | QIODevice::Text);//*
//    data=ui->plainTextEdit->toPlainText();
    QString str,data;

    while(!file1->atEnd()){
        str=file1->readLine();
        while(!str.contains("NewFrame")){
            str=file1->readLine();
        }
        //here, str contains NewFrame hence next line has data
        qDebug()<<"Found NewFrame";
        data=file1->readLine();
        QStringList lst=data.split(",");
        QByteArray yuvarr;
        foreach(QString s,lst){
            yuvarr.append(s.toInt(0,16));
        }
        //yuvarr now has the binary YUV422 UYVY data
        //Now display it or store it

        //YUV422_to_RGB888(yuvarr);
        QFile *file2=new QFile(ui->lineEdit_2->text());
        file2->open(QIODevice::WriteOnly);
        QDataStream out(file2);
        out.setByteOrder(QDataStream::LittleEndian); // *** set little endian byte order
        foreach(quint8 dat,yuvarr){
            out<<dat;
        }
        file2->close();
    }

    file1->close();

}

void MainWindow::on_pushButton_3_clicked()
{
    QString file1Name = QFileDialog::getSaveFileName(this,tr("Save Data File"),"C:\\ ", tr("yuv Files (*.yuv)"));
    ui->lineEdit_2->setText(file1Name);
}

void MainWindow::on_actionFile_info_triggered()
{
    QMessageBox msgb;
    msgb.setText("Format of file\n nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,nn,\n\n where nn is a number in hexa decimal form\n");
    msgb.exec();
}

void MainWindow::YUV422_to_RGB888( QByteArray rawStreamingFrame ) {

    // Print data size

    qDebug() << "Print the data size from read file: " << rawStreamingFrame.length() << " bytes...";

    // Get the raw data in the bytearray

    const char *binaryData = rawStreamingFrame.data();

    // Define the H and W of our output image ( we know it's 640 x 480 )
    int width = 720;
    int height = rawStreamingFrame.length()/width;



    // Create a QByteArray to store the RGB Data

    int redContainer[height*width];

    int greenContainer[height*width];

    int blueContainer[height*width];

    // To reconstruct pixels in pairs, 4 bytes of data are required. The bytes are arranged as u, y1, v, y2. The total number of bytes in the image stream is 2 x width x height.

    qDebug() << "Looping through... " << rawStreamingFrame.length()-4;

    // Loop through 4 bytes at a time

    int cnt = -1;

    for ( int i = 0 ; i <= rawStreamingFrame.length()-4 ; i += 4 ) {

       // Extract yuv components

       int u  = (int)binaryData[i];

       int y1 = (int)binaryData[i+1];

       int v  = (int)binaryData[i+2];

       int y2 = (int)binaryData[i+3];

       // Define the RGB

       int r1 = 0 , g1 = 0 , b1 = 0;

       int r2 = 0 , g2 = 0 , b2 = 0;

       // u and v are +-0.5

       u -= 128;

       v -= 128;

       // Conversion

       r1 = y1 + 1.403*v;

       g1 = y1 - 0.344*u - 0.714*v;

       b1 = y1 + 1.770*u;

       r2 = y2 + 1.403*v;

       g2 = y2 - 0.344*u - 0.714*v;

       b2 = y2 + 1.770*u;

       // Increment by one so we can insert

       cnt+=1;

       // Append to the array holding our RGB Values

       redContainer[cnt] = r1;

       greenContainer[cnt] = g1;

       blueContainer[cnt] = b1;

       // Increment again since we have 2 pixels per uv value

       cnt+=1;

       // Store the second pixel

       redContainer[cnt] = r2;

       greenContainer[cnt] = g2;

       blueContainer[cnt] = b2;

    }

    // Define a QImage for our RGB Data

    QImage rgbImage = QImage(width, height, QImage::Format_RGB888);

    // Print constructing RGB image

    qDebug() << "Now constructing RGB Image... ";

    // Now construct our RGB image

    int pixelCounter = -1;

    for ( int i = 0; i < height; ++i ) {

        for ( int j = 0; j < width; ++j ) {

            pixelCounter+=1;

            rgbImage.setPixel( j, i, qRgb( redContainer[pixelCounter] ,  greenContainer[pixelCounter] , blueContainer[pixelCounter] ) );

        }

    }

    // Print pixel counter

    qDebug() << "Counted number of pixels: " << pixelCounter;

    // Save our Image!

    rgbImage.save("rgb_from_yuv.bmp");

}
