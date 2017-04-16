#include "mainwindow.h"
#include "scale.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QKeyEvent>
#include <QMenu>
#include <qDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <QSlider>
#include <QEvent>
#include <fstream>
#include <time.h>
#include <QComboBox>
#define nullptr 0

void message_t::set(double d1,double d2,double d3,double minr,double maxr,
                        QString p,int in)
{

    detection1 = d1;
    detection2 = d2;
    detection3 = d3;
    minRadius = minr;
    maxRadius = maxr;
    path = p;
    interval = in;

}

mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent)
{
    qDebug()<<"lalala";
    state = Null;
    srand(time(nullptr));
    qDebug()<<"lalala";
    loadAttr();
    qDebug()<<"lalala";
    layout();
    setConnect();
    preAttr = "";
    name = "";
    backgroundname = "";
qDebug()<<"lalala";
    msg.set(128,0.5,0.6,2*setupWindow->dpixText->text().toDouble(),
            INF,"data/temp",1);

    viewWindow->setPath(currentPath);
    showWindow->setRadius(&msg.minRadius,&msg.maxRadius);
    setupWindow->setPath("data/camera/");
    showWindow->setPath(msg.path);
    isInverse = new bool();
    *isInverse = true;
    showWindow->setI(isInverse);

}

void mainwindow::loadAttr()
{
    QString path;
    QDir dir;
    path=dir.currentPath();

    QFile file;

    file.setFileName("path.txt");
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()){
        QMessageBox::critical(NULL, "Error", QStringLiteral("读取当前目录失败!"),
                              QMessageBox::Yes);
        return;
    }
    currentPath = file.readAll();
    file.close();

    file.setFileName("data.txt");

    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()){
        QMessageBox::critical(NULL, "Error", QStringLiteral("未能正确加载预设参数!"),
                              QMessageBox::Yes);
        return;
    }
    QVector<QString> strlist;
    while(!file.atEnd()){
        QString str = file.readAll();
        QString num = "";
        for(int i=0;i<str.size();i++){
            if(str[i]!=' ' && str[i]!='\n' &&str[i]!='\r'){
                num = num + str[i];
            }
            else if(str[i]=='\n'){
                QString name = strlist[0];
                qDebug()<<"name="<<name;
                premessage_t pmsg;
                pmsg.zmin = strlist[1];
                pmsg.zmax = strlist[2];
                pmsg.interval = strlist[3];
                pmsg.lamda = strlist[4];
                qDebug()<<"name="<<name;
               // pmsg.Mag = strlist[5];
                pmsg.dpix = strlist[5];
                pmsg.detection1 = strlist[6];
                pmsg.detection2 = strlist[7];
                pmsg.detection3 = strlist[8];
                qDebug()<<"name="<<name;
              //  pmsg.location = strlist[10];
                pmsg.path = strlist[9];
                pmsg.minRadius = strlist[10];
                qDebug()<<"name="<<name;
                pmsg.maxRadius = strlist[11];
                pmsg.plotnum = strlist[12];qDebug()<<"name="<<name;
                map[name] = pmsg;qDebug()<<"name="<<name;
                strlist.clear();
            }
            else if(str[i]==' '){
                strlist.push_back(num);
                num = "";
            }
        }
    }
    //qDebug()<<"ok load";
    file.close();
    file.setFileName("name.txt");
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        QString name("");
        QString str = file.readAll();
        for(int i=0;i<str.size();i++){
            if(str[i]==' '){
                namelist.push_back(name);
                name = "";
            }
            else{
                name = name + str[i];
            }
        }
    }
    file.close();
qDebug()<<"here";

}


void mainwindow::clearBuffer()
{

    QDir dir(msg.path);
    QFileInfoList fileList;
    QFileInfo curFile;
    if(!dir.exists()) {
        return;
    }
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    while(fileList.size()>0) {
        int infoNum=fileList.size();
        for(int i=infoNum-1;i>=0;i--){
            curFile=fileList[i];
            if(curFile.isFile()){
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }
        }
    }
    viewWindow->updateImg();
    viewWindow->clearImg();
    showWindow->setNum(0);
    toolWindow->slid->setNum(1);
    toolWindow->slid->setDisable();
    state = Null;
    emit(clear());
}

