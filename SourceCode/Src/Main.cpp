#include "CommonAPI.h"
#include<stdio.h>
#include "LessonX.h"
#include<string.h>
#include<time.h>
#include<stdlib.h>

//tankwar2.0

//注意：
//1：碰撞函数循环整型变量还未用的x之后
//2.碰撞比较时,strstr包含一类，strcmp针对单独个体,不要使用memcmo





//坦克信息结构体
typedef struct tank{
char *name;//精灵名字
float PosX;
float PosY;//精灵的坐标
int State = 1;//状态：1：活着，2：死亡
int Direction;//朝向
char *EnemyBullet[100];//记录敌方坦克的子弹名字
int EnemyBulletNum = 0;//计数敌方坦克子弹
char *explode;//储存坦克爆炸动画的名字接口
}TANK;

//函数
void IEnemyDie(const char* szname);//处理坦克的死亡，及不可碰撞，不发射子弹，不可见
void WallExplode(const char* szname,int n);//墙被摧毁爆炸。参数：爆炸墙的名字,第n块墙
void CopyExplode(const char* szname,int number);//复制的敌方坦克爆炸。参数：1：爆炸坦克，2：敌方坦克编号
void Explode(const char* szname,int type);//坦克爆炸动画。参数：1：爆炸精灵，2：类型：1为我方坦克，2为敌方初始坦克，3为敌方复制坦克
void EnemyMove(const char* szName);//敌方坦克的运动,参数：精灵名字
void CreatEnemyTanks(int j);//产生敌方坦克
void OnBulletColOther(const char* Bullet,const char* Other);
void MyOnFire(const char *szname,int n);//我方坦克开火。参数:开火的精灵：mytank，第n发子弹。
void IEnemyOnFire(const char *szname,int n);//地方初始坦克开火.参数:开火的精灵：enemy，第n发子弹
void EnemyOnFire(const char *szname,int Number,int n);//敌方坦克开火。参数：开火的精灵：enemy结构体数组，敌方坦克编号，第n发子弹
void ClearMap(void);//，游戏结束，清除地图
void LoadMap();//载入地图
void OnMyTankColOther(const char* mytank,const char* other);
void differentnum(int a[],int m,int n,int k);//产生不同的几个随机数,参数:储存数组，范围上下限n，m，产生k个


//道具
void toolbomp(void);//道具：爆炸
void tooltimer(void);//道具：减少游戏时间
void toolstar(void);//道具：射速提高
void toolcreat(int s);//道具产生

//子弹全局变量
char *MyBullet[100];//记录我方坦克子弹的名字
char *IEnemyBullet[100];//记录敌方初始坦克的子弹名字
int IEnemyBulletNum = 0;//计数敌方初始坦克的子弹数量
int MyBulletNum=0;//计数我方坦克子弹
float EnemyBulletSpeed = 20.f;//敌方子弹速度
float MyBulletSpeed = 20.f;//我方子弹速度


//敌方坦克生成的有关变量
int j=1;//记录产生敌方坦克的批次
int TotalEnemyNum = 1;//记录敌方坦克总数,初始有一个
float CreatLeftTime = 0.f;//每批次生成坦克的剩余时间
float RotateLeftTime = 0.f;//敌方坦克转向剩余时间
float OnFireLeftTime = 0.f;//敌方坦克开火剩余时间
float MyFireLeftTime = 0.f;//我方坦克离下次开炮的剩余时间
float ToolCreatLeftTime = 0.f;//道具出现剩余时间
float GameLeftTime = 0.f;//一局游戏的剩余时间

//游戏设置处
const int EnemyNum = 3;//每次生成敌方坦克的数量
const float CreatPeriod = 10.f;//每批次坦克产生的时间周期
const float RotateTime = 2.f;//敌方坦克转向的时间
const float OnFirePeriod = 3.f;//敌方坦克开火的周期
const float MyFirePeriod = 0.5;//我方坦克开炮的时间间隔
const float ToolCreatTime = 10.f;//道具出现时间周期
const float GameTime = 60.f;//一局游戏的时间
float velociyt = 5.f;//所有的坦克运动时的速度


//初始化坦克结构体数据
TANK mytank;//我方坦克
TANK Ienemy;//敌方初始坦克
TANK enemy[100];//地方复制坦克

//地图全局变量
int WallCount = 0;//记录砖墙的数量，因为游戏结束要卸载地图
int MetalWallCount = 0;//记录铁墙的数量，因为游戏结束要卸载地图
int Map[11][13];//地图数组
bool Load = true;//用于判断是否载入地图
char *WallName[1000];//记录砖墙的名字
char *MetalWallName[1000];//记录铁墙的名字
int differenttoolnum[3];//记录3个不同的随机数，用于每个道具随机产生一次

