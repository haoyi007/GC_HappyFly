#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <QThread>
#include <QLayout>
#include "qcustomplot.h"

#define VARCOUNT 10

union RegAddr_type
{
   unsigned char c[1600];
   float fval[400];
   int word[400];
   struct RegAddr_name
   {//符号	物理变量名称	宏标记	字节长度	数据类型（字长）	读写状态	默认值	数值范围（单位）	                           地址	H地址
//	unsigned int TimeCounts;
//	unsigned int Delay_us;
    unsigned char ControlMode; //  飞行器控制模式 0:保护状态  1：有操纵增稳 2：姿态控	3: 轨迹控制（位置模式)  4: 轨迹控制（速度模式)
    unsigned char FlightState;   // 飞行工作状态  0自检模式；1 停机 2：起飞模式 3：自主飞行  4：返程 5：降落模式 6：降落完成
    unsigned char bSensorState; // 各传感器工作状态，按位显示 1正常 0不正常  bit_0: IMU  bit_1 GPS bit_2:激光高度	 bit_3 接收机
    unsigned char isTakeoff; // 0:停机 1：离地
    unsigned char HandState; // 0-10  机械手状态  0：收起 1：松臂中，2：空闲 3：待抓 4:抓桶成功 5 抓桶失败 6 放桶  7: 收臂
    unsigned char  FlowStatus; // 任务流程
    unsigned char RobostStatue;//  全自主状态  0:手动  1：自主控制
    unsigned char  WorkMode;	// 工作模式  0 保护停机  1:	遥控调试 2： 自主模式

    unsigned short HandPwm_H[10];//(4) // 遥控器输入PWM信号 高电平 us长度
    short 	 IMU_R_xyz[3];  //(24) IMU 角速率 	放大 1000倍    		 rad
    short 	 IMU_A_xyz[3];  //(30) IMU 加速度 	放大 1000倍    		 g
    short 	 ATI_xyz[3];  //  姿态角 滚转 俯仰角 偏航 	放大 1000倍  rad
    short	 Veloc[3];	// 放大100倍	 北东地速度
    short	 uvw[3];	   // 放大100倍	 前右下速度
    short	 Magnet[3];// 磁航向值
    unsigned short OutPwm_H[12];//(60)	  // 舵机高电平信号值

    short Temp[3];   //(84) 放大100倍 飞控盒温度 发动机缸1温度 发动机缸 2 温度
    unsigned short Votage[2]; //(90) 放大100倍 系统供电电池电压 舵机电池电压
    float UltraHeiht; //(m) GPS起飞高度
    float FlyingHeiht; //m 离地高度
    //unsigned short HandHeight; //(m ) 放大1000倍机械手高度
    unsigned short RotorSpeed; //rpm  放大 10倍 旋翼转速

    float PressHeight;     // (m) 气压高度
    float ControlValue[5]; // 操纵量(ms):总距/横向/纵向/偏航/油门

  float Height_Control_obj; // m 预定控制高度
    float Height_Control_cur;  // m 当前飞行高度

    short ATI_obj[3]; //  期望 姿态角 滚转 俯仰角 偏航 	放大 1000倍  rad
    short Rate_obj[3]; //   期望 角速率
    short Heading_obj; 				//航向角速率目标值

    double Pos[3]; // 当前飞行器经纬度	(经度  纬度    高度)

    double Pos_Origin[10][2]; // 预设位置 (经度 纬度  )  0:起飞(原点)
    double Pos_Rang[4][2]; 	 // 边界位置点 (经度 纬度  )
    float POS_Move[2]; // 飞机起飞点移动位移 米 [北 东] 向

    unsigned char ImgState; // 图像辨识 状态 0：无数据 1 ：目标丢失 2 图像定位中
    unsigned char ImgMode; // 上电运行成功 2: 上电无图像
    short	ImgDist[3];    // short 单位 mm  范围 ±327675mm
    short	ImgVeloc[2];   //short 单位 mm/s 范围 ±327675mm/s
    short	ImgHeading;	   //相对角度	单位 ° (*0.1)  范围 ±90°(-90~90)
    unsigned char BucketNum; // 看到捅的数量
    unsigned char ImgSpare;	 //对齐预留