bool mainwindow::copyFile(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}

void mainwindow::saveAll(QString path,bool isOneKey)
{

    QChar* tmp = new QChar[6];
    tmp[0] = '[';
    tmp[1] = rand()%(123-41) + 41;
    tmp[2] = rand()%(123-41) + 41;
    tmp[3] = rand()%(123-41) + 41;
    tmp[4] = ']';
    tmp[5] = '\0';
    QString prefix(tmp);
    delete[] tmp;


    showWindow->setPrefix(prefix);
    for(int i = 0;i < imgnum;i++) {
        for(int j = 0; j < totalnum ;j++) {
            copyFile(msg.path+"/temp_plane_"+QString::number(i*totalnum + j)+".jpg" ,path+"/"+prefix+"temp_plane_"+QString::number(i*totalnum + j)+".jpg", true);
            copyFile(msg.path+"/temp_plane_"+QString::number(i*totalnum + j)+"i.jpg" ,path+"/"+prefix+"temp_plane_"+QString::number(i*totalnum + j)+"i.jpg", true);
        }
    }
    if(state == Rebuilt){
        QMessageBox::about(0,"Message",QStringLiteral("保存成功!自动命名前缀为：")+prefix);
        return;
    }
    for(int i = 0;i < imgnum;i++) {
        copyFile(msg.path+"/img_fuse"+QString::number(i)+".bmp" ,path+"/"+prefix+"img_fuse"+QString::number(i)+".bmp", true);
        copyFile(msg.path+"/img_fuse"+QString::number(i)+"i.bmp" ,path+"/"+prefix+"img_fuse"+QString::number(i)+"i.bmp", true);
    }
    if(state == Extend) {
        QMessageBox::about(0,"Message",QStringLiteral("保存成功!自动命名前缀为：")+prefix);
        return;
    }
    for(int i = 0;i < imgnum;i++) {
        copyFile(msg.path+"/img_binaryzation"+QString::number(i)+".bmp" ,path+"/"+prefix+"binaryzation"+QString::number(i)+".jpg", true);
        copyFile(msg.path+"/img_binaryzation"+QString::number(i)+"i.bmp" ,path+"/"+prefix+"binaryzation"+QString::number(i)+"i.jpg", true);
        copyFile(msg.path+"/map_highfrequency"+QString::number(i)+".tiff" ,path+"/"+prefix+"map_highfrequency"+QString::number(i)+".tiff", true);
        copyFile(msg.path+"/map_highfrequency"+QString::number(i)+"i.tiff" ,path+"/"+prefix+"map_highfrequency"+QString::number(i)+"i.tiff", true);

        copyFile(msg.path+"/map_lowfrequency"+QString::number(i)+".tiff" ,path+"/"+prefix+"map_lowfrequency"+QString::number(i)+".tiff", true);
        copyFile(msg.path+"/map_lowfrequency"+QString::number(i)+"i.tiff" ,path+"/"+prefix+"map_lowfrequency"+QString::number(i)+"i.tiff", true);
   }
    if(state == Detect){
        QMessageBox::about(0,"Message",QStringLiteral("保存成功!自动命名前缀为：")+prefix);
        return;
    }
    for(int i = 0;i < imgnum;i++) {
        if(!isOneKey){

            showWindow->save(i,path+"/"+prefix+"/location" + QString::number(i)+".xls",path+"/"+prefix+"location"+QString::number(i)+".jpg",false);
            showWindow->save(i,path+"/"+prefix+"/ilocation" + QString::number(i)+".xls",path+"/"+prefix+"ilocation"+QString::number(i)+".jpg",true);
        }
    }

    QMessageBox::about(0,"Message",QStringLiteral("保存成功!自动命名前缀为：")+prefix);
}

void mainwindow::saveImg()
{
    if(state == Null){
        QMessageBox::about(0,"Message",QStringLiteral("没有可以保存的图片"));
        return;
    }
    QUrl url = QFileDialog::getExistingDirectoryUrl
            (this);
    if(url.toString().size()<=8)return;

    QString path = url.toString().mid(8);
    saveAll(path,false);

}