//标识游戏开始与结束的全局变量
int Start = 0;//控制游戏的状态，0为未开始，1为游戏开始，2为游戏结束,3为游戏胜利(时间到）
int Score = 0;//得分

//我方坦克的速度
float SpeedUp = 0.f;
float SpeedDown = 0.f;
float SpeedLeft = 0.f;
float SpeedRight = 0.f;

// 主函数入口
int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	// 初始化游戏引擎
	if( !dInitGameEngine( hInstance, lpCmdLine ) )
		return 0;

	// To do : 在此使用API更改窗口标题
	dSetWindowTitle("Tank War");

	// 引擎主循环，处理屏幕图像刷新等工作
	while( dEngineMainLoop() )
	{
		// 获取两次调用之间的时间差，传递给游戏逻辑处理
		float	fTimeDelta	=	dGetTimeDelta();
        GameLeftTime-=fTimeDelta;//记录时间

    if(Start==1)//游戏开始
    {
        if(GameLeftTime>0.f)//一局游戏进行中
        {
            dSetSpriteCollisionReceive("bullet",1);//设置子弹的碰撞属性
            //添加游戏进行的代码

            //实时计算时间
            OnFireLeftTime -= fTimeDelta;
            CreatLeftTime -= fTimeDelta;
            RotateLeftTime -= fTimeDelta;
            MyFireLeftTime -= fTimeDelta;
            ToolCreatLeftTime -= fTimeDelta;

            //加载出敌方坦克
            if(CreatLeftTime<=0.f)
            {
                CreatEnemyTanks(j);//产生的第j轮
                j++;
                CreatLeftTime = CreatPeriod;//重置产生敌方坦克的剩余时间
            }

            //产生道具
            differentnum(differenttoolnum,1,3,3);
            int w = 0;
            while(ToolCreatLeftTime<0.f)
            {
                toolcreat(differenttoolnum[w]);
                ToolCreatLeftTime = ToolCreatTime;
                w++;
            }

            //移动敌方坦克
            if(RotateLeftTime<=0.f)
                {
                    for(int b=1;b<j;b++)
                    {
                        for(int i=0;i<EnemyNum;i++)
                        {
                            EnemyMove(enemy[10*b+i].name);//移动复制的敌方坦克
                            EnemyMove("enemy");//移动敌方坦克模板
                            RotateLeftTime = RotateTime;//重置转向剩余时间
                        }
                    }
                }

                //敌方坦克开火
            if(OnFireLeftTime<=0.f)
                {
                    for(int c=1;c<j;c++)//敌方复制坦克开火
                    {
                        for(int d=0;d<EnemyNum;d++)
                        {
                            EnemyOnFire(enemy[10*c+d].name,10*c+d,enemy[10*c+d].EnemyBulletNum);//复制的敌方坦克开火
                            enemy[10*c+d].EnemyBulletNum++;
                        }
                    }

                    //模板敌方坦克开火
                    if(Ienemy.State==1)
                    {
                        IEnemyOnFire("enemy",IEnemyBulletNum);
                        IEnemyBulletNum++;
                    }

                    //重置开火时间
                    OnFireLeftTime = OnFirePeriod;
                }

            //不断实时显示游戏数据
            dSetTextValue("time",(int)GameLeftTime);
            dSetTextValue("score",Score);
            dSetTextValue("enemyNum",TotalEnemyNum);
            dSetTextValue("score",Score);

            if(Load)//载入地图，保证只载入一次地图
            {
                LoadMap();
                Load=false;
            }
        }
        else//时间到，一局游戏结束
        {
            Start = 3;
            ClearMap();//清楚地图
            dSetSpriteVisible("victory",true);
            dPlaySound("win",1,1);
        }
    }
    else if(Start==2) //游戏结束后处理
    {
        ClearMap();//清除地图
        dSetSpriteVisible("gameover",true);
    }
		// 执行游戏主循环
		GameMainLoop( fTimeDelta );
	};

	// 关闭游戏引擎
	dShutdownGameEngine();
	return 0;
}

// 引擎捕捉鼠标移动消息后，将调用到本函数
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void dOnMouseMove( const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseMove(fMouseX, fMouseY );
}

// 引擎捕捉鼠标点击消息后，将调用到本函数
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseClick(iMouseType, fMouseX, fMouseY);

}

// 引擎捕捉鼠标弹起消息后，将调用到本函数
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}

