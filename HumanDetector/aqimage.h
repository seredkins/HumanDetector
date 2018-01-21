#ifndef AQIMAGE_H
#define AQIMAGE_H

#include <QImage>
#include <vector>

class AQImage : public QImage{
private:
    int shift;

    static const int square_side = 7;

    std::vector<std::vector<int>> sobel_img_x;
    std::vector<std::vector<int>> sobel_img_y;

    QImage buffer_image;
public:
    // Initializetors
    AQImage();
    AQImage(QImage image) : QImage(image){}
    AQImage(QString, int, int, int, int);
    AQImage(int height, int width, QImage::Format format) : QImage(height, width, format){}
    AQImage(QString);

    // Setter
    void setImage(QString);

    // Functions for working with extended image
    AQImage initBuffImg(const int&);
    void extractResult(QImage);

    // Draws border with size at the pixel
    void drawBorder(int, int, int, int);

    // Functions for calculating Histogram of Oriented Gradients
    bool isInitializedSobels;      // If Sobel values are already calculated for image we don't need to do it again
    void initSobelImages();        // Calculates Soble values for all the image
    std::vector<double> HOG(int);  // Returns Vector of HOG's
};


#endif // AQIMAGE_H
