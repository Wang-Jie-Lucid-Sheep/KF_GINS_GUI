#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Eigen/Dense>
#include <absl/time/clock.h>
#include <iomanip>
#include <iostream>
#include <yaml-cpp/yaml.h>

#include "common/angle.h"
#include "fileio/filesaver.h"
#include "fileio/gnssfileloader.h"
#include "fileio/imufileloader.h"

#include "kf-gins/gi_engine.h"

bool loadConfig(YAML::Node &config, GINSOptions &options);
void writeNavResult(double time, NavState &navstate, FileSaver &navfile, FileSaver &imuerrfile);
void writeSTD(double time, Eigen::MatrixXd &cov, FileSaver &stdfile);
void appendColoredText(QTextBrowser* textBrowser, const QString& text, const QColor& color);

string Yaml_Path;
string GINS_Output_path;
string GNSS_Input_path;
string INS_Input_path;
std::vector<double> Pos_result;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("KF—GINS组合导航解算程序");
    setWindowIcon(QIcon(":/Picture/ICON.ico"));
    connect(ui->action_Config,&QAction::triggered,this,&MainWindow::Input_Config_Yaml);
    connect(ui->action_POS,&QAction::triggered,this,&MainWindow::Save_Pos);
    connect(ui->action_Draw_result,&QAction::triggered,this,&MainWindow::Draw_result);
    connect(ui->action_Draw_error,&QAction::triggered,this,&MainWindow::Draw_error);
    connect(ui->action_Exit,&QAction::triggered,this,&MainWindow::Exit_action);
    connect(ui->action_Information,&QAction::triggered,this,&MainWindow::Information_action);

    appendColoredText(this->ui->textBrowser, "KF-GINS: An EKF-Based GNSS/INS Integrated Navigation System", Qt::black);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    appendColoredText(this->ui->textBrowser, "KF-GINS:一种基于EKF的GNSS/INS组合导航系统", Qt::black);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);

    this->ui->progressBar->setValue(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void appendColoredText(QTextBrowser* textBrowser, const QString& text, const QColor& color) {
    QTextCursor cursor(textBrowser->textCursor());
    cursor.movePosition(QTextCursor::End);
    cursor.insertHtml(QString("<span style=\"color: %1;\">%2</span>").arg(color.name(), text));
    cursor.movePosition(QTextCursor::End);
}

void MainWindow::Input_Config_Yaml()
{
    QString Config_Yaml_Path = QFileDialog::getOpenFileName(this,"请选择一个Yaml文件",QCoreApplication::applicationFilePath());
    if(Config_Yaml_Path.isEmpty())
    {
        QMessageBox::warning(this,"警告","请选择一个Yaml文件");
    }
    else
    {
        Yaml_Path=Config_Yaml_Path.toStdString();
        appendColoredText(this->ui->textBrowser, "Yaml文件导入完成", Qt::blue);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    }
}

void MainWindow::Save_Pos()
{
    QString Save_Pos_Path = QFileDialog::getOpenFileName(this,"请选择一个Yaml文件",QCoreApplication::applicationFilePath());
    if(Save_Pos_Path.isEmpty())
    {
        QMessageBox::warning(this,"警告","请选择一个Yaml文件");
    }
    else
    {
        string save_pos_path=Save_Pos_Path.toStdString();
        std::ofstream ans;
        ans.open(save_pos_path,std::ios::out);

        ans<<"%  "<<"GPST          "<<"latitude(deg) "<<"longitude(deg)  "<<"height(m)     "<<"Q"<<'\n';
        for(int i=0;i<Pos_result.size();i+=100)
        {
            for(int j=0;j<=4;j++)
            {
                //string pos_result = std::to_string();
                ans<<std::setprecision(9)<<Pos_result[i+j]<<"   ";
            }
            ans<<1<<'\n';
        }
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "pos文件导出完成", Qt::blue);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    }
}