    unsigned int	GpsSol_iTow; // ms :GPS millisecond Time of Week
    short GpsSol_Week ;// GPS week(GPS time)
    unsigned char GpsSol_gpsFix;  // Gpsfix Type  0x00 No fix; 0x01: dead reckoning only  0x02: 2D-fix; 0x03: 3D-fix;0x04:GPS+dead reckoning combined
    unsigned char GpsSol_Flags;  // fix Status flags: 0x01:GPSfixOK;0x02:DiffSoln(is DGPS used); 0x04: WKNSET(is Week Number valid); 0x08:TOWSET(is Time of Weed valid);
    int GpsSol_ecefX; // cm;  ECEF X coordinate
    int GpsSol_ecefY; // cm;  ECEF Y coordinate
    int GpsSol_ecefZ; // cm;  ECEF Z coordinate
    unsigned int GpsSol_pAcc;  // cm; 3D position Accuracy Estimate
    int	GpsSol_ecefVX; // cm/s  ECEF X velocity
    int	GpsSol_ecefVY; // cm/s  ECEF Y velocity
    int	GpsSol_ecefVZ; // cm/s  ECEF Z velocity
    unsigned int GpsSol_sAcc; // cm/s Speed Accuracy Estimate

    unsigned short GpsSol_pDOP; // 0.01 Postition DOP
    unsigned char  GpsSol_numSV; // Number of Svs used in navigation solution;
  char SetPailID;				//	 设置捅的编号
    unsigned int CurPailID;			   // 对齐 预留


    float outOutLoopControlLaw[3];	// 速度回路输出期望 俯仰和操纵角
    short gSetVeloc_cmps_B[3];  // 机体下期望速度
    float PosError_N[2];   // 地理坐标系系位置偏差
    float PosError_B[2];   // 载体系下位置偏差

    float Horg[3]; // 观察数据

    // 抓取区的位置
  float GPS2_Height;			// 动平台的海拔高度
    double GPS2_Pos[2];	//	 移动平台的经纬度	(经度  纬度）
    short  GPS2_Veloc[3];	// 放大100倍	 移动平台的北东地速度
    unsigned short GPS2_GpsSol_pDOP; // 0.01  移动平台的Postition DOP
    unsigned char  GPS2_GpsSol_numSV; // 移动平台的Number of Svs used in navigation solution;
    unsigned char GPS2_GpsSol_Flags;   // 移动平台的fix Status flags: 0x01:GPSfixOK;0x02:DiffSoln(is DGPS used); 0x04: WKNSET(is Week Number valid); 0x08:TOWSET(is Time of Weed valid);

    // 投放区的位置
    float GPS3_Height;			// 动平台的海拔高度
    double GPS3_Pos[2];	//	 移动平台的经纬度	(经度  纬度）
    short  GPS3_Veloc[3];	// 放大100倍	 移动平台的北东地速度
    unsigned short GPS3_GpsSol_pDOP; // 0.01  移动平台的Postition DOP
    unsigned char  GPS3_GpsSol_numSV; // 移动平台的Number of Svs used in navigation solution;
    unsigned char GPS3_GpsSol_Flags;   // 移动平台的fix Status flags: 0x01:GPSfixOK;0x02:DiffSoln(is DGPS used); 0x04: WKNSET(is Week Number valid); 0x08:TOWSET(is Time o


    unsigned short PowerVotage;					//放大100倍 动力电池电压
    float Systime;