// 引擎捕捉键盘按下消息后，将调用到本函数
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
//
void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress )
{
    //游戏开始键入
    if(iKey==KEY_SPACE&&Start==0)//仅当游戏未开始且按下空格执行
    {
        Start = 1;
        GameLeftTime = GameTime;
    }
    //我方坦克的操作
    switch(iKey)
    {
    case KEY_W:
        SpeedUp = -velociyt;
        dSetSpriteRotation("mytank",0);
        break;
    case KEY_S:
        SpeedDown = velociyt;
        dSetSpriteRotation("mytank",180);
        break;
    case KEY_A:
        SpeedLeft = -velociyt;
        dSetSpriteRotation("mytank",270);
        break;
    case KEY_D:
        SpeedRight = velociyt;
        dSetSpriteRotation("mytank",90);
        break;
    }
    dSetSpriteLinearVelocity("mytank",SpeedLeft+SpeedRight,SpeedUp+SpeedDown);
    if((SpeedLeft+SpeedRight)>0)//翻转
        dSetSpriteFlipX("mytank",false);
    else if((SpeedUp+SpeedDown)<0)
        dSetSpriteFlipX("mytank",true);

    //键入开火：J
    if(iKey==KEY_J&&Start==1&&MyFireLeftTime<=0)
    {
        MyOnFire("mytank",MyBulletNum);
        MyBulletNum++;
        MyFireLeftTime = MyFirePeriod;//重置时间
    }
	// 可以在此添加游戏需要的响应函数
	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);
}

// 引擎捕捉键盘弹起消息后，将调用到本函数
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
//
void dOnKeyUp( const int iKey )
{
    switch(iKey)
    {
    case KEY_W:
        SpeedUp = 0.f;
        break;
    case KEY_S:
        SpeedDown = 0.f;
        break;
    case KEY_A:
        SpeedLeft = 0.f;
        break;
    case KEY_D:
        SpeedRight = 0.f;
        break;
    }
    dSetSpriteLinearVelocity("mytank",SpeedLeft+SpeedRight,SpeedUp+SpeedDown);
	// 可以在此添加游戏需要的响应函数
	OnKeyUp(iKey);
}