void mainwindow::setConnect()
{
    connect(initWindow->okBtn,SIGNAL(clicked()),this,SLOT(initOk()));
    connect(initWindow->cancelBtn,SIGNAL(clicked()),this,SLOT(initCancel()));

    connect(menuWindow->saveAction,SIGNAL(triggered()),this,SLOT(saveImg()));
    connect(menuWindow->importAction,SIGNAL(triggered()),this,SLOT(importImg()));
    connect(menuWindow->importBackgroundAction,SIGNAL(triggered()),this,SLOT(importBackgroundImg()));
    connect(menuWindow->setAction,SIGNAL(triggered()),this,SLOT(setup()));
    connect(menuWindow->clearAction,SIGNAL(triggered()),this,SLOT(clearBuffer()));
    connect(menuWindow->changeAction1,SIGNAL(triggered()),this,SLOT(changeColor1()));
    connect(menuWindow->changeAction2,SIGNAL(triggered()),this,SLOT(changeColor2()));
    connect(menuWindow->startAction,SIGNAL(triggered()),setupWindow,SLOT(play()));

    connect(toolWindow->zoomInBtn,SIGNAL(clicked()),this,SLOT(zoomIn()));
    connect(toolWindow->zoomOutBtn,SIGNAL(clicked()),this,SLOT(zoomOut()));
    connect(toolWindow->originBtn,SIGNAL(clicked()),this,SLOT(original()));
    connect(toolWindow->fullScreenBtn,SIGNAL(clicked()),this,SLOT(fullScreen()));

    connect(toolWindow->slid->nextbtn,SIGNAL(clicked()),this,SLOT(nextImg()));
    connect(toolWindow->slid->lastbtn,SIGNAL(clicked()),this,SLOT(lastImg()));
    connect(toolWindow->slid->slid, SIGNAL(valueChanged(int)), showWindow, SLOT(setNum(int)));

    connect(toolWindow->inverseBtn,SIGNAL(clicked()),this,SLOT(setInverse()));
    connect(toolWindow->scaleBtn,SIGNAL(clicked()),this,SLOT(Scale()));
    connect(toolWindow->calculateBtn,SIGNAL(clicked()),this,SLOT(calculate()));

    connect(toolline->lastBtn,SIGNAL(clicked()),this,SLOT(setLastImg()));
    connect(toolline->nextBtn,SIGNAL(clicked()),this,SLOT(setNextImg()));
    connect(toolline->lastpairBtn,SIGNAL(clicked()),this,SLOT(setLastPage()));
    connect(toolline->nextpairBtn,SIGNAL(clicked()),this,SLOT(setNextPage()));
    connect(toolline->chooseCatalogBtn,SIGNAL(clicked()),this,SLOT(chooseCatalog()));
    connect(toolline->clearBtn,SIGNAL(clicked()),viewWindow,SLOT(clear()));
    connect(viewWindow,SIGNAL(loadImg(QString)),this,SLOT(setPath(QString)));
    connect(viewWindow,SIGNAL(choose(int)),showWindow,SLOT(setIndex(int)));
    connect(viewWindow,SIGNAL(choose(int)),this,SLOT(setIndex(int)));
    connect(toolline->chooseBackgroundBtn,SIGNAL(clicked()),this,SLOT(chooseBackground()));
    connect(toolline->clearBtn,SIGNAL(clicked()),viewWindow,SLOT(cancelBackground()));
    connect(toolline->allchooseBtn,SIGNAL(clicked()),viewWindow,SLOT(chooseAll()));

    connect(setupWindow->tabWindow->came,SIGNAL(changePath(QString)),
            viewWindow,SLOT(setPath(QString)));
    connect(setupWindow->tabWindow->stopBtn,SIGNAL(clicked()),this,SLOT(updatePath()));
}

void mainwindow::changeColor1()
{
    if(showWindow->isShowWhite()){
        showWindow->isShowWhite(false);
    }
    else {
        showWindow->isShowWhite(true);
    }
}

