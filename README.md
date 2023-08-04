本文作者感谢武汉大学卫星导航定位技术研究中心多源智能导航实验室\(i2Nav\)牛小骥教授团队开源的KF\-GINS软件平台。

The authors would like to acknowledge the team of Prof\. Xiaoji Niu of the Integrated and Intelligent Navigation \(i2Nav\) group from GNSS Research Center of Wuhan University for providing the open\-source KF\-GINS software that was used in the paper\.

# 简介

本项目以“基于扩展卡尔曼滤波的GNSS/INS组合导航软件（KF\-GINS）”为核心，为KF\-GINS编写了GUI系统，方便使用者在Windows操作系统下进行使用。编写语言为C\+\+版本Qt，使用MSVC2017进行编译。

该程序由文件输入与输出、参数配置、解算核心、结果绘图、解算过程显示五个模块构成。目前可以实现基本的解算工作，参数配置与结果绘图模块目前尚未完成编写。

# 编写日志

2023\.7\.29

1. 完成编写GUI基本框架和相关子页面。
2. 增添了文件输入方式，可直接手动指定文件导入、输出路径。
3. 增添了解算进度条，方便读取解算进度。
4. 增添了手动输入参数的功能（仅编写页面，功能未编写）。
5. 增添了解算后直接进行绘图的功能（仅编写页面，功能未编写）。
6. 增添了错误信息与解算信息显示功能。
7. 增添了输出标准pos文件的功能。

# 编译方法

此项目使用Windows下的QtCreater构建，因使用已编译的DLL库构建，目前仅支持MSVC编译器在Release x64的环境下进行编译，暂不支持Linux下进行编译，若需要其他版本，请编译在.pro文件内更改相应的库文件的路径

# 使用方法

注：由于目前手动输入参数的功能未开发完毕，因此仍然需要借助yaml文件配置解算的初始参数，因此在解算之前需要指定“yaml”、“GNSS”、“INS”、“Output”四个路径才可进行解算。且软件本身位置和指定的文件路径中不能含有中文。

## 导入四个文件的位置

![github](https://github.com/Wang-Jie-Lucid-Sheep/KF_GINS_GUI/blob/main/example/input_config.png)

![github](https://github.com/Wang-Jie-Lucid-Sheep/KF_GINS_GUI/blob/main/example/Input_Path.png)

## 点击解算按钮即可进行解算

![github](https://github.com/Wang-Jie-Lucid-Sheep/KF_GINS_GUI/blob/main/example/Caculate.png)

解算过程中，进度条会显示当前解算进度，解算完成后，解算结果文件会自动保存在选定的输出路径下。

![github](https://github.com/Wang-Jie-Lucid-Sheep/KF_GINS_GUI/blob/main/example/Caculate_Progress.png)

如果需要将定位文件输出为标准的pos文件，可通过“保存POS”文件进行输出。

![github](https://github.com/Wang-Jie-Lucid-Sheep/KF_GINS_GUI/blob/main/example/output_pos.png)

之后可以配合RTKLIB软件输出kml格式，并在谷歌地球中进行显示。

![github](https://github.com/Wang-Jie-Lucid-Sheep/KF_GINS_GUI/blob/main/example/RTKLIB_kml.png)

![github](https://github.com/Wang-Jie-Lucid-Sheep/KF_GINS_GUI/blob/main/example/Google_Earth.png)