// 引擎捕捉到精灵与精灵碰撞之后，调用此函数
// 精灵之间要产生碰撞，必须在编辑器或者代码里设置精灵发送及接受碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
//注：参数精灵包含了所有发起碰撞和被碰撞的精灵
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
    //我方坦克
    if(strcmp(szSrcName,"mytank")==0)//用于确定处理对象
    {
        //与砖墙相碰的情况
        if(strstr(szTarName,"wall")!=NULL)
        dSetSpriteLinearVelocity(szSrcName,0.f,0.f);

        //与敌方坦克相碰的情况,游戏结束
        if(strstr(szTarName,"enemy")!=NULL)
        Start = 2;

        //与道具bomp相碰
        if(strstr(szTarName,dMakeSpriteName("bomp",0))!=NULL)
        {
            toolbomp();
            dDeleteSprite(dMakeSpriteName("bomp",0));
            dPlaySound("tool",0,1);//播放道具音乐
        }

        //与道具timer相碰
        if(strstr(szTarName,dMakeSpriteName("timer",0))!=NULL)
            {
                dDeleteSprite(dMakeSpriteName("timer",0));
                dPlaySound("tool",0,1);//播放道具音乐
                tooltimer();
            }

        //与道具star相碰
        if(strstr(szTarName,dMakeSpriteName("star",0))!=NULL)
            {
                dDeleteSprite(dMakeSpriteName("star",0));
                dPlaySound("tool",0,1);//播放道具音乐
                toolstar();
            }
    }


    //复制出的我方子弹
    for(int n=0;n<MyBulletNum;n++)
    {
        if(strcmp(szSrcName,MyBullet[n])==0)
    {
        //子弹与复制出的墙碰撞
        for(int c=0;c<WallCount;c++)
        {
            if((strcmp(szTarName,WallName[c])==0))
            {
                dDeleteSprite(MyBullet[n]);
                dDeleteSprite(WallName[c]);
                WallExplode(WallName[c],c);
                dPlaySound("wallbroken",0,1);//播放墙爆炸音乐
            }
        }

        //子弹与复制出的铁墙碰撞
        for(int y=0;y<MetalWallCount;y++)
        {
            if((strcmp(szTarName,MetalWallName[y])==0))
            {
                dDeleteSprite(MyBullet[n]);
                dPlaySound("beatmetal",0,1);//播放打铁音乐
            }
        }

        //子弹与模板铁墙相碰
        if((strcmp(szTarName,"wall_metal")==0))
        {
            dDeleteSprite(MyBullet[n]);
            dPlaySound("beatmetal",0,1);//播放打铁音乐
        }

        //子弹与模板墙相碰
        if((strcmp(szTarName,"wall")==0))
        {
            dDeleteSprite(MyBullet[n]);
            dDeleteSprite("wall");
            WallExplode("wall",100);//100为初始墙的编号
            dPlaySound("wallbroken",0,1);//播放墙爆炸音乐
        }

        //子弹与复制的敌方坦克相碰
        for(int d=1;d<j;d++)
        {
            for(int e=0;e<EnemyNum;e++)
            {
                if((strcmp(szTarName,enemy[10*d+e].name)==0))
                {
                    dDeleteSprite(MyBullet[n]);
                    dDeleteSprite(enemy[10*d+e].name);
                    CopyExplode(enemy[10*d+e].name,10*d+e);
                    TotalEnemyNum--;
                    Score++;
                    dPlaySound("kaipao",0,1);//播放爆炸音乐
                }
            }
        }

        //子弹与敌方初始坦克相碰
        if((strcmp(szTarName,"enemy")==0))
            {
                dDeleteSprite(MyBullet[n]);
                Explode("enemy",2);
                IEnemyDie("enemy");//标记为死亡
                TotalEnemyNum--;
                Score++;
                dPlaySound("kaipao",0,1);//播放爆炸音乐
            }

        //子弹与指挥部相碰,游戏结束
        if((strcmp(szTarName,"myboss"))==0)
        {
            Start = 2;
        }
    }
    }

    //敌方初始坦克子弹
    for(int l=0;l<IEnemyBulletNum;l++)
    {
        if(strcmp(szSrcName,IEnemyBullet[l])==0)
    {
        //子弹与复制出的墙碰撞
        for(int o=0;o<WallCount;o++)
        {
            if((strcmp(szTarName,WallName[o])==0))
            {
                dDeleteSprite(IEnemyBullet[l]);
                dDeleteSprite(WallName[o]);
                WallExplode(WallName[o],o);
                dPlaySound("wallbroken",0,1);//播放墙爆炸音乐
            }
        }

        //子弹与复制出的铁墙碰撞
        for(int x=0;x<MetalWallCount;x++)
        {
            if((strcmp(szTarName,MetalWallName[x])==0))
            {
                dDeleteSprite(IEnemyBullet[l]);
                dPlaySound("beatmetal",0,1);//播放打铁音乐
            }
        }

        //子弹与我方坦克子弹相碰
        for (int u = 0;u<MyBulletNum;u++)
        {
            if(strcmp(szTarName,MyBullet[u])==0)
            {
                dDeleteSprite(IEnemyBullet[l]);
                dDeleteSprite(MyBullet[u]);
            }
        }

        //子弹与模板砖墙相碰
        if((strcmp(szTarName,"wall")==0))
        {
            dDeleteSprite(IEnemyBullet[l]);
            dDeleteSprite("wall");
            WallExplode("wall",100);
        }

        //子弹与模板铁墙相碰
        if((strcmp(szTarName,"wall_metal")==0))
        {
            dDeleteSprite(IEnemyBullet[l]);
            dPlaySound("beatmetal",0,1);//播放打铁音乐
        }

        //子弹与敌方复制坦克相碰
        for(int p=1;p<j;p++)
        {
            for(int q=0;q<EnemyNum;q++)
            {
                if((strcmp(szTarName,enemy[10*p+q].name)==0))
                {
                    dDeleteSprite(IEnemyBullet[l]);
                }
            }
        }

        //子弹与指挥部相碰,游戏结束
        if((strcmp(szTarName,"myboss"))==0)
        {
            Start = 2;
        }
    }
    }

    //复制出的敌方坦克的子弹
    for(int f=1;f<j;f++)//遍历所有敌方复制坦克
    {
        for(int g=0;g<EnemyNum;g++)
        {
            for(int h=0;h<enemy[10*f+g].EnemyBulletNum;h++)
            {
                if(strcmp(szSrcName,enemy[10*f+g].EnemyBullet[h])==0)//确定了敌方坦克的目标子弹,编号为(10*f+g)敌方坦克的第h颗子弹
                    {
                        //子弹与复制出的墙碰撞
                        for(int k=0;k<WallCount;k++)//遍历每一块墙
                            {
                                if((strcmp(szTarName,WallName[k])==0))
                                {
                                    dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                                    dDeleteSprite(WallName[k]);
                                    WallExplode(WallName[k],k);
                                    dPlaySound("wallbroken",0,1);//播放墙爆炸音乐
                                }
                            }

                            //子弹与模板墙碰撞
                            if(strcmp(szTarName,"wall")==0)
                            {
                                dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                                dDeleteSprite("wall");
                                WallExplode("wall",100);
                                dPlaySound("wallbroken",0,1);//播放墙爆炸音乐
                            }

                            //子弹与模板铁墙相碰
                            if((strcmp(szTarName,"wall_metal")==0))
                            {
                                dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                                dPlaySound("beatmetal",0,1);//播放打铁音乐
                            }

                             //子弹与复制出的铁墙碰撞
                            for(int z=0;z<MetalWallCount;z++)
                            {
                                if((strcmp(szTarName,MetalWallName[z])==0))
                                {
                                    dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                                    dPlaySound("beatmetal",0,1);//播放打铁音乐
                                }
                            }

                        //子弹与除自身的其他敌方复制坦克碰撞
                            for(int r=1;r<j;r++)
                            {
                                for(int s=0;s<EnemyNum;s++)
                                {
                                    if((strcmp(szTarName,enemy[10*r+s].name)==0)&&(strcmp(szTarName,enemy[10*f+g].name)!=0))//不击中自身
                                    {
                                        //dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);   敌方坦克之间子弹可以穿过
                                    }
                                }
                            }

                        //子弹与敌方初始坦克相碰
                        if(strcmp(szTarName,"enemy")==0)
                        {
                            //dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                        }

                        //子弹与我方坦克相碰
                        if(strcmp(szTarName,"mytank")==0)
                        {
                            Start = 2;
                            Explode("mytank",1);
                        }

                        //子弹与我方指挥部相碰
                        if(strcmp(szTarName,"myboss")==0)
                        {
                            Start = 2;
                        }

                        //子弹与我方坦克子弹相碰
                        for(int v =0;v<MyBulletNum;v++)
                        {
                            if(strcmp(szTarName,MyBullet[v])==0)
                            {
                                dDeleteSprite(MyBullet[v]);
                                dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                            }
                        }
                    }
            }
        }
    }

    //复制的敌方坦克
    for(int b=1;b<j;b++)//遍历所有复制的坦克
    {
        for(int i=0;i<EnemyNum;i++)
        {
            if(strcmp(szSrcName,enemy[10*b+i].name)==0)//用于确定处理对象,编号为10*b+i
            {

            //与墙相碰的情况
            if(strstr(szTarName,"wall")!=NULL)
            {
                dSetSpriteLinearVelocity(szSrcName,0.f,0.f);
            }

            //与敌方坦克相碰
            if(strstr(szTarName,"enemy")!=NULL)
            {
                dSetSpriteLinearVelocity(szSrcName,0.f,0.f);
            }

            //与我方坦克相碰
            if(strcmp(szTarName,"mytank")==0)
            {
                Start = 2;
                Explode("mytank",1);
            }

            //与我方指挥部相碰
            if(strcmp(szTarName,"myboss")==0)
            {
                Start = 2;
            }
            }
        }
    }

    //敌方初始坦克
    if(strcmp(szSrcName,"enemy")==0)//用于确定处理对象
        {
        //与墙相碰的情况
        if(strstr(szTarName,"wall")!=NULL)
        {
            dSetSpriteLinearVelocity(szSrcName,0.f,0.f);
        }

        //与敌方的复制坦克相碰
        for(int c=1;c<j;c++)
        {
            for(int d=0;d<EnemyNum;d++)
            {
                if(strcmp(szTarName,enemy[10*c+d].name)==0)
                {
                    dSetSpriteLinearVelocity(szSrcName,0.f,0.f);
                }
            }
        }
        }

	// 可以在此添加游戏需要的响应函数
	OnSpriteColSprite(szSrcName, szTarName);
}


