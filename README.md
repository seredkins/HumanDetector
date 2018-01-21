ЧТОБЫ ЗАПУСТИТЬ ПРИЛОЖЕНИЕ НУЖНА БИБЛИОТЕКА QT
ДЛЯ ПРОСМОТРА В Qt Creator ОТКРОЙТЕ ФАЙЛ .pro И НАЖМИТЕ "НАСТРОИТЬ ПРОЕКТ"

Это приложение определяет есть ли на картинке человек, если есть, обводит его в рамочку
Оно использует машинное обучение (библиотеку liblinear(SVM)).
Модель обучается на Гистограммах Ориентированных Градиентов (HOG), вычисляемых для каждого прямоугольника 80х200

Чтобы испытать приложение можно:
1. Воспользоваться приложенными тестами
2. Загрузить свои

Для этого нужно поменять в mainwindow.cpp/detect() переменные:
    modelName - имя вашей модели (если изменено)
    pathToModels - ваш путь к папке с моделями
    pathToTestImagesFolder - ваш путь к папке с тестовыми изображениями
    pathToResultsFolder - ваш путь к папке с результатами
    imagesType - расширение ваших изображений (если используете свои)

Чтобы обучить свою модель нужно:
1. Подобрать обучающую выборку (минимум 250 фото)
2. Создать файл с данными о том, где на каждой картинке человек
    Файл должен быть вида:
        <номер картинки (= ее имя)> <начало прямоугольника с человеком по оси Y> <начало прямоугольника с человеком по оси X> <конец прямоугольника с человеком по оси Y> <конец прямоугольника с человеком по оси X>
        <номер картинки (= ее имя)> <начало прямоугольника с человеком по оси Y> <начало прямоугольника с человеком по оси X> <конец прямоугольника с человеком по оси Y> <конец прямоугольника с человеком по оси X>
        <номер картинки (= ее имя)> <начало прямоугольника с человеком по оси Y> <начало прямоугольника с человеком по оси X> <конец прямоугольника с человеком по оси Y> <конец прямоугольника с человеком по оси X>
        ...
    Обратите внимание что человек на фото желательно должен быть один
3. Поменять в mainwindow.cpp/learn() переменные:
    modelName - имя вашей (новой) модели
    pathToModels - ваш путь к папке с моделями
    pathToLearnImagesFolder - ваш путь к папке с обучающей выборкой
    imagesType - расширение ваших изображений
    pathToDataFile - путь к вышему файлу с данными
