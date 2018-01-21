#include "mainwindow.h"
#include "model.h"
#include <QImage>
#include <QPixmap>
#include <QImageReader>
#include <QFileDialog>
#include <QTextBrowser>
#include <fstream>
#include <vector>
#include <ctime>
#include <random>

// Init and Deinit
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {
    draw_widget = new QLabel("", this);

    hlayout = new QHBoxLayout();
//    open_button = new QPushButton("Open Image", this);
//    hlayout->addWidget(open_button);

    for (int i = 0; i < BUTTONS_COUNT; ++i) {
        buttons[i] = new QPushButton("Button " + QString::number(i + 1), this);
        hlayout->addWidget(buttons[i]);
    }

    vlayout = new QVBoxLayout();
    vlayout->addWidget(draw_widget);
    vlayout->addLayout(hlayout);

    QWidget *central = new QWidget(this);
    central->setLayout(vlayout);
    setCentralWidget(central);

    setFixedSize(1000, 500);

    connect(buttons[0], SIGNAL(clicked(bool)), this, SLOT(detect()));
    connect(buttons[1], SIGNAL(clicked(bool)), this, SLOT(learn()));

    buttons[0]->setText("Detect");
    buttons[1]->setText("Learn");
}

MainWindow::~MainWindow() {
    delete open_button;
    delete draw_widget;
    for (int i = 0; i < BUTTONS_COUNT; ++i)
        delete buttons[i];
    delete hlayout;
    delete vlayout;
//    if (image) delete image;
}


// Functions for working with image layout
void MainWindow::openImage() {
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:/",
                                                    tr("Images (*.png *.xpm *.jpg)"));
//    if (filename == "") image=nullptr;
    image.setImage(filename);
    updateImage();
}

void MainWindow::updateImage() {
//    if (!image) return;
    draw_widget->setPixmap(QPixmap::fromImage(image));
}


// Functions for working with model
void MainWindow::detect() {

    // Change this values if needed
    const QString pathToModels = "/Users/serg/Desktop/ComputerVision/HumanDetector/";
    const QString modelName = "Model3.txt";
    const QString pathToTestImagesFolder = "/Users/serg/Desktop/ComputerVision/HumanDetector/Images/Test/";
    const QString pathToResultsFolder = "/Users/serg/Desktop/ComputerVision/HumanDetector/Images/Results/";
    const QString imagesType = ".png";

    std::vector<std::vector<double>> hogs;
    std::vector<std::pair<int, double>>modelOutput;

    // Change this values if needed
    const int startOfImagesNumeration = 5;
    const int endOfImagesNumeration = 244;

    model.load_file((pathToModels + modelName).toStdString());

    for (int test_num = startOfImagesNumeration; test_num <= endOfImagesNumeration; ++test_num) {
        hogs = {};

        QString test_path = pathToTestImagesFolder;
        test_path += QString::number(test_num);
        test_path += imagesType;
        AQImage img(test_path);

        for (int i = 0; i < img.width() - 80; i += 1)
            hogs.push_back(img.HOG(i));

        modelOutput = model.predict(hogs);

        // Finding max value (it is human)
        int max = 0;
        for (int i = 0; i < int(modelOutput.size()); ++i)
            if (modelOutput[i].second > modelOutput[max].second && modelOutput[i].first) max = i;

        // Bordering human
        if (modelOutput.size() != 0 && modelOutput[max].second > 0.7) img.drawBorder(max, 0, 80, 200);

        // Generating path where result will be saved
        QString res_path = pathToResultsFolder;
        res_path += QString::number(test_num);
        res_path += imagesType;

        img.save(res_path);
    }
}

void MainWindow::learn() {
    srand(time(0));
    std::ifstream data;
    std::string _str;
    std::vector<int> nums;
    std::vector<std::vector<double>> hogs;
    std::vector<int> values;

    // Change this values if needed
    const QString pathToDataFile = "/Users/serg/Desktop/ComputerVision/HumanDetector/data.idl";
    const QString pathToLearnImagesFolder = "/Users/serg/Desktop/ComputerVision/HumanDetector/Images/Learn/";
    const QString pathToModels = "/Users/serg/Desktop/ComputerVision/HumanDetector/";
    const QString modelName = "Model3.txt";
    const QString imagesType = ".png";

    data.open(pathToDataFile.toStdString(), std::ios::in);
    while (!data.eof()) {
        data >> _str;
        nums.push_back(atoi(_str.c_str()));
        if (nums.size() < 5) continue;

        QString path = pathToLearnImagesFolder;
        path += QString::number(nums[0]);
        path += imagesType;

        // Putting humans hog to hogs
        AQImage picture(path);
        hogs.push_back(picture.HOG(nums[2]));
        values.push_back(1);

        // Genering random pos in every learning pic. to init rectangular where no any human
        int rand_pos;
        while (true){
            rand_pos = rand() % (picture.width() - 80);
            if (!(rand_pos >= nums[1] && rand_pos <= nums[1] + 80)) break;
        }

        // Putting random background hog to hogs
        hogs.push_back(picture.HOG(rand_pos));
        values.push_back(0);

        nums = {};
    }

    data.close();
    model.learn(hogs, values);
    model.save_file((pathToModels + modelName).toStdString());
}