// 引擎捕捉到精灵与世界边界碰撞之后，调用此函数.
// 精灵之间要产生碰撞，必须在编辑器或者代码里设置精灵的世界边界限制
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	// 可以在此添加游戏需要的响应函数
	OnSpriteColWorldLimit(szName, iColSide);
}

void LoadMap()//地图载入
{
    int i,j,x,y;
    int Map[11][13]=
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,1,0,1,0,1,0,1,0},
        {0,2,0,2,0,1,0,1,0,2,0,2,0},
        {0,1,0,2,0,2,1,2,0,1,0,1,0},
        {0,1,0,1,0,1,0,1,0,1,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,2,1,0,0,0,0,0},
        {0,1,0,2,0,0,0,0,0,2,0,1,0},
        {0,1,0,2,0,0,0,0,0,2,0,1,0},
        {0,1,0,1,0,1,1,1,0,1,0,1,0},
        {0,0,0,0,0,1,0,1,0,0,0,0,0},
    };
    for(i=0;i<11;i++)
    {
        for(j=0;j<13;j++)
        {
            if(Map[i][j]==1)//砖墙的名字从0开始，1是砖墙的位置
            {
                char *CopyWall = dMakeSpriteName("wall",WallCount);
                WallName[WallCount]=dMakeSpriteName("wall",WallCount);
                dCloneSprite("wall",CopyWall);
                WallCount++;//计数墙的数量
                x=-24+4*j;
                y=-20+4*i;
                dSetSpritePosition("wall",x,y);
            }
             if(Map[i][j]==2)//铁墙的名字从0开始，2是铁墙的位置
            {
                char *CopyWall = dMakeSpriteName("wall_metal",MetalWallCount);
                MetalWallName[MetalWallCount]=dMakeSpriteName("wall_metal",MetalWallCount);
                dCloneSprite("wall_metal",CopyWall);
                MetalWallCount++;//计数墙的数量
                x=-24+4*j;
                y=-20+4*i;
                dSetSpritePosition("wall_metal",x,y);
            }
        }
    }
    //初始化固定图标
    dSetSpriteVisible("myboss",true);
    dSetSpriteVisible("start",false);
    dSetSpriteVisible("splash",false);
    dSetSpriteVisible("mytank",true);
    dSetSpriteVisible("enemy",true);
    dSetSpriteVisible("tips",false);
    dPlaySound("music",1,1);
}