void MainWindow::on_GINS_Caculate_clicked()
{

    ui->textBrowser->clear();
/*
    if (argc != 2) {
        std::cout << "usage: KF-GINS kf-gins.yaml" << std::endl;
        return;
    }
*/
    if (Yaml_Path.empty()) {
        //std::cout << "usage: KF-GINS kf-gins.yaml" << std::endl;
        appendColoredText(this->ui->textBrowser, "Error:usage: KF-GINS kf-gins.yaml", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "错误：未导入KF-GINS kf-gins.yaml配置文件", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        return;
    }
    if (GNSS_Input_path.empty()||GNSS_Input_path.empty()||INS_Input_path.empty()) {
        //std::cout << "usage: KF-GINS kf-gins.yaml" << std::endl;
        appendColoredText(this->ui->textBrowser, "Error:Please configure the input file path and output path first", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "错误：请先配置输入文件路径与输出路径", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        return;
    }
    //std::cout << std::endl << "KF-GINS: An EKF-Based GNSS/INS Integrated Navigation System" << std::endl << std::endl;
    appendColoredText(this->ui->textBrowser, "KF-GINS: An EKF-Based GNSS/INS Integrated Navigation System", Qt::black);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    appendColoredText(this->ui->textBrowser, "KF-GINS：一种基于EKF的GNSS/INS组合导航系统", Qt::black);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    auto ts = absl::Now();

    // 加载配置文件
    // load configuration file
    YAML::Node config;
    try {
        config = YAML::LoadFile(Yaml_Path);
    } catch (YAML::Exception &exception) {
/*
        std::cout << "Failed to read configuration file. Please check the path and format of the configuration file!"
                  << std::endl;
*/
        appendColoredText(this->ui->textBrowser, "Error:Failed to read configuration file. Please check the path and format of the configuration file!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "错误：无法读取配置文件。请检查配置文件的路径和格式！", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        return;
    }

    // 读取配置参数到GINSOptioins中，并构造GIEngine
    // load configuration parameters to GINSOptioins
    GINSOptions options;
    if (!loadConfig(config, options)) {
//        std::cout << "Error occurs in the configuration file!" << std::endl;
        appendColoredText(this->ui->textBrowser, "Error:Error occurs in the configuration file!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "错误：配置文件出错！", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        return;
    }

    // 读取文件路径配置
    // load filepath configuration
    std::string imupath, gnsspath, outputpath;
    try {
/*
        imupath    = config["imupath"].as<std::string>();
        gnsspath   = config["gnsspath"].as<std::string>();
        outputpath = config["outputpath"].as<std::string>();
*/
        imupath = INS_Input_path;
        gnsspath = GNSS_Input_path;
        outputpath = GINS_Output_path;

    } catch (YAML::Exception &exception) {
//        std::cout << "Failed when loading configuration. Please check the file path and output path!" << std::endl;
        appendColoredText(this->ui->textBrowser, "Error:Failed when loading configuration. Please check the file path and output path!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "错误：加载配置时失败，请检查文件路径和输出路径!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        return;
    }
    appendColoredText(this->ui->textBrowser, "GNSS文件导入完成", Qt::blue);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    appendColoredText(this->ui->textBrowser, "INS文件导入完成", Qt::blue);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    // imu数据配置，数据处理区间
    // imudata configuration， data processing interval
    int imudatalen, imudatarate;
    double starttime, endtime;
    try {
        imudatalen  = config["imudatalen"].as<int>();
        imudatarate = config["imudatarate"].as<int>();
        starttime   = config["starttime"].as<double>();
        endtime     = config["endtime"].as<double>();
    } catch (YAML::Exception &exception) {
/*
        std::cout << "Failed when loading configuration. Please check the data length, data rate, and the process time!"
                  << std::endl;
*/
        appendColoredText(this->ui->textBrowser, "Failed when loading configuration. Please check the data length, data rate, and the process time!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "错误：加载配置时失败。请检查数据长度、数据速率和处理时间！", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        return;
    }

    // 加载GNSS文件和IMU文件
    // load GNSS file and IMU file
    GnssFileLoader gnssfile(gnsspath);
    ImuFileLoader imufile(imupath, imudatalen, imudatarate);

    // 构造GIEngine
    // Construct GIEngine
    GIEngine giengine(options);

    // 构造输出文件
    // construct output file
    // navfile: gnssweek(1) + time(1) + pos(3) + vel(3) + euler angle(3) = 11
    // imuerrfile: time(1) + gyrbias(3) + accbias(3) + gyrscale(3) + accscale(3) = 13
    // stdfile: time(1) + pva_std(9) + imubias_std(6) + imuscale_std(6) = 22
    int nav_columns = 11, imuerr_columns = 13, std_columns = 22;
    FileSaver navfile(outputpath + "/KF_GINS_Navresult.nav", nav_columns, FileSaver::TEXT);
    FileSaver imuerrfile(outputpath + "/KF_GINS_IMU_ERR.txt", imuerr_columns, FileSaver::TEXT);
    FileSaver stdfile(outputpath + "/KF_GINS_STD.txt", std_columns, FileSaver::TEXT);

    // 检查文件是否正确打开
    // check if these files are all opened
    if (!gnssfile.isOpen() || !imufile.isOpen() || !navfile.isOpen() || !imuerrfile.isOpen() || !stdfile.isOpen()) {
        //std::cout << "Failed to open data file!" << std::endl;
        appendColoredText(this->ui->textBrowser, "Failed to open data file!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "打开数据文件失败!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        return;
    }

    // 检查处理时间
    // check process time
    if (endtime < 0) {
        endtime = imufile.endtime();
    }
    if (endtime > 604800 || starttime < imufile.starttime() || starttime > endtime) {
        //std::cout << "Process time ERROR!" << std::endl;
        appendColoredText(this->ui->textBrowser, "Process time ERROR!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        appendColoredText(this->ui->textBrowser, "进程时间错误!", Qt::red);
        appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
        return;
    }

    // 数据对齐
    // data alignment
    IMU imu_cur;
    do {
        imu_cur = imufile.next();
    } while (imu_cur.time < starttime);

    GNSS gnss;
    do {
        gnss = gnssfile.next();
    } while (gnss.time <= starttime);

    // 添加IMU数据到GIEngine中，补偿IMU误差
    // add imudata to GIEngine and compensate IMU error
    giengine.addImuData(imu_cur, true);

    // 添加GNSS数据到GIEngine
    // add gnssdata to GIEngine
    giengine.addGnssData(gnss);

    // 用于保存处理结果
    // used to save processing results
    double timestamp;
    NavState navstate;
    Eigen::MatrixXd cov;

    // 用于显示处理进程
    // used to display processing progress
    int percent = 0, lastpercent = 0;
    double interval = endtime - starttime;

    while (true) {
        // 当前IMU状态时间新于GNSS时间时，读取并添加新的GNSS数据到GIEngine
        // load new gnssdata when current state time is newer than GNSS time and add it to GIEngine
        if (gnss.time < imu_cur.time && !gnssfile.isEof()) {
            gnss = gnssfile.next();
            giengine.addGnssData(gnss);
        }

        // 读取并添加新的IMU数据到GIEngine
        // load new imudata and add it to GIEngine
        imu_cur = imufile.next();
        if (imu_cur.time > endtime || imufile.isEof()) {
            break;
        }
        giengine.addImuData(imu_cur);

        // 处理新的IMU数据
        // process new imudata
        giengine.newImuProcess();

        // 获取当前时间，IMU状态和协方差
        // get current timestamp, navigation state and covariance
        timestamp = giengine.timestamp();
        navstate  = giengine.getNavState();
        cov       = giengine.getCovariance();

        // 保存处理结果
        // save processing results
        writeNavResult(timestamp, navstate, navfile, imuerrfile);
        writeSTD(timestamp, cov, stdfile);

        //Pos_result.clear();
        Pos_result.push_back(0);
        Pos_result.push_back(timestamp);
        Pos_result.push_back(navstate.pos[0] * R2D);
        Pos_result.push_back(navstate.pos[1] * R2D);
        Pos_result.push_back(navstate.pos[2]);

        // 显示处理进展
        // display processing progress
        percent = int((imu_cur.time - starttime) / interval * 100);
        if (percent - lastpercent >= 1) {
            //std::cout << " - Processing: " << std::setw(3) << percent << "%\r" << std::flush;
            this->ui->progressBar->setValue(percent);
            appendColoredText(this->ui->textBrowser, "- Processing:", Qt::darkBlue);
            appendColoredText(this->ui->textBrowser, QString::number(percent), Qt::darkBlue);
            appendColoredText(this->ui->textBrowser, "%<br>", Qt::darkBlue);
            this->ui->textBrowser->moveCursor(QTextCursor::End);
            lastpercent = percent;
        }
    }

    // 关闭打开的文件
    // close opened file
    imufile.close();
    gnssfile.close();
    navfile.close();
    imuerrfile.close();
    stdfile.close();

    // 处理完毕
    // process finish
    auto te = absl::Now();
    //std::cout << std::endl << std::endl << "KF-GINS Process Finish! ";
    appendColoredText(this->ui->textBrowser, "<br>KF-GINS Process Finish!", Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, "KF-GINS解算完成!", Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, "From", Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, QString::number(starttime,'f',2), Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, " s to ", Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, QString::number(endtime,'f',2), Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, " s, total ", Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, QString::number(interval,'f',2), Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, " s!<br>", Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, "Cost ", Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, QString::number(absl::ToDoubleSeconds(te - ts),'f',2), Qt::darkGreen);
    appendColoredText(this->ui->textBrowser, " s in total", Qt::darkGreen);
    this->ui->textBrowser->moveCursor(QTextCursor::End);



    //std::cout << "From " << starttime << " s to " << endtime << " s, total " << interval << " s!" << std::endl;
    //std::cout << "Cost " << absl::ToDoubleSeconds(te - ts) << " s in total" << std::endl;

    return;
}

/**
 * @brief 从配置文件中读取GIEngine相关的初始状态，并转换为标准单位
 *        Load initial states of GIEngine from configuration file and convert them to standard units
 * */
bool loadConfig(YAML::Node &config, GINSOptions &options) {

    // 读取初始位置(纬度 经度 高程)、(北向速度 东向速度 垂向速度)、姿态(欧拉角，ZYX旋转顺序, 横滚角、俯仰角、航向角)
    // load initial position(latitude longitude altitude)
    //              velocity(speeds in the directions of north, east and down)
    //              attitude(euler angle, ZYX, roll, pitch and yaw)
    std::vector<double> vec1, vec2, vec3, vec4, vec5, vec6;
    try {
        vec1 = config["initpos"].as<std::vector<double>>();
        vec2 = config["initvel"].as<std::vector<double>>();
        vec3 = config["initatt"].as<std::vector<double>>();
    } catch (YAML::Exception &exception) {
        std::cout << "Failed when loading configuration. Please check initial position, velocity, and attitude!"
                  << std::endl;
        return false;
    }
    for (int i = 0; i < 3; i++) {
        options.initstate.pos[i]   = vec1[i] * D2R;
        options.initstate.vel[i]   = vec2[i];
        options.initstate.euler[i] = vec3[i] * D2R;
    }
    options.initstate.pos[2] *= R2D;

    // 读取IMU误差初始值(零偏和比例因子)
    // load initial imu error (bias and scale factor)
    try {
        vec1 = config["initgyrbias"].as<std::vector<double>>();
        vec2 = config["initaccbias"].as<std::vector<double>>();
        vec3 = config["initgyrscale"].as<std::vector<double>>();
        vec4 = config["initaccscale"].as<std::vector<double>>();
    } catch (YAML::Exception &exception) {
        std::cout << "Failed when loading configuration. Please check initial IMU error!" << std::endl;
        return false;
    }
    for (int i = 0; i < 3; i++) {
        options.initstate.imuerror.gyrbias[i]  = vec1[i] * D2R / 3600.0;
        options.initstate.imuerror.accbias[i]  = vec2[i] * 1e-5;
        options.initstate.imuerror.gyrscale[i] = vec3[i] * 1e-6;
        options.initstate.imuerror.accscale[i] = vec4[i] * 1e-6;
    }

    // 读取初始位置、速度、姿态(欧拉角)的标准差
    // load initial position std, velocity std and attitude(euler angle) std
    try {
        vec1 = config["initposstd"].as<std::vector<double>>();
        vec2 = config["initvelstd"].as<std::vector<double>>();
        vec3 = config["initattstd"].as<std::vector<double>>();
    } catch (YAML::Exception &exception) {
        std::cout << "Failed when loading configuration. Please check initial std of position, velocity, and attitude!"
                  << std::endl;
        return false;
    }
    for (int i = 0; i < 3; i++) {
        options.initstate_std.pos[i]   = vec1[i];
        options.initstate_std.vel[i]   = vec2[i];
        options.initstate_std.euler[i] = vec3[i] * D2R;
    }

    // 读取IMU噪声参数
    // load imu noise parameters
    try {
        vec1 = config["imunoise"]["arw"].as<std::vector<double>>();
        vec2 = config["imunoise"]["vrw"].as<std::vector<double>>();
        vec3 = config["imunoise"]["gbstd"].as<std::vector<double>>();
        vec4 = config["imunoise"]["abstd"].as<std::vector<double>>();
        vec5 = config["imunoise"]["gsstd"].as<std::vector<double>>();
        vec6 = config["imunoise"]["asstd"].as<std::vector<double>>();

        options.imunoise.corr_time = config["imunoise"]["corrtime"].as<double>();
    } catch (YAML::Exception &exception) {
        std::cout << "Failed when loading configuration. Please check IMU noise!" << std::endl;
        return false;
    }
    for (int i = 0; i < 3; i++) {
        options.imunoise.gyr_arw[i]      = vec1[i];
        options.imunoise.acc_vrw[i]      = vec2[i];
        options.imunoise.gyrbias_std[i]  = vec3[i];
        options.imunoise.accbias_std[i]  = vec4[i];
        options.imunoise.gyrscale_std[i] = vec5[i];
        options.imunoise.accscale_std[i] = vec6[i];
    }

    // 读取IMU误差初始标准差,如果配置文件中没有设置，则采用IMU噪声参数中的零偏和比例因子的标准差
    // Load initial imu bias and scale std, set to bias and scale instability std if load failed
    try {
        vec1 = config["initbgstd"].as<std::vector<double>>();
    } catch (YAML::Exception &exception) {
        vec1 = {options.imunoise.gyrbias_std.x(), options.imunoise.gyrbias_std.y(), options.imunoise.gyrbias_std.z()};
    }
    try {
        vec2 = config["initbastd"].as<std::vector<double>>();
    } catch (YAML::Exception &exception) {
        vec2 = {options.imunoise.accbias_std.x(), options.imunoise.accbias_std.y(), options.imunoise.accbias_std.z()};
    }
    try {
        vec3 = config["initsgstd"].as<std::vector<double>>();
    } catch (YAML::Exception &exception) {
        vec3 = {options.imunoise.gyrscale_std.x(), options.imunoise.gyrscale_std.y(),
                options.imunoise.gyrscale_std.z()};
    }
    try {
        vec4 = config["initsastd"].as<std::vector<double>>();
    } catch (YAML::Exception &exception) {
        vec4 = {options.imunoise.accscale_std.x(), options.imunoise.accscale_std.y(),
                options.imunoise.accscale_std.z()};
    }
    // IMU初始误差转换为标准单位
    // convert initial imu errors' units to standard units
    for (int i = 0; i < 3; i++) {
        options.initstate_std.imuerror.gyrbias[i]  = vec1[i] * D2R / 3600.0;
        options.initstate_std.imuerror.accbias[i]  = vec2[i] * 1e-5;
        options.initstate_std.imuerror.gyrscale[i] = vec3[i] * 1e-6;
        options.initstate_std.imuerror.accscale[i] = vec4[i] * 1e-6;
    }

    // IMU噪声参数转换为标准单位
    // convert imu noise parameters' units to standard units
    options.imunoise.gyr_arw *= (D2R / 60.0);
    options.imunoise.acc_vrw /= 60.0;
    options.imunoise.gyrbias_std *= (D2R / 3600.0);
    options.imunoise.accbias_std *= 1e-5;
    options.imunoise.gyrscale_std *= 1e-6;
    options.imunoise.accscale_std *= 1e-6;
    options.imunoise.corr_time *= 3600;

    // GNSS天线杆臂, GNSS天线相位中心在IMU坐标系下位置
    // gnss antenna leverarm, position of GNSS antenna phase center in IMU frame
    try {
        vec1 = config["antlever"].as<std::vector<double>>();
    } catch (YAML::Exception &exception) {
        std::cout << "Failed when loading configuration. Please check antenna leverarm!" << std::endl;
        return false;
    }
    options.antlever = Eigen::Vector3d(vec1.data());

    return true;
}

/**
 * @brief 保存导航结果和IMU误差，已转换为常用单位
 *        save navigation result and imu error, converted them to common units
 * */
void writeNavResult(double time, NavState &navstate, FileSaver &navfile, FileSaver &imuerrfile) {

    std::vector<double> result;

    // 保存导航结果
    // save navigation result
    result.clear();
    result.push_back(0);
    result.push_back(time);
    result.push_back(navstate.pos[0] * R2D);
    result.push_back(navstate.pos[1] * R2D);
    result.push_back(navstate.pos[2]);
    result.push_back(navstate.vel[0]);
    result.push_back(navstate.vel[1]);
    result.push_back(navstate.vel[2]);
    result.push_back(navstate.euler[0] * R2D);
    result.push_back(navstate.euler[1] * R2D);
    result.push_back(navstate.euler[2] * R2D);
    navfile.dump(result);

    // 保存IMU误差
    // save IMU error
    auto imuerr = navstate.imuerror;
    result.clear();
    result.push_back(time);
    result.push_back(imuerr.gyrbias[0] * R2D * 3600);
    result.push_back(imuerr.gyrbias[1] * R2D * 3600);
    result.push_back(imuerr.gyrbias[2] * R2D * 3600);
    result.push_back(imuerr.accbias[0] * 1e5);
    result.push_back(imuerr.accbias[1] * 1e5);
    result.push_back(imuerr.accbias[2] * 1e5);
    result.push_back(imuerr.gyrscale[0] * 1e6);
    result.push_back(imuerr.gyrscale[1] * 1e6);
    result.push_back(imuerr.gyrscale[2] * 1e6);
    result.push_back(imuerr.accscale[0] * 1e6);
    result.push_back(imuerr.accscale[1] * 1e6);
    result.push_back(imuerr.accscale[2] * 1e6);
    imuerrfile.dump(result);
}

/**
 * @brief 保存标准差，已转换为常用单位
 *        save standard deviation, converted to common units
 * */
void writeSTD(double time, Eigen::MatrixXd &cov, FileSaver &stdfile) {

    std::vector<double> result;

    result.clear();
    result.push_back(time);
    // 保存位置、速度、姿态标准差
    // save position, velocity and attitude std
    for (int i = 0; i < 6; i++) {
        result.push_back(sqrt(cov(i, i)));
    }
    for (int i = 6; i < 9; i++) {
        result.push_back(sqrt(cov(i, i)) * R2D);
    }

    // 保存IMU误差标准差
    // save imu error std
    for (int i = 9; i < 12; i++) {
        result.push_back(sqrt(cov(i, i)) * R2D * 3600);
    }
    for (int i = 12; i < 15; i++) {
        result.push_back(sqrt(cov(i, i)) * 1e5);
    }
    for (int i = 15; i < 21; i++) {
        result.push_back(sqrt(cov(i, i)) * 1e6);
    }
    stdfile.dump(result);
}

void MainWindow::on_GINS_Output_clicked()
{
    QString GINS_Output_Path = QFileDialog::getExistingDirectory(this, tr("Select Path"));

    if(GINS_Output_Path.isEmpty())
    {
        QMessageBox::warning(this,"警告","请选择一个GINS路径");
    }
    else
    {
        this->ui->OutPut_lineEdit_3->setText(GINS_Output_Path);
        GINS_Output_path = GINS_Output_Path.toStdString();
    }
}

void MainWindow::on_GNSS_Input_clicked()
{
    QString GNSS_Input_Path = QFileDialog::getOpenFileName(this,"请选择一个GNSS文件",QCoreApplication::applicationFilePath());
    if(GNSS_Input_Path.isEmpty())
    {
        QMessageBox::warning(this,"警告","请选择一个GNSS文件");
    }
    else
    {
        this->ui->GNSS_lineEdit->setText(GNSS_Input_Path);
        GNSS_Input_path = GNSS_Input_Path.toStdString();
    }

}

void MainWindow::on_INS_Input_clicked()
{
    QString INS_Input_Path = QFileDialog::getOpenFileName(this,"请选择一个GNSS文件",QCoreApplication::applicationFilePath());
    if(INS_Input_Path.isEmpty())
    {
        QMessageBox::warning(this,"警告","请选择一个INS文件");
    }
    else
    {
        this->ui->INS_lineEdit_2->setText(INS_Input_Path);
        INS_Input_path = INS_Input_Path.toStdString();
    }
}

void MainWindow::on_GINS_Close_clicked()
{
    this->close();
}

void MainWindow::on_GINS_Config_clicked()
{
    Config_View *config_view = new Config_View();
    config_view->show();
}

void MainWindow::on_GINS_Plot_clicked()
{
    Draw_result_view *draw_result = new Draw_result_view();
    draw_result->show();
}

void MainWindow::Information_action()
{
    ui->textBrowser->clear();
    appendColoredText(this->ui->textBrowser, "该软件是武汉大学i2Nav团队开源了一套基于扩展卡尔曼滤波的GNSS/INS组合导航系统(EKF-Based GNSS/INS Integrated", Qt::black);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    appendColoredText(this->ui->textBrowser, "Navigation System, KF-GINS)算法代码,这套代码结合团队已公开视频课程讲义[1, 2],可作", Qt::black);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    appendColoredText(this->ui->textBrowser, "为基础的GNSS/INS组合算法研究平台，KF-GINS采用C++语言编写，各功能相对独立，可拓展能力",Qt::black);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);
    appendColoredText(this->ui->textBrowser, "较强。KF-GINS已上传到GitHub，可通过以下链接获取:https://github.com/i2Nav-WHU/KF-GINS", Qt::black);
    appendColoredText(this->ui->textBrowser, "<br>", Qt::black);

}

void MainWindow::Exit_action()
{
    this->close();
}

void MainWindow::Draw_result()
{
    Draw_result_view *draw_result = new Draw_result_view();
    draw_result->show();
}

void MainWindow::Draw_error()
{
    Draw_error_view *draw_error = new Draw_error_view();
    draw_error->show();
}