void mainwindow::changeColor2()
{
    if(showWindow->isDrawWhite()) {
        showWindow->isDrawWhite(false);
    }
    else {
        showWindow->isDrawWhite(true);
    }
}

void mainwindow::updatePath()
{
    viewWindow->setPath("data/camera");
    cpath = "data/temp";
}

void mainwindow::setIndex(int in)
{
    count = 1;
    index = in;

    if(index == -1) {
        toolWindow->slid->setDisable();
    }
    else {
        toolWindow->slid->setNum(1);
        toolWindow->slid->setEnable();
    }
}

void mainwindow::chooseBackground()
{
    if(viewWindow->isEmpty()) {
        QMessageBox::critical(NULL, "Error", QStringLiteral("您尚未选中任何图片\n"),
                              QMessageBox::Yes);
        return;
    }
    QString path = QFileDialog::getOpenFileName(this,
        tr("Choose Background Image"),
        "",
        tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF")); //选择路径
    if(path.isEmpty()) {
        return;
    }
    else{
        QMessageBox::about(0,"Message",QStringLiteral("导入成功!是否与选中图片进行去背景操作?"));
    }
    viewWindow->setBackground(path);

    Mat background=imread(path.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
    background.convertTo(background,CV_64F,1.0/255.0);

    std::vector<std::string> vec = viewWindow->getFilename();
    for(int i = 0;i < vec.size(); i++) {
        Mat hologram=imread(vec[i],CV_LOAD_IMAGE_GRAYSCALE);
        hologram.convertTo(hologram,CV_64F,1.0/255.0);

        if(background.rows!=hologram.rows||background.cols!=hologram.cols) {
            QMessageBox::critical(NULL, "Error", QStringLiteral("您选择的背景图与原图大小不相同\n"),
                              QMessageBox::Yes);
            return;
        }
        hologram=hologram-background;
        double gmax,gmin;
        minMaxIdx(hologram,&gmax,&gmin);
        hologram=(hologram-gmin)/(gmax-gmin);
        normalize(hologram,hologram,0.0,255.0,NORM_MINMAX);

        imwrite(vec[i].substr(0,vec[i].size()-4)+"r"+vec[i].substr(vec[i].size()-4),hologram);

    }
    viewWindow->reset();
}

void mainwindow::updatePath(QString p )
{
    currentPath = p;

    viewWindow->updatePath(p);
    viewWindow->updateImg();
}

void mainwindow::chooseCatalog()
{
    if(cata->isOpen) {
        return;
    }
    cata = new catalog();
    connect(cata->updatebtn,SIGNAL(clicked()),viewWindow,SLOT(updateImg()));
    connect(cata,SIGNAL(updatePath(QString)),this,SLOT(updatePath(QString)));
    cata->show();
    cata->setPath(currentPath);
}

void mainwindow::setLastPage()
{
    viewWindow->setLastPage();
}

void mainwindow::setNextPage()
{
    viewWindow->setNextPage();
}

void mainwindow::setNextImg()
{
    viewWindow->setNextImg();
}

void mainwindow::setLastImg()
{
    viewWindow->setLastImg();
}

void mainwindow::setInverse()
{
    if(toolWindow->inverseBtn->isDown){
        *isInverse = true;
    }
    else *isInverse = false;
    showWindow->setInverse(*isInverse);
    showWindow->reloadImg();
}

void mainwindow::Scale()
{
    scale* sca;
    sca = showWindow->getSca();
    if(sca==nullptr)return;
    if(sca->isVisible()){
        sca->setVisible(false);
    }
    else{
        sca->setVisible(true);
    }
}

void mainwindow::calculate()
{
    if(showWindow->getCalculate()){
        showWindow->setCalculate(false);
    }
    else {
        showWindow->reset();
        showWindow->setCalculate(true);
    }
}

void mainwindow::nextImg()
{
    if(count>=total){
        return;
    }
    count++;
}

void mainwindow::lastImg()
{
    if(count<=1){
        return;
    }
    count--;
}

void mainwindow::zoomIn()
{
    showWindow->zoomIn();
}

void mainwindow::zoomOut()
{
    showWindow->zoomOut();
}

void mainwindow::original()
{
    showWindow->original();
}

void mainwindow::fullScreen()
{
    showWindow->fullScreen();
}

void mainwindow::layout()
{
    menuWindow = new menu();
    setupWindow = new setupwindow();
    showWindow = new showtabwindow();
    viewWindow = new viewwindow();
    toolWindow = new toolwindow(this);
    initWindow = new initwindow(&namelist,&map);
    toolline = new toolLine();
    dock = new QDockWidget(QStringLiteral("工具"),this);
    dock->setWidget(toolWindow);
    dock->setFeatures(QDockWidget::DockWidgetFloatable);
    showWindow->setPath("data/temp");
    showWindow->setPlotNum(map[namelist[0]].plotnum.toInt());
    for(int i=0;i<VMAX;i++){
        hlayout[i] = new QHBoxLayout();
    }
    for(int i=0;i<HMAX;i++){
        vlayout[i] = new QVBoxLayout();
    }


    hlayout[0]->addWidget(dock);
    hlayout[0]->addWidget(showWindow);
    hlayout[0]->setContentsMargins(0,0,0,0);

    vlayout[0]->addLayout(hlayout[0]);
    vlayout[0]->addWidget(toolline);
    vlayout[0]->addWidget(viewWindow);
    vlayout[0]->setMargin(0);
    vlayout[0]->setSpacing(0);

    hlayout[1]->addLayout(vlayout[0]);
    hlayout[1]->addWidget(setupWindow);
    hlayout[1]->setContentsMargins(10,0,10,10);

    vlayout[1]->addWidget(menuWindow);
    vlayout[1]->setSpacing(0);
    vlayout[1]->addLayout(hlayout[1]);
    vlayout[1]->setMargin(0);


    initWindow->show();
    setLayout(vlayout[1]);
}

void mainwindow::paintEvent(QPaintEvent*)
{
    QPainter paint(this);
    paint.setPen(QColor(240,240,240));
    paint.setBrush(QColor(240,240,240));
    paint.drawRect(rect());
}

void mainwindow::initOk()
{
    preAttr = initWindow->combobox->currentText();
    setupWindow->zminText->setText(initWindow->startBox->text());
    setupWindow->zmaxText->setText(initWindow->endBox->text());
    setupWindow->dpixText->setText(initWindow->pixelBox->text());
    setupWindow->lamdaText->setText(initWindow->waveBox->text());

  //  setupWindow->MagText->setText(initWindow->planeBox->text());
    setupWindow->intervalText->setText(initWindow->planeBox->text());

    msg.minRadius = 2*initWindow->pixelBox->text().toDouble();

    msg.set(map[initWindow->combobox->currentText()].detection1.toDouble(),
            map[initWindow->combobox->currentText()].detection2.toDouble(),
            map[initWindow->combobox->currentText()].detection3.toDouble(),
            map[initWindow->combobox->currentText()].minRadius.toDouble(),
            map[initWindow->combobox->currentText()].maxRadius.toDouble(),
            map[initWindow->combobox->currentText()].path,
            map[initWindow->combobox->currentText()].plotnum.toInt());
    toolWindow->set(setupWindow->zminText->text().toDouble(),setupWindow->intervalText->text().toDouble());

    delete initWindow;

    initWindow = nullptr;
}

void mainwindow::initCancel()
{
    delete initWindow;
    initWindow = nullptr;
}

void mainwindow::openInitWindow()
{
    if(initWindow){
        return;
    }
    initWindow = new initwindow(&namelist,&map);
    initWindow->show();
    connect(initWindow->okBtn,SIGNAL(clicked()),this,SLOT(initOk()));
    connect(initWindow->cancelBtn,SIGNAL(clicked()),this,SLOT(initCancel()));
}

void mainwindow::importImg()
{
    name = QFileDialog::getOpenFileName(this,
        tr("Choose Image"),
        "",
        tr("QImages(*.bmp *.png; *.jpg;)")); //选择路径
    if(name.isEmpty()) {
        return;
    }
    else{
        setupWindow->loadImg(name);
    }

}

void mainwindow::setPath(QString p)
{
    name = p;
    setupWindow->loadImg(p);
}

void mainwindow::importBackgroundImg()
{
    if(name.isEmpty()){
        QMessageBox::critical(NULL, "Error", QStringLiteral("请先导入原图!"),
                              QMessageBox::Yes);
        return;
    }
    backgroundname = QFileDialog::getOpenFileName(this,
        tr("Choose Background Image"),
        "",
        tr("*.bmp;; *.png;; *.jpg;; *.tif;; *.GIF")); //选择路径
    if(backgroundname.isEmpty()) {
        return;
    }
    else{
        QMessageBox::about(0,"Message",QStringLiteral("导入成功!是否与当前图片进行去背景操作?"));
    }
    Mat background=imread(backgroundname.toStdString(),CV_LOAD_IMAGE_GRAYSCALE);
    background.convertTo(background,CV_64F,1.0/255.0);

    std::string p = setupWindow->getPath().toStdString();

    Mat hologram=imread(p,CV_LOAD_IMAGE_GRAYSCALE);
    hologram.convertTo(hologram,CV_64F,1.0/255.0);

    if(background.rows!=hologram.rows||background.cols!=hologram.cols) {
        QMessageBox::critical(NULL, "Error", QStringLiteral("您选择的背景图与原图大小不相同\n"),
                          QMessageBox::Yes);
        return;
    }
    hologram=hologram-background;
    double gmax,gmin;
    minMaxIdx(hologram,&gmax,&gmin);
    hologram=(hologram-gmin)/(gmax-gmin);
    normalize(hologram,hologram,0.0,255.0,NORM_MINMAX);

    imwrite(p.substr(0,p.size()-4)+"r"+p.substr(p.size()-4),hologram);
    setupWindow->loadImg(QString::fromStdString(p.substr(0,p.size()-4)+"r"+p.substr(p.size()-4)));
}

void mainwindow::setup()
{
    if(setupDialog->isCreate)return;


    setupDialog = new setupdialog(preAttr,&namelist,&map,setupWindow->dpixText->text().toDouble(),&msg);
    setupDialog->show();
    connect(setupDialog->okBtn,SIGNAL(clicked()),this,SLOT(setupOk()));
    connect(setupDialog->cancelBtn,SIGNAL(clicked()),this,SLOT(setupCancel()));
}

void mainwindow::updateAttr(QString name,premessage_t msg)
{
    map[name] = msg;
    namelist.push_back(name);
    setupDialog->updateAttr();
}

void mainwindow::setupOk()
{
    msg.set(setupDialog->line1->text().toDouble(),
            setupDialog->line2->text().toDouble(),
            setupDialog->line3->text().toDouble(),
            setupDialog->minRadiusText->text().toInt(),
            setupDialog->maxRadiusText->text().toInt(),
            setupDialog->fileDirText->text(),
            setupDialog->intervalText->text().toInt());
    preAttr = setupDialog->attrBox->currentText();
    setupWindow->zminText->setText(map[preAttr].zmin);
    setupWindow->zmaxText->setText(map[preAttr].zmax);
    setupWindow->dpixText->setText(map[preAttr].dpix);
    setupWindow->lamdaText->setText(map[preAttr].lamda);

  //  setupWindow->MagText->setText(initWindow->planeBox->text());
    setupWindow->intervalText->setText(map[preAttr].interval);
    showWindow->setPath(msg.path);
    showWindow->setPlotNum(setupDialog->intervalText->text().toInt());
    delete setupDialog;
}

void mainwindow::setupCancel()
{
    delete setupDialog;
}

void mainwindow::keyReleaseEvent(QKeyEvent* event)
{
    if(viewWindow->isIn &&event->key() == CTRL) {
        viewWindow->release();
    }
    else if(viewWindow->isIn && event->key() == SHIFT) {
        viewWindow->release();
    }

}

void mainwindow::keyPressEvent(QKeyEvent* event)
{
    if(viewWindow->isIn &&event->key() == CTRL) {
        viewWindow->pressCtrl();
    }
    else if(viewWindow->isIn && event->key() == SHIFT) {
        viewWindow->pressShift();
    }

}

void mainwindow::closeEvent(QCloseEvent*)
{
    exit(1);
}