void ClearMap(void)//，游戏结束，清除地图
{
    int i,j,s,t;
    s = WallCount++;//记录铁墙的数目
    t = MetalWallCount++;//记录铁墙的数目
    dSetSpriteVisible("mytank",false);//删除我的坦克
    dSetSpriteVisible("myboss",false);//删除我方指挥部
    dSetSpriteVisible("enemy",false);//删除敌方
    dSetSpriteVisible("wall",false);
    dSetSpriteVisible(dMakeSpriteName("star",0),false);
    dSetSpriteVisible(dMakeSpriteName("bomp",0),false);
    dSetSpriteVisible(dMakeSpriteName("timer",0),false);
    dSetSpriteVisible("star",false);
    dSetSpriteVisible("bomp",false);
    dSetSpriteVisible("timer",false);

    //删除砖墙
    for(i=1;i<s;i++)
    {

        dSetSpriteVisible((dMakeSpriteName("wall",i)),false);
    }

    //删除铁墙
    for(j=1;j<t;j++)
    {

        dSetSpriteVisible((dMakeSpriteName("wall_metal",j)),false);
        dSetSpriteVisible("wall_metal",false);
    }
}

//参数:开火的精灵：mytank,,第n颗子弹
void MyOnFire(const char *szname,int n)//我方坦克开火
{
            //获取精灵的位置和朝向
            mytank.PosX=dGetSpritePositionX(szname);
            mytank.PosY=dGetSpritePositionY(szname);
            mytank.Direction=dGetSpriteRotation(szname);

            //复制子弹
            dCloneSprite("bullet",dMakeSpriteName("mybullet",n));
            MyBullet[n]=dMakeSpriteName("mybullet",n);//记录保存名字
            dSetSpriteVisible(MyBullet[n],true);//保证子弹可见

            dSetSpritePosition(MyBullet[n],mytank.PosX,mytank.PosY);//设置子弹的位置
            dSetSpriteLinearVelocityPolar(MyBullet[n],MyBulletSpeed,(mytank.Direction+270));//设置子弹发射方向,由于图片的方向问题，角度加270°
            dSetSpriteRotation(MyBullet[n],mytank.Direction);//设置子弹图片的朝向
}

//参数:开火的精灵：enemy,第n颗子弹,状态
void IEnemyOnFire(const char *szname,int n)//敌方初始坦克开火
{
            //获取精灵的位置和朝向
        Ienemy.PosX=dGetSpritePositionX(szname);
        Ienemy.PosY=dGetSpritePositionY(szname);
        Ienemy.Direction=dGetSpriteRotation(szname);

        //复制子弹
        dCloneSprite("bullet",dMakeSpriteName("ienemybullet",n));
        IEnemyBullet[n]=dMakeSpriteName("ienemybullet",n);//记录保存名字
        dSetSpriteVisible(IEnemyBullet[n],true);//保证子弹可见

        dSetSpritePosition(IEnemyBullet[n],Ienemy.PosX,Ienemy.PosY);//设置子弹的位置
        dSetSpriteLinearVelocityPolar(IEnemyBullet[n],EnemyBulletSpeed,(Ienemy.Direction+270));//设置子弹发射方向,由于图片的方向问题，角度加270°
        dSetSpriteRotation(IEnemyBullet[n],Ienemy.Direction);//设置子弹图片的朝向
}


    //参数，敌方复制坦克名字，敌方坦克角标编号，第n颗子弹