    //控制器实时调参所需参数
    //内回路滚转通道
    float Lat_Kp_Stab_Rate;						 //内回路 滚转通道 角速率 增稳 比例参数
    float Lat_Ki_Stab_Rate;						 //内回路 滚转通道 角速率 增稳 积分参数
    float Lat_Kd_Stab_Rate;						 //内回路 滚转通道 角速率 增稳 微分参数
    float Lat_Range_Stab_abs;					 //内回路 滚转通道 角速率 增稳 输出范围
    float Lat_Kp_Stab_Atti;						 //内回路 滚转通道 角度 增稳 比例参数
    float Lat_Range_Stab_Atti_abs;				 //内回路 滚转通道 角度 增稳 输出范围
    float Lat_Kp_Atti;							 //内回路 滚转通道 姿态 比例参数
    float Lat_Ki_Atti;							 //内回路 滚转通道 姿态 积分参数
    float Lat_Kd_Atti;							 //内回路 滚转通道 姿态 微分参数
    float Lat_Range_Atti_abs;					 //内回路 滚转通道 姿态 输出范围
    //内回路俯仰通道
    float Lon_Kp_Stab_Rate;						 //内回路 俯仰通道 角速率 增稳 比例参数
    float Lon_Ki_Stab_Rate;						 //内回路 俯仰通道 角速率 增稳 积分参数
    float Lon_Kd_Stab_Rate;						 //内回路 俯仰通道 角速率 增稳 微分参数
    float Lon_Range_Stab_abs;					 //内回路 俯仰通道 角速率 增稳 输出范围
    float Lon_Kp_Stab_Atti;						 //内回路 俯仰通道 角度 增稳 比例参数
    float Lon_Range_Stab_Atti_abs;		//内回路 俯仰通道 角度 增稳 输出范围
    float Lon_Kp_Atti;							 //内回路 俯仰通道 姿态 比例参数
    float Lon_Ki_Atti;							 //内回路 俯仰通道 姿态 积分参数
    float Lon_Kd_Atti;							 //内回路 俯仰通道 姿态 微分参数
    float Lon_Range_Atti_abs;					 //内回路 俯仰通道 姿态 输出范围
    //内回路偏航通道
    float Rud_Kp_Stab_Rate;						 //内回路 偏航通道 角速率 增稳 比例参数
    float Rud_Ki_Stab_Rate;						 //内回路 偏航通道 角速率 增稳 积分参数
    float Rud_Kd_Stab_Rate;						 //内回路 偏航通道 角速率 增稳 微分参数
    float Rud_Range_Stab_abs;					 //内回路 偏航通道 角速率 增稳 输出范围
    float Rud_Kp_Stab_Atti;						 //内回路 偏航通道 角度 增稳 比例参数
    float Rud_Range_Stab_Atti_abs;				 //内回路 偏航通道 角度 增稳 输出范围
    float Rud_Kp_Atti;							 //内回路 偏航通道 姿态 比例参数
    float Rud_Ki_Atti;							 //内回路 偏航通道 姿态 积分参数
    float Rud_Kd_Atti;							 //内回路 偏航通道 姿态 微分参数
    float Rud_Range_Atti_abs;					 //内回路 偏航通道 姿态 输出范围
    //内回路高度通道
    float Hgt_Kp_Stab_Rate;						 //外回路 高度通道 角速率 增稳 比例参数
    float Hgt_Ki_Stab_Rate;						 //外回路 高度通道 角速率 增稳 积分参数
    float Hgt_Kd_Stab_Rate;						 //外回路 高度通道 角速率 增稳 微分参数
    float Hgt_Range_Stab_abs;					 //外回路 高度通道 角速率 增稳 输出范围
    float Hgt_Kp_Stab_Atti;						 //外回路 高度通道 角度 增稳 比例参数
    float Hgt_Range_Stab_Atti_abs;				 //外回路 高度通道 角度 增稳 输出范围
    float Hgt_Kp_Atti;							 //外回路 高度通道 姿态 比例参数
    float Hgt_Ki_Atti;							 //外回路 高度通道 姿态 积分参数
    float Hgt_Kd_Atti;							 //外回路 高度通道 姿态 微分参数
    float Hgt_Range_Atti_abs;					 //外回路 高度通道 姿态 输出范围

