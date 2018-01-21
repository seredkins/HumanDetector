#include "aqimage.h"
#include <QImageReader>
#include <QImage>
#include <math.h>

// Initializators
AQImage::AQImage(){
    QImage _buff(1, 1, this->format());
    AQImage buff(_buff);
    *this = buff;
}

AQImage::AQImage(QString path, int pos_x, int pos_y, int width, int height){

    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();

    AQImage _image(width, height, image.format());
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j)
            _image.setPixel(i, j, image.pixel(width + pos_x, height + pos_y));
    *this = _image;
}

AQImage::AQImage(QString path){
    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();

    //copying newImage to image with RGB format (newImage cold be grayscale)
    QImage _new_image(image.width(), image.height(), QImage::Format_RGB32);
    for (int i = 0; i < _new_image.width(); ++i)
        for (int j = 0; j < _new_image.height(); ++j)
            _new_image.setPixel(i, j, image.pixel(i, j));

    AQImage new_image(_new_image);
    new_image.isInitializedSobels = false;
    *this = new_image;
}

// Setter
void AQImage::setImage(QString path){

    QImageReader reader(path);
    reader.setAutoTransform(true);
    QImage image = reader.read();

    //copying newImage to image with RGB format (newImage cold be grayscale)
    QImage _new_image(image.width(), image.height(), QImage::Format_RGB32);
    for (int i = 0; i < _new_image.width(); ++i)
        for (int j = 0; j < _new_image.height(); ++j)
            _new_image.setPixel(i, j, image.pixel(i, j));

    AQImage new_image(_new_image);
    *this = new_image;

}

// Functions for working with extended image
AQImage AQImage::initBuffImg(const int &box_size){
    shift = box_size / 2;
    QImage _buffer_image(this->width() + 2*shift, this->height() + 2*shift, this->format());

    for (int i = 0; i < _buffer_image.width(); ++i)
        for (int j = 0; j < _buffer_image.height(); ++j){
            if (!(i < shift || j < shift || i > _buffer_image.width() - 1 - shift || j > _buffer_image.height() - 1 - shift)) // if not edges
                _buffer_image.setPixel(i, j, this->pixel(i - shift, j - shift)); //copying image pixels
            else
                _buffer_image.setPixel(i, j, this->pixel(abs(i - pow((-1),(i < shift))*shift -
                                                                 pow((-1),(i < shift))*2),

                                                         abs(j - pow((-1),(j < shift))*shift -
                                                                 pow((-1),(j < shift))*2)));
        }
    return _buffer_image;
}

void AQImage::extractResult(QImage _image){
    for (int i = 0; i < _image.width(); ++i)
        for (int j = 0; j < _image.height(); ++j)
            this->setPixel(i, j, _image.pixel(i + shift, j + shift));
}

// Functions for calculating Histogram of Oriented Gradients
void AQImage::initSobelImages(){
//    this->save("C:/SHP/CV/MLv1.1.1/ML/Images/Results");
    std::vector<std::vector<long double>>sobel_x;
    std::vector<std::vector<long double>>sobel_y;
    shift = 1;

    sobel_x.resize(3);
    sobel_y.resize(3);
    sobel_img_x.resize(buffer_image.height());
    sobel_img_y.resize(buffer_image.height());

    sobel_y[0].push_back(-1); sobel_y[0].push_back(-2); sobel_y[0].push_back(-1);
    sobel_y[1].push_back(0);  sobel_y[1].push_back(0);  sobel_y[1].push_back(0);
    sobel_y[2].push_back(1);  sobel_y[2].push_back(2);  sobel_y[2].push_back(1);

    sobel_x[0].push_back(-1);  sobel_x[0].push_back(0);  sobel_x[0].push_back(1);
    sobel_x[1].push_back(-2);  sobel_x[1].push_back(0);  sobel_x[1].push_back(2);
    sobel_x[2].push_back(-1);  sobel_x[2].push_back(0);  sobel_x[2].push_back(1);

    for (int i = shift; i < buffer_image.width() - shift; ++i)
        for (int j = shift; j < buffer_image.height() - shift; ++j){
            double gray_x = 0;
            double gray_y = 0;
            for (int m = 0; m < 3; ++m)
                for (int k = 0; k < 3; ++k){
                    gray_x += qGray(buffer_image.pixel(i + m - shift, j + k - shift))*sobel_x[m][k];
                    gray_y += qGray(buffer_image.pixel(i + m - shift, j + k - shift))*sobel_y[m][k];
                }

            sobel_img_x[j].push_back(gray_x);
            sobel_img_y[j].push_back(gray_y);
        }
    // If Sobel values are already calculated for image we don't need to do it again
    this->isInitializedSobels = true;
}

std::vector<double> AQImage::HOG(int pos_x){
    std::pair<AQImage, AQImage>f;
    std::vector<double>sings; //all sings of a picture

    // If Sobel values are not calculated
    if (!(this->isInitializedSobels)){
        buffer_image = initBuffImg(3);
        initSobelImages();
    }

    for (int i = pos_x; i < pos_x + 80; i += 8)
        for (int j = 1; j < 200 - 1; j += 8){
            std::vector<double>gist; //gistoghram in every box 8x8
            gist.resize(8); //contains 8 values
            for (int m = i; m < i + 8; ++m)
                for (int k = j; k < j + 8; ++k){
                    double angle = atan2(sobel_img_y[k][m], sobel_img_x[k][m]) + M_PI;

                    //finding trig. segment where angle is
                    //only one of boolean variables turns to true (1)
                    //the others turns to false (0)

                    bool b0 = ((angle >= 0) && (angle <= M_PI / 4));
                    bool b1 = ((angle > M_PI / 4) && (angle <= M_PI / 2));
                    bool b2 = ((angle > M_PI / 2) && (angle <= 3*M_PI / 4));
                    bool b3 = ((angle > 3*M_PI / 4) && (angle <= M_PI));
                    bool b4 = ((angle > M_PI) && (angle <= 5*M_PI / 4));
                    bool b5 = ((angle > 5*M_PI / 4) && (angle <= 3*M_PI / 2));
                    bool b6 = ((angle > 3*M_PI / 2) && (angle <= 7*M_PI / 4));
                    bool b7 = ((angle > 7*M_PI / 4) && (angle < 2*M_PI));

                    //when multiply boolean variables by positions all of them turns to 0 except one

                    int pos = b0*0 + b1*1 + b2*2 + b3*3 + b4*4 + b5*5 + b6*6 +b7*7;
                    gist[pos]++;
                }
            for (auto it : gist) sings.push_back(it);
        }
    return sings;
}

// Draws border with size at the pixel
void AQImage::drawBorder(int itx, int ity, int wi, int he){
    for (int a = itx; a < wi + itx; ++a){
        this->setPixel(a, ity, qRgb(255, 0, 0));
        this->setPixel(a, ity + he, qRgb(255, 0, 0));
    }

    for (int a = ity; a < he + ity; ++a){
        this->setPixel(itx, a, qRgb(255, 0, 0));
        this->setPixel(itx + wi, a, qRgb(255, 0, 0));
    }
}