void EnemyOnFire(const char *szname,int Number,int n)//敌方坦克开火
  {
        //获取精灵的位置和朝向
        enemy[Number].PosX=dGetSpritePositionX(szname);
        enemy[Number].PosY=dGetSpritePositionY(szname);
        enemy[Number].Direction=dGetSpriteRotation(szname);

        //复制子弹
        dCloneSprite("bullet",dMakeSpriteName("enemybullet",n));
        enemy[Number].EnemyBullet[n]=dMakeSpriteName("enemybullet",n);//记录保存名字
        dSetSpriteVisible(enemy[Number].EnemyBullet[n],true);//保证子弹可见

        dSetSpritePosition(enemy[Number].EnemyBullet[n],enemy[Number].PosX,enemy[Number].PosY);//设置子弹的位置
        dSetSpriteLinearVelocityPolar(enemy[Number].EnemyBullet[n],EnemyBulletSpeed,(enemy[Number].Direction+270));//设置子弹发射方向,由于图片的方向问题，角度加270°
        dSetSpriteRotation(enemy[Number].EnemyBullet[n],enemy[Number].Direction);//设置子弹图片的朝向
  }


void CreatEnemyTanks(int j)//产生敌方坦克,参数：第j批次产生
{
    for(int i=0;i<EnemyNum;i++)//每批次生成的敌方坦克
    {
        enemy[10*j+i].name = dMakeSpriteName("enemy",10*j+i);//用作记录敌方复制坦克的下标 ji
        dCloneSprite("enemy",enemy[10*j+i].name);

        //保证复制出的坦克可碰撞，可见的属性
        dSetSpriteVisible(enemy[10*j+i].name,true);
        dSetSpriteCollisionActive(enemy[10*j+i].name,true,true);

        TotalEnemyNum++;//用于初始面板记录
        enemy[10*j+i].PosX = dRandomRange(-24*4,24*4)/4;//需要为4的倍数

        int a = rand()%2+1;//随机出现在两行
        if(a==1)
        {
            enemy[10*j+i].PosY = 0.f;
        }
        else
        {
            enemy[10*j+i].PosY = -22.f;
        }

        dSetSpritePosition(enemy[10*j+i].name,enemy[10*j+i].PosX,enemy[10*j+i].PosY);//部署敌方坦克位置
        dSetSpriteVisible(enemy[10*j+i].name,true);//保证复制出来的坦克可见
    }
}

void EnemyMove(const char* szName)//敌方坦克的运动,参数：精灵名字
{
    int s = rand()%4+1;//产生4个随机数代表四个方向
    switch(s)
    {
    case 1:
        dSetSpriteLinearVelocityPolar(szName,velociyt,0);
        dSetSpriteRotation(szName,90);
        break;
    case 2:
        dSetSpriteLinearVelocityPolar(szName,velociyt,90);
        dSetSpriteRotation(szName,180);
        break;
    case 3:
        dSetSpriteLinearVelocityPolar(szName,velociyt,180);
        dSetSpriteRotation(szName,270);
        break;
    case 4:
        dSetSpriteLinearVelocityPolar(szName,velociyt,270);
        dSetSpriteRotation(szName,0);
        break;
    }
}

//坦克爆炸动画。参数：1：爆炸精灵，2：类型：1为我方坦克，2为敌方初始坦克，3为敌方复制坦克
void Explode(const char* szname,int type)
{
    if(type == 1)//我方坦克
    {
        //获取精灵的位置和朝向
        mytank.PosX=dGetSpritePositionX(szname);
        mytank.PosY=dGetSpritePositionY(szname);
        dCloneSprite("explode","myexplode");

        //设置爆炸位置
        dSetSpritePosition("myexplode",mytank.PosX,mytank.PosY);
        dSetSpriteLifeTime("myexplode",0.5);//存活时长为0.5秒

    }

    if(type ==2)//敌方初始坦克
    {
        //获取精灵的位置和朝向
        Ienemy.PosX=dGetSpritePositionX(szname);
        Ienemy.PosY=dGetSpritePositionY(szname);
        dCloneSprite("explode","Ienemyexplode");

        //设置爆炸位置
        dSetSpritePosition("Ienemyexplode",Ienemy.PosX,Ienemy.PosY);
        dSetSpriteLifeTime("Ienemyexplode",0.5);//存活时长为0.5秒
    }

}