    //外回路前向位置通道
    float PosX_Kp;								 //外回路 前向位置 比例参数
    float PosX_Ki;								 //外回路 前向位置 积分参数
    float PosX_Kd;								 //外回路 前向位置 微分参数
    float PosX_Kd_d; // 二次微分系数			 //外回路 前向位置 二次微分系数
    float PosX_fStab;							 //外回路 前向位置 阻尼系数
    float PosX_Limit_Output;					 //外回路 前向位置 输出范围
    float PosX_Limit_Integral;					 //外回路 前向位置 积分范围
    float PosX_Limit_Derivat;					 //外回路 前向位置 微分范围
    float PosX_Error_Death;						 //外回路 前向位置 死区大小
    float PosX_Limit_DDerivate;					 //外回路 前向位置 二次微分系数 输出范围
    //外回路横向位置通道
    float PosY_Kp;								 //外回路 横向位置 比例参数
    float PosY_Ki;								 //外回路 横向位置 积分参数
    float PosY_Kd;								 //外回路 横向位置 微分参数
    float PosY_Kd_d; // 二次微分系数			 //外回路 横向位置 二次微分系数
    float PosY_fStab;							 //外回路 横向向位置 阻尼系数
    float PosY_Limit_Output;					 //外回路 横向位置 输出范围
    float PosY_Limit_Integral;					 //外回路 横向位置 积分范围
    float PosY_Limit_Derivat;					 //外回路 横向位置 微分范围
    float PosY_Error_Death;						 //外回路 横向位置 死区大小
    float PosY_Limit_DDerivate;					 //外回路 横向位置 二次微分系数 输出范围

    //外回路前向速度通道
    float VelocX_Kp;							  //外回路 前向速度	比例参数
    float VelocX_Ki;							  //外回路 前向速度 积分参数
    float VelocX_Kd;							  //外回路 前向速度 微分参数
    float VelocX_Kd_d;							  //外回路 前向速度 二次微分系数
    float VelocX_fStab;							  //外回路 前向速度 阻尼系数
    float VelocX_Limit_Output;					  //外回路 前向速度 输出范围
    float VelocX_Limit_Integral;				  //外回路 前向速度 积分范围
    float VelocX_Limit_Derivat;					  //外回路 前向速度 微分范围
    float VelocX_Error_Death;					  //外回路 前向速度 死区大小
    float VelocX_Limit_DDerivate;				  //外回路 前向速度 二次微分系数 输出范围
    //外回路横向速度通道
    float VelocY_Kp;							  //外回路 横向速度	比例参数
    float VelocY_Ki;							  //外回路 横向速度 积分参数
    float VelocY_Kd;							  //外回路 横向速度 微分参数
    float VelocY_Kd_d;							  //外回路 横向速度 二次微分系数
    float VelocY_fStab;							  //外回路 横向速度 阻尼系数
    float VelocY_Limit_Output;					  //外回路 横向速度 输出范围
    float VelocY_Limit_Integral;				  //外回路 横向速度 积分范围
    float VelocY_Limit_Derivat;					  //外回路 横向速度 微分范围
    float VelocY_Error_Death;					  //外回路 横向速度 死区大小
    float VelocY_Limit_DDerivate;				  //外回路 横向速度 二次微分系数 输出范围

//外回路横向速度通道
    float VelocZ_Kp;							  //外回路 横向速度	比例参数
    float VelocZ_Ki;							  //外回路 横向速度 积分参数
    float VelocZ_Kd;							  //外回路 横向速度 微分参数
    float VelocZ_Kd_d;							  //外回路 横向速度 二次微分系数
    float VelocZ_fStab;							  //外回路 横向速度 阻尼系数
    float VelocZ_Limit_Output;					  //外回路 横向速度 输出范围
    float VelocZ_Limit_Integral;				  //外回路 横向速度 积分范围
    float VelocZ_Limit_Derivat;					  //外回路 横向速度 微分范围
    float VelocZ_Error_Death;					  //外回路 横向速度 死区大小
    float VelocZ_Limit_DDerivate;				  //外回路 横向速度 二次微分系数 输出范围

    float Head_SetValue;			  //地面站设置航向角增量	单位 度
    //补充
    float TurnAngle_1;				//协调转弯角度
    float HeadingBaseErrror;		//二航向角基准差值
    float Headorg[3]; 				// 航向测量单元原始数据，0单元为惯性航向，1单元为磁航向，2为二航向角差值

    float PosN_RefXY[2];      //地轴系下起飞点到目标点的XY位移
    float PosN_ObjXY[2];      //地轴系下软化后的位置


    float PosN_RefH[2];					// m 地轴系下高度目标值-Z
    float PosN_OriginXY[10][2]; 		// m 桶位与平台测点之间的位置 (X、Y轴向)
    double Pos2_Origin[2]; 				// 移动平台测点初始位置 (经度 纬度 )  (原点)


    short Magnet_MinMax[3][2];    // 地磁三轴上的最小最大
    short Maganet_Correct[3];     // 矫正后的地磁强度;
     int HandStrain[2]; // 机械手应变值
    unsigned char HandStatue; // 机械手状态
    unsigned char HandWeightState;  // 是否钩桶
     int HandWeightStrain;  //机械手重量

    //外回路位置速度通道
    float Veloc_PosX_Kp;							  //外回路 横向速度	比例参数
    float Veloc_PosX_Ki;							  //外回路 横向速度 积分参数
    float Veloc_PosX_Kd;							  //外回路 横向速度 微分参数
    float Veloc_PosX_Kd_d;							  //外回路 横向速度 二次微分系数
    float Veloc_PosX_fStab;							  //外回路 横向速度 阻尼系数
    float Veloc_PosX_Limit_Output;					  //外回路 横向速度 输出范围
    float Veloc_PosX_Limit_Integral;				  //外回路 横向速度 积分范围
    float Veloc_PosX_Limit_Derivat;					  //外回路 横向速度 微分范围
    float Veloc_PosX_Error_Death;					  //外回路 横向速度 死区大小
    float Veloc_PosX_Limit_DDerivate;				  //外回路 横向速度 二次微分系数 输出范围


        //外回路位置速度通道
    float Veloc_PosY_Kp;							  //外回路 横向速度	比例参数
    float Veloc_PosY_Ki;							  //外回路 横向速度 积分参数
    float Veloc_PosY_Kd;							  //外回路 横向速度 微分参数
    float Veloc_PosY_Kd_d;							  //外回路 横向速度 二次微分系数
    float Veloc_PosY_fStab;							  //外回路 横向速度 阻尼系数
    float Veloc_PosY_Limit_Output;					  //外回路 横向速度 输出范围
    float Veloc_PosY_Limit_Integral;				  //外回路 横向速度 积分范围
    float Veloc_PosY_Limit_Derivat;					  //外回路 横向速度 微分范围
    float Veloc_PosY_Error_Death;					  //外回路 横向速度 死区大小
    float Veloc_PosY_Limit_DDerivate;				  //外回路 横向速度 二次微分系数 输出范围

    unsigned char Pail_State[8];        // 8个捅设置状
    }name;

};

union mem
{
    char c[8];
    short s;
    int i;
    float f;
    double d;
};

class DataProcessor:public QThread
{
    Q_OBJECT
public:
    DataProcessor();
    ~DataProcessor();

    void fileRead(QString, int offset = 0);

    void setOffset(int offset);

    int getFileindex();

    void stop();
    void dataStart();

    void getName();

    void addData(int);

    double getData(int);

    void setData(int index, double val);

    void HAOYI_Solve_Data_Receive_Anl(QByteArray data_buf,quint8 num);

signals:
    void datashow(QByteArray);
    void sendATIOk(float roll,float pitch,float yaw,float current_Yaw);
    void HAOYI_Solve_Data(QByteArray,quint8);
    void parserdata(QByteArray);

private:

    int fileIndex;
    int dataCount;
    int fileSize;


    QTimer * dataTime;
    int key;

};

#endif // DATAPROCESSOR_H