//复制的敌方坦克爆炸。参数：1：爆炸坦克，2：敌方坦克编号
void CopyExplode(const char* szname,int number)
{
        //获取精灵的位置和朝向
        enemy[number].PosX=dGetSpritePositionX(szname);
        enemy[number].PosY=dGetSpritePositionY(szname);
        enemy[number].explode=dMakeSpriteName("enemyexplode",number);//记录名字
        dCloneSprite("explode",enemy[number].explode);

         //设置爆炸位置
        dSetSpritePosition(enemy[number].explode,enemy[number].PosX,enemy[number].PosY);
        dSetSpriteLifeTime(enemy[number].explode,0.5);//存活时长为0.5秒
}

//砖墙被摧毁爆炸。参数：爆炸墙的名字,第n块墙 100为初始墙的编号
void WallExplode(const char* szname,int n)
{
    float x,y;
    if(n=100)//处理模板墙   ??:“==”不可以而“=”可以
    {
        //记录墙的坐标
        x = dGetSpritePositionX(szname);
        y = dGetSpritePositionY(szname);
        dCloneSprite("wallexplode",dMakeSpriteName("wallexplode",n));

        //设置爆炸位置
        dSetSpritePosition(dMakeSpriteName("wallexplode",n),x,y);
        dSetSpriteLifeTime(dMakeSpriteName("wallexplode",n),0.5);//存活时间为0.5秒
    }

    else
    {

        x = dGetSpritePositionX(szname);
        y = dGetSpritePositionY(szname);
        dCloneSprite("wallexplode",dMakeSpriteName("wallexplode",n));
    }
}

//处理坦克的死亡，及不可碰撞，不发射子弹，不可见
void IEnemyDie(const char* szname)
{
    dSetSpriteVisible(szname,false);
    dSetSpriteCollisionActive(szname,false,false);
    Ienemy.State = 2;//状态为死亡
}

void toolbomp(void)//道具：爆炸
{
    //初始坦克死亡
    Explode("enemy",2);
    IEnemyDie("enemy");//标记为死亡
    TotalEnemyNum--;
    Score++;
    dPlaySound("kaipao",0,1);//播放爆炸音乐

    //现存坦克的死亡
    for(int i=1;i<j;i++)
    {
        for(int k=0;k<EnemyNum;k++)
            {
                dDeleteSprite(enemy[10*i+k].name);
                CopyExplode(enemy[10*i+k].name,10*i+k);//播放爆炸动画
                dPlaySound("kaipao",0,1);//播放爆炸音乐
                Score++;
            }
    }
}

void tooltimer(void)//道具：减少游戏时间
{
        GameLeftTime-=10;//减少游戏时间
}

void toolstar(void)//道具：射速提高
{
        MyBulletSpeed = 50.f;
}

void toolcreat(int s)//道具产生,参数：产生的第s号工具
{
    int i,j,x,y;
    int Map[11][13]=
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,1,1,0,1,0,1,0},
        {0,1,0,1,0,1,0,1,0,1,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,0,0,0,0,0},
        {0,1,0,1,0,0,0,0,0,1,0,1,0},
        {0,1,0,1,0,0,0,0,0,1,0,1,0},
        {0,1,0,1,0,1,1,1,0,1,0,1,0},
        {0,0,0,0,0,1,0,1,0,0,0,0,0},
    };

    while(Map[i=dRandomRange(2,10)][j=dRandomRange(2,12)]==0)//为0的不存在墙，不包括边缘，避免道具出现在墙外
    {
                x=-24+4*j;
                y=-20+4*i;
                char *copytimer;
                char *copystar;
                char *copybomp;
                switch(s)
                {
                case 1:
                    copytimer=dMakeSpriteName("timer",0);
                    dCloneSprite("timer",copytimer);
                    dSetSpritePosition(copytimer,x,y);
                    break;
                case 2:
                    copystar=dMakeSpriteName("star",0);
                    dCloneSprite("star",copystar);
                    dSetSpritePosition(copystar,x,y);
                    break;
                case 3:
                    copybomp=dMakeSpriteName("bomp",0);
                    dCloneSprite("bomp",copybomp);
                    dSetSpritePosition(copybomp,x,y);
                    break;
            }
            break;
    }
}

void differentnum(int a[],int m,int n,int k)//产生不同的几个随机数,参数:储存数组，范围上下限n，m，产生k个
{
    int i,j,t;
    for(i = 0; i < k; i++)
    {
        t = rand()%(n-m+1)+m;
        for(j = 0; j < i; j ++)
            if(a[j] == t)
                break;
            if(j == i)//不重复
                a[i] = t;//记录随机数。
    }
}
