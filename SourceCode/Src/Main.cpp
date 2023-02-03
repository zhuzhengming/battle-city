#include "CommonAPI.h"
#include<stdio.h>
#include "LessonX.h"
#include<string.h>
#include<time.h>
#include<stdlib.h>

//tankwar2.0

//ע�⣺
//1����ײ����ѭ�����ͱ�����δ�õ�x֮��
//2.��ײ�Ƚ�ʱ,strstr����һ�࣬strcmp��Ե�������,��Ҫʹ��memcmo





//̹����Ϣ�ṹ��
typedef struct tank{
char *name;//��������
float PosX;
float PosY;//���������
int State = 1;//״̬��1�����ţ�2������
int Direction;//����
char *EnemyBullet[100];//��¼�з�̹�˵��ӵ�����
int EnemyBulletNum = 0;//�����з�̹���ӵ�
char *explode;//����̹�˱�ը���������ֽӿ�
}TANK;

//����
void IEnemyDie(const char* szname);//����̹�˵���������������ײ���������ӵ������ɼ�
void WallExplode(const char* szname,int n);//ǽ���ݻٱ�ը����������ըǽ������,��n��ǽ
void CopyExplode(const char* szname,int number);//���Ƶĵз�̹�˱�ը��������1����ը̹�ˣ�2���з�̹�˱��
void Explode(const char* szname,int type);//̹�˱�ը������������1����ը���飬2�����ͣ�1Ϊ�ҷ�̹�ˣ�2Ϊ�з���ʼ̹�ˣ�3Ϊ�з�����̹��
void EnemyMove(const char* szName);//�з�̹�˵��˶�,��������������
void CreatEnemyTanks(int j);//�����з�̹��
void OnBulletColOther(const char* Bullet,const char* Other);
void MyOnFire(const char *szname,int n);//�ҷ�̹�˿��𡣲���:����ľ��飺mytank����n���ӵ���
void IEnemyOnFire(const char *szname,int n);//�ط���ʼ̹�˿���.����:����ľ��飺enemy����n���ӵ�
void EnemyOnFire(const char *szname,int Number,int n);//�з�̹�˿��𡣲���������ľ��飺enemy�ṹ�����飬�з�̹�˱�ţ���n���ӵ�
void ClearMap(void);//����Ϸ�����������ͼ
void LoadMap();//�����ͼ
void OnMyTankColOther(const char* mytank,const char* other);
void differentnum(int a[],int m,int n,int k);//������ͬ�ļ��������,����:�������飬��Χ������n��m������k��


//����
void toolbomp(void);//���ߣ���ը
void tooltimer(void);//���ߣ�������Ϸʱ��
void toolstar(void);//���ߣ��������
void toolcreat(int s);//���߲���

//�ӵ�ȫ�ֱ���
char *MyBullet[100];//��¼�ҷ�̹���ӵ�������
char *IEnemyBullet[100];//��¼�з���ʼ̹�˵��ӵ�����
int IEnemyBulletNum = 0;//�����з���ʼ̹�˵��ӵ�����
int MyBulletNum=0;//�����ҷ�̹���ӵ�
float EnemyBulletSpeed = 20.f;//�з��ӵ��ٶ�
float MyBulletSpeed = 20.f;//�ҷ��ӵ��ٶ�


//�з�̹�����ɵ��йر���
int j=1;//��¼�����з�̹�˵�����
int TotalEnemyNum = 1;//��¼�з�̹������,��ʼ��һ��
float CreatLeftTime = 0.f;//ÿ��������̹�˵�ʣ��ʱ��
float RotateLeftTime = 0.f;//�з�̹��ת��ʣ��ʱ��
float OnFireLeftTime = 0.f;//�з�̹�˿���ʣ��ʱ��
float MyFireLeftTime = 0.f;//�ҷ�̹�����´ο��ڵ�ʣ��ʱ��
float ToolCreatLeftTime = 0.f;//���߳���ʣ��ʱ��
float GameLeftTime = 0.f;//һ����Ϸ��ʣ��ʱ��

//��Ϸ���ô�
const int EnemyNum = 3;//ÿ�����ɵз�̹�˵�����
const float CreatPeriod = 10.f;//ÿ����̹�˲�����ʱ������
const float RotateTime = 2.f;//�з�̹��ת���ʱ��
const float OnFirePeriod = 3.f;//�з�̹�˿��������
const float MyFirePeriod = 0.5;//�ҷ�̹�˿��ڵ�ʱ����
const float ToolCreatTime = 10.f;//���߳���ʱ������
const float GameTime = 60.f;//һ����Ϸ��ʱ��
float velociyt = 5.f;//���е�̹���˶�ʱ���ٶ�


//��ʼ��̹�˽ṹ������
TANK mytank;//�ҷ�̹��
TANK Ienemy;//�з���ʼ̹��
TANK enemy[100];//�ط�����̹��

//��ͼȫ�ֱ���
int WallCount = 0;//��¼שǽ����������Ϊ��Ϸ����Ҫж�ص�ͼ
int MetalWallCount = 0;//��¼��ǽ����������Ϊ��Ϸ����Ҫж�ص�ͼ
int Map[11][13];//��ͼ����
bool Load = true;//�����ж��Ƿ������ͼ
char *WallName[1000];//��¼שǽ������
char *MetalWallName[1000];//��¼��ǽ������
int differenttoolnum[3];//��¼3����ͬ�������������ÿ�������������һ��

//��ʶ��Ϸ��ʼ�������ȫ�ֱ���
int Start = 0;//������Ϸ��״̬��0Ϊδ��ʼ��1Ϊ��Ϸ��ʼ��2Ϊ��Ϸ����,3Ϊ��Ϸʤ��(ʱ�䵽��
int Score = 0;//�÷�

//�ҷ�̹�˵��ٶ�
float SpeedUp = 0.f;
float SpeedDown = 0.f;
float SpeedLeft = 0.f;
float SpeedRight = 0.f;

// ���������
int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	// ��ʼ����Ϸ����
	if( !dInitGameEngine( hInstance, lpCmdLine ) )
		return 0;

	// To do : �ڴ�ʹ��API���Ĵ��ڱ���
	dSetWindowTitle("Tank War");

	// ������ѭ����������Ļͼ��ˢ�µȹ���
	while( dEngineMainLoop() )
	{
		// ��ȡ���ε���֮���ʱ�����ݸ���Ϸ�߼�����
		float	fTimeDelta	=	dGetTimeDelta();
        GameLeftTime-=fTimeDelta;//��¼ʱ��

    if(Start==1)//��Ϸ��ʼ
    {
        if(GameLeftTime>0.f)//һ����Ϸ������
        {
            dSetSpriteCollisionReceive("bullet",1);//�����ӵ�����ײ����
            //�����Ϸ���еĴ���

            //ʵʱ����ʱ��
            OnFireLeftTime -= fTimeDelta;
            CreatLeftTime -= fTimeDelta;
            RotateLeftTime -= fTimeDelta;
            MyFireLeftTime -= fTimeDelta;
            ToolCreatLeftTime -= fTimeDelta;

            //���س��з�̹��
            if(CreatLeftTime<=0.f)
            {
                CreatEnemyTanks(j);//�����ĵ�j��
                j++;
                CreatLeftTime = CreatPeriod;//���ò����з�̹�˵�ʣ��ʱ��
            }

            //��������
            differentnum(differenttoolnum,1,3,3);
            int w = 0;
            while(ToolCreatLeftTime<0.f)
            {
                toolcreat(differenttoolnum[w]);
                ToolCreatLeftTime = ToolCreatTime;
                w++;
            }

            //�ƶ��з�̹��
            if(RotateLeftTime<=0.f)
                {
                    for(int b=1;b<j;b++)
                    {
                        for(int i=0;i<EnemyNum;i++)
                        {
                            EnemyMove(enemy[10*b+i].name);//�ƶ����Ƶĵз�̹��
                            EnemyMove("enemy");//�ƶ��з�̹��ģ��
                            RotateLeftTime = RotateTime;//����ת��ʣ��ʱ��
                        }
                    }
                }

                //�з�̹�˿���
            if(OnFireLeftTime<=0.f)
                {
                    for(int c=1;c<j;c++)//�з�����̹�˿���
                    {
                        for(int d=0;d<EnemyNum;d++)
                        {
                            EnemyOnFire(enemy[10*c+d].name,10*c+d,enemy[10*c+d].EnemyBulletNum);//���Ƶĵз�̹�˿���
                            enemy[10*c+d].EnemyBulletNum++;
                        }
                    }

                    //ģ��з�̹�˿���
                    if(Ienemy.State==1)
                    {
                        IEnemyOnFire("enemy",IEnemyBulletNum);
                        IEnemyBulletNum++;
                    }

                    //���ÿ���ʱ��
                    OnFireLeftTime = OnFirePeriod;
                }

            //����ʵʱ��ʾ��Ϸ����
            dSetTextValue("time",(int)GameLeftTime);
            dSetTextValue("score",Score);
            dSetTextValue("enemyNum",TotalEnemyNum);
            dSetTextValue("score",Score);

            if(Load)//�����ͼ����ֻ֤����һ�ε�ͼ
            {
                LoadMap();
                Load=false;
            }
        }
        else//ʱ�䵽��һ����Ϸ����
        {
            Start = 3;
            ClearMap();//�����ͼ
            dSetSpriteVisible("victory",true);
            dPlaySound("win",1,1);
        }
    }
    else if(Start==2) //��Ϸ��������
    {
        ClearMap();//�����ͼ
        dSetSpriteVisible("gameover",true);
    }
		// ִ����Ϸ��ѭ��
		GameMainLoop( fTimeDelta );
	};

	// �ر���Ϸ����
	dShutdownGameEngine();
	return 0;
}

// ���沶׽����ƶ���Ϣ�󣬽����õ�������
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
void dOnMouseMove( const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseMove(fMouseX, fMouseY );
}

// ���沶׽�������Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseClick(iMouseType, fMouseX, fMouseY);

}

// ���沶׽��굯����Ϣ�󣬽����õ�������
// ���� iMouseType����갴��ֵ���� enum MouseTypes ����
// ���� fMouseX, fMouseY��Ϊ��굱ǰ����
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}

// ���沶׽���̰�����Ϣ�󣬽����õ�������
// ���� iKey�������µļ���ֵ�� enum KeyCodes �궨��
// ���� iAltPress, iShiftPress��iCtrlPress�������ϵĹ��ܼ�Alt��Ctrl��Shift��ǰ�Ƿ�Ҳ���ڰ���״̬(0δ���£�1����)
//
void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress )
{
    //��Ϸ��ʼ����
    if(iKey==KEY_SPACE&&Start==0)//������Ϸδ��ʼ�Ұ��¿ո�ִ��
    {
        Start = 1;
        GameLeftTime = GameTime;
    }
    //�ҷ�̹�˵Ĳ���
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
    if((SpeedLeft+SpeedRight)>0)//��ת
        dSetSpriteFlipX("mytank",false);
    else if((SpeedUp+SpeedDown)<0)
        dSetSpriteFlipX("mytank",true);

    //���뿪��J
    if(iKey==KEY_J&&Start==1&&MyFireLeftTime<=0)
    {
        MyOnFire("mytank",MyBulletNum);
        MyBulletNum++;
        MyFireLeftTime = MyFirePeriod;//����ʱ��
    }
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);
}

// ���沶׽���̵�����Ϣ�󣬽����õ�������
// ���� iKey������ļ���ֵ�� enum KeyCodes �궨��
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
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnKeyUp(iKey);
}


// ���沶׽�������뾫����ײ֮�󣬵��ô˺���
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��鷢�ͼ�������ײ
// ���� szSrcName��������ײ�ľ�������
// ���� szTarName������ײ�ľ�������
//ע������������������з�����ײ�ͱ���ײ�ľ���
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
    //�ҷ�̹��
    if(strcmp(szSrcName,"mytank")==0)//����ȷ���������
    {
        //��שǽ���������
        if(strstr(szTarName,"wall")!=NULL)
        dSetSpriteLinearVelocity(szSrcName,0.f,0.f);

        //��з�̹�����������,��Ϸ����
        if(strstr(szTarName,"enemy")!=NULL)
        Start = 2;

        //�����bomp����
        if(strstr(szTarName,dMakeSpriteName("bomp",0))!=NULL)
        {
            toolbomp();
            dDeleteSprite(dMakeSpriteName("bomp",0));
            dPlaySound("tool",0,1);//���ŵ�������
        }

        //�����timer����
        if(strstr(szTarName,dMakeSpriteName("timer",0))!=NULL)
            {
                dDeleteSprite(dMakeSpriteName("timer",0));
                dPlaySound("tool",0,1);//���ŵ�������
                tooltimer();
            }

        //�����star����
        if(strstr(szTarName,dMakeSpriteName("star",0))!=NULL)
            {
                dDeleteSprite(dMakeSpriteName("star",0));
                dPlaySound("tool",0,1);//���ŵ�������
                toolstar();
            }
    }


    //���Ƴ����ҷ��ӵ�
    for(int n=0;n<MyBulletNum;n++)
    {
        if(strcmp(szSrcName,MyBullet[n])==0)
    {
        //�ӵ��븴�Ƴ���ǽ��ײ
        for(int c=0;c<WallCount;c++)
        {
            if((strcmp(szTarName,WallName[c])==0))
            {
                dDeleteSprite(MyBullet[n]);
                dDeleteSprite(WallName[c]);
                WallExplode(WallName[c],c);
                dPlaySound("wallbroken",0,1);//����ǽ��ը����
            }
        }

        //�ӵ��븴�Ƴ�����ǽ��ײ
        for(int y=0;y<MetalWallCount;y++)
        {
            if((strcmp(szTarName,MetalWallName[y])==0))
            {
                dDeleteSprite(MyBullet[n]);
                dPlaySound("beatmetal",0,1);//���Ŵ�������
            }
        }

        //�ӵ���ģ����ǽ����
        if((strcmp(szTarName,"wall_metal")==0))
        {
            dDeleteSprite(MyBullet[n]);
            dPlaySound("beatmetal",0,1);//���Ŵ�������
        }

        //�ӵ���ģ��ǽ����
        if((strcmp(szTarName,"wall")==0))
        {
            dDeleteSprite(MyBullet[n]);
            dDeleteSprite("wall");
            WallExplode("wall",100);//100Ϊ��ʼǽ�ı��
            dPlaySound("wallbroken",0,1);//����ǽ��ը����
        }

        //�ӵ��븴�Ƶĵз�̹������
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
                    dPlaySound("kaipao",0,1);//���ű�ը����
                }
            }
        }

        //�ӵ���з���ʼ̹������
        if((strcmp(szTarName,"enemy")==0))
            {
                dDeleteSprite(MyBullet[n]);
                Explode("enemy",2);
                IEnemyDie("enemy");//���Ϊ����
                TotalEnemyNum--;
                Score++;
                dPlaySound("kaipao",0,1);//���ű�ը����
            }

        //�ӵ���ָ�Ӳ�����,��Ϸ����
        if((strcmp(szTarName,"myboss"))==0)
        {
            Start = 2;
        }
    }
    }

    //�з���ʼ̹���ӵ�
    for(int l=0;l<IEnemyBulletNum;l++)
    {
        if(strcmp(szSrcName,IEnemyBullet[l])==0)
    {
        //�ӵ��븴�Ƴ���ǽ��ײ
        for(int o=0;o<WallCount;o++)
        {
            if((strcmp(szTarName,WallName[o])==0))
            {
                dDeleteSprite(IEnemyBullet[l]);
                dDeleteSprite(WallName[o]);
                WallExplode(WallName[o],o);
                dPlaySound("wallbroken",0,1);//����ǽ��ը����
            }
        }

        //�ӵ��븴�Ƴ�����ǽ��ײ
        for(int x=0;x<MetalWallCount;x++)
        {
            if((strcmp(szTarName,MetalWallName[x])==0))
            {
                dDeleteSprite(IEnemyBullet[l]);
                dPlaySound("beatmetal",0,1);//���Ŵ�������
            }
        }

        //�ӵ����ҷ�̹���ӵ�����
        for (int u = 0;u<MyBulletNum;u++)
        {
            if(strcmp(szTarName,MyBullet[u])==0)
            {
                dDeleteSprite(IEnemyBullet[l]);
                dDeleteSprite(MyBullet[u]);
            }
        }

        //�ӵ���ģ��שǽ����
        if((strcmp(szTarName,"wall")==0))
        {
            dDeleteSprite(IEnemyBullet[l]);
            dDeleteSprite("wall");
            WallExplode("wall",100);
        }

        //�ӵ���ģ����ǽ����
        if((strcmp(szTarName,"wall_metal")==0))
        {
            dDeleteSprite(IEnemyBullet[l]);
            dPlaySound("beatmetal",0,1);//���Ŵ�������
        }

        //�ӵ���з�����̹������
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

        //�ӵ���ָ�Ӳ�����,��Ϸ����
        if((strcmp(szTarName,"myboss"))==0)
        {
            Start = 2;
        }
    }
    }

    //���Ƴ��ĵз�̹�˵��ӵ�
    for(int f=1;f<j;f++)//�������ез�����̹��
    {
        for(int g=0;g<EnemyNum;g++)
        {
            for(int h=0;h<enemy[10*f+g].EnemyBulletNum;h++)
            {
                if(strcmp(szSrcName,enemy[10*f+g].EnemyBullet[h])==0)//ȷ���˵з�̹�˵�Ŀ���ӵ�,���Ϊ(10*f+g)�з�̹�˵ĵ�h���ӵ�
                    {
                        //�ӵ��븴�Ƴ���ǽ��ײ
                        for(int k=0;k<WallCount;k++)//����ÿһ��ǽ
                            {
                                if((strcmp(szTarName,WallName[k])==0))
                                {
                                    dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                                    dDeleteSprite(WallName[k]);
                                    WallExplode(WallName[k],k);
                                    dPlaySound("wallbroken",0,1);//����ǽ��ը����
                                }
                            }

                            //�ӵ���ģ��ǽ��ײ
                            if(strcmp(szTarName,"wall")==0)
                            {
                                dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                                dDeleteSprite("wall");
                                WallExplode("wall",100);
                                dPlaySound("wallbroken",0,1);//����ǽ��ը����
                            }

                            //�ӵ���ģ����ǽ����
                            if((strcmp(szTarName,"wall_metal")==0))
                            {
                                dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                                dPlaySound("beatmetal",0,1);//���Ŵ�������
                            }

                             //�ӵ��븴�Ƴ�����ǽ��ײ
                            for(int z=0;z<MetalWallCount;z++)
                            {
                                if((strcmp(szTarName,MetalWallName[z])==0))
                                {
                                    dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                                    dPlaySound("beatmetal",0,1);//���Ŵ�������
                                }
                            }

                        //�ӵ��������������з�����̹����ײ
                            for(int r=1;r<j;r++)
                            {
                                for(int s=0;s<EnemyNum;s++)
                                {
                                    if((strcmp(szTarName,enemy[10*r+s].name)==0)&&(strcmp(szTarName,enemy[10*f+g].name)!=0))//����������
                                    {
                                        //dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);   �з�̹��֮���ӵ����Դ���
                                    }
                                }
                            }

                        //�ӵ���з���ʼ̹������
                        if(strcmp(szTarName,"enemy")==0)
                        {
                            //dDeleteSprite(enemy[10*f+g].EnemyBullet[h]);
                        }

                        //�ӵ����ҷ�̹������
                        if(strcmp(szTarName,"mytank")==0)
                        {
                            Start = 2;
                            Explode("mytank",1);
                        }

                        //�ӵ����ҷ�ָ�Ӳ�����
                        if(strcmp(szTarName,"myboss")==0)
                        {
                            Start = 2;
                        }

                        //�ӵ����ҷ�̹���ӵ�����
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

    //���Ƶĵз�̹��
    for(int b=1;b<j;b++)//�������и��Ƶ�̹��
    {
        for(int i=0;i<EnemyNum;i++)
        {
            if(strcmp(szSrcName,enemy[10*b+i].name)==0)//����ȷ���������,���Ϊ10*b+i
            {

            //��ǽ���������
            if(strstr(szTarName,"wall")!=NULL)
            {
                dSetSpriteLinearVelocity(szSrcName,0.f,0.f);
            }

            //��з�̹������
            if(strstr(szTarName,"enemy")!=NULL)
            {
                dSetSpriteLinearVelocity(szSrcName,0.f,0.f);
            }

            //���ҷ�̹������
            if(strcmp(szTarName,"mytank")==0)
            {
                Start = 2;
                Explode("mytank",1);
            }

            //���ҷ�ָ�Ӳ�����
            if(strcmp(szTarName,"myboss")==0)
            {
                Start = 2;
            }
            }
        }
    }

    //�з���ʼ̹��
    if(strcmp(szSrcName,"enemy")==0)//����ȷ���������
        {
        //��ǽ���������
        if(strstr(szTarName,"wall")!=NULL)
        {
            dSetSpriteLinearVelocity(szSrcName,0.f,0.f);
        }

        //��з��ĸ���̹������
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

	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnSpriteColSprite(szSrcName, szTarName);
}


// ���沶׽������������߽���ײ֮�󣬵��ô˺���.
// ����֮��Ҫ������ײ�������ڱ༭�����ߴ��������þ��������߽�����
// ���� szName����ײ���߽�ľ�������
// ���� iColSide����ײ���ı߽� 0 ��ߣ�1 �ұߣ�2 �ϱߣ�3 �±�
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	// �����ڴ������Ϸ��Ҫ����Ӧ����
	OnSpriteColWorldLimit(szName, iColSide);
}

void LoadMap()//��ͼ����
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
            if(Map[i][j]==1)//שǽ�����ִ�0��ʼ��1��שǽ��λ��
            {
                char *CopyWall = dMakeSpriteName("wall",WallCount);
                WallName[WallCount]=dMakeSpriteName("wall",WallCount);
                dCloneSprite("wall",CopyWall);
                WallCount++;//����ǽ������
                x=-24+4*j;
                y=-20+4*i;
                dSetSpritePosition("wall",x,y);
            }
             if(Map[i][j]==2)//��ǽ�����ִ�0��ʼ��2����ǽ��λ��
            {
                char *CopyWall = dMakeSpriteName("wall_metal",MetalWallCount);
                MetalWallName[MetalWallCount]=dMakeSpriteName("wall_metal",MetalWallCount);
                dCloneSprite("wall_metal",CopyWall);
                MetalWallCount++;//����ǽ������
                x=-24+4*j;
                y=-20+4*i;
                dSetSpritePosition("wall_metal",x,y);
            }
        }
    }
    //��ʼ���̶�ͼ��
    dSetSpriteVisible("myboss",true);
    dSetSpriteVisible("start",false);
    dSetSpriteVisible("splash",false);
    dSetSpriteVisible("mytank",true);
    dSetSpriteVisible("enemy",true);
    dSetSpriteVisible("tips",false);
    dPlaySound("music",1,1);
}

void ClearMap(void)//����Ϸ�����������ͼ
{
    int i,j,s,t;
    s = WallCount++;//��¼��ǽ����Ŀ
    t = MetalWallCount++;//��¼��ǽ����Ŀ
    dSetSpriteVisible("mytank",false);//ɾ���ҵ�̹��
    dSetSpriteVisible("myboss",false);//ɾ���ҷ�ָ�Ӳ�
    dSetSpriteVisible("enemy",false);//ɾ���з�
    dSetSpriteVisible("wall",false);
    dSetSpriteVisible(dMakeSpriteName("star",0),false);
    dSetSpriteVisible(dMakeSpriteName("bomp",0),false);
    dSetSpriteVisible(dMakeSpriteName("timer",0),false);
    dSetSpriteVisible("star",false);
    dSetSpriteVisible("bomp",false);
    dSetSpriteVisible("timer",false);

    //ɾ��שǽ
    for(i=1;i<s;i++)
    {

        dSetSpriteVisible((dMakeSpriteName("wall",i)),false);
    }

    //ɾ����ǽ
    for(j=1;j<t;j++)
    {

        dSetSpriteVisible((dMakeSpriteName("wall_metal",j)),false);
        dSetSpriteVisible("wall_metal",false);
    }
}

//����:����ľ��飺mytank,,��n���ӵ�
void MyOnFire(const char *szname,int n)//�ҷ�̹�˿���
{
            //��ȡ�����λ�úͳ���
            mytank.PosX=dGetSpritePositionX(szname);
            mytank.PosY=dGetSpritePositionY(szname);
            mytank.Direction=dGetSpriteRotation(szname);

            //�����ӵ�
            dCloneSprite("bullet",dMakeSpriteName("mybullet",n));
            MyBullet[n]=dMakeSpriteName("mybullet",n);//��¼��������
            dSetSpriteVisible(MyBullet[n],true);//��֤�ӵ��ɼ�

            dSetSpritePosition(MyBullet[n],mytank.PosX,mytank.PosY);//�����ӵ���λ��
            dSetSpriteLinearVelocityPolar(MyBullet[n],MyBulletSpeed,(mytank.Direction+270));//�����ӵ����䷽��,����ͼƬ�ķ������⣬�Ƕȼ�270��
            dSetSpriteRotation(MyBullet[n],mytank.Direction);//�����ӵ�ͼƬ�ĳ���
}

//����:����ľ��飺enemy,��n���ӵ�,״̬
void IEnemyOnFire(const char *szname,int n)//�з���ʼ̹�˿���
{
            //��ȡ�����λ�úͳ���
        Ienemy.PosX=dGetSpritePositionX(szname);
        Ienemy.PosY=dGetSpritePositionY(szname);
        Ienemy.Direction=dGetSpriteRotation(szname);

        //�����ӵ�
        dCloneSprite("bullet",dMakeSpriteName("ienemybullet",n));
        IEnemyBullet[n]=dMakeSpriteName("ienemybullet",n);//��¼��������
        dSetSpriteVisible(IEnemyBullet[n],true);//��֤�ӵ��ɼ�

        dSetSpritePosition(IEnemyBullet[n],Ienemy.PosX,Ienemy.PosY);//�����ӵ���λ��
        dSetSpriteLinearVelocityPolar(IEnemyBullet[n],EnemyBulletSpeed,(Ienemy.Direction+270));//�����ӵ����䷽��,����ͼƬ�ķ������⣬�Ƕȼ�270��
        dSetSpriteRotation(IEnemyBullet[n],Ienemy.Direction);//�����ӵ�ͼƬ�ĳ���
}


    //�������з�����̹�����֣��з�̹�˽Ǳ��ţ���n���ӵ�
void EnemyOnFire(const char *szname,int Number,int n)//�з�̹�˿���
  {
        //��ȡ�����λ�úͳ���
        enemy[Number].PosX=dGetSpritePositionX(szname);
        enemy[Number].PosY=dGetSpritePositionY(szname);
        enemy[Number].Direction=dGetSpriteRotation(szname);

        //�����ӵ�
        dCloneSprite("bullet",dMakeSpriteName("enemybullet",n));
        enemy[Number].EnemyBullet[n]=dMakeSpriteName("enemybullet",n);//��¼��������
        dSetSpriteVisible(enemy[Number].EnemyBullet[n],true);//��֤�ӵ��ɼ�

        dSetSpritePosition(enemy[Number].EnemyBullet[n],enemy[Number].PosX,enemy[Number].PosY);//�����ӵ���λ��
        dSetSpriteLinearVelocityPolar(enemy[Number].EnemyBullet[n],EnemyBulletSpeed,(enemy[Number].Direction+270));//�����ӵ����䷽��,����ͼƬ�ķ������⣬�Ƕȼ�270��
        dSetSpriteRotation(enemy[Number].EnemyBullet[n],enemy[Number].Direction);//�����ӵ�ͼƬ�ĳ���
  }


void CreatEnemyTanks(int j)//�����з�̹��,��������j���β���
{
    for(int i=0;i<EnemyNum;i++)//ÿ�������ɵĵз�̹��
    {
        enemy[10*j+i].name = dMakeSpriteName("enemy",10*j+i);//������¼�з�����̹�˵��±� ji
        dCloneSprite("enemy",enemy[10*j+i].name);

        //��֤���Ƴ���̹�˿���ײ���ɼ�������
        dSetSpriteVisible(enemy[10*j+i].name,true);
        dSetSpriteCollisionActive(enemy[10*j+i].name,true,true);

        TotalEnemyNum++;//���ڳ�ʼ����¼
        enemy[10*j+i].PosX = dRandomRange(-24*4,24*4)/4;//��ҪΪ4�ı���

        int a = rand()%2+1;//�������������
        if(a==1)
        {
            enemy[10*j+i].PosY = 0.f;
        }
        else
        {
            enemy[10*j+i].PosY = -22.f;
        }

        dSetSpritePosition(enemy[10*j+i].name,enemy[10*j+i].PosX,enemy[10*j+i].PosY);//����з�̹��λ��
        dSetSpriteVisible(enemy[10*j+i].name,true);//��֤���Ƴ�����̹�˿ɼ�
    }
}

void EnemyMove(const char* szName)//�з�̹�˵��˶�,��������������
{
    int s = rand()%4+1;//����4������������ĸ�����
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

//̹�˱�ը������������1����ը���飬2�����ͣ�1Ϊ�ҷ�̹�ˣ�2Ϊ�з���ʼ̹�ˣ�3Ϊ�з�����̹��
void Explode(const char* szname,int type)
{
    if(type == 1)//�ҷ�̹��
    {
        //��ȡ�����λ�úͳ���
        mytank.PosX=dGetSpritePositionX(szname);
        mytank.PosY=dGetSpritePositionY(szname);
        dCloneSprite("explode","myexplode");

        //���ñ�ըλ��
        dSetSpritePosition("myexplode",mytank.PosX,mytank.PosY);
        dSetSpriteLifeTime("myexplode",0.5);//���ʱ��Ϊ0.5��

    }

    if(type ==2)//�з���ʼ̹��
    {
        //��ȡ�����λ�úͳ���
        Ienemy.PosX=dGetSpritePositionX(szname);
        Ienemy.PosY=dGetSpritePositionY(szname);
        dCloneSprite("explode","Ienemyexplode");

        //���ñ�ըλ��
        dSetSpritePosition("Ienemyexplode",Ienemy.PosX,Ienemy.PosY);
        dSetSpriteLifeTime("Ienemyexplode",0.5);//���ʱ��Ϊ0.5��
    }

}

//���Ƶĵз�̹�˱�ը��������1����ը̹�ˣ�2���з�̹�˱��
void CopyExplode(const char* szname,int number)
{
        //��ȡ�����λ�úͳ���
        enemy[number].PosX=dGetSpritePositionX(szname);
        enemy[number].PosY=dGetSpritePositionY(szname);
        enemy[number].explode=dMakeSpriteName("enemyexplode",number);//��¼����
        dCloneSprite("explode",enemy[number].explode);

         //���ñ�ըλ��
        dSetSpritePosition(enemy[number].explode,enemy[number].PosX,enemy[number].PosY);
        dSetSpriteLifeTime(enemy[number].explode,0.5);//���ʱ��Ϊ0.5��
}

//שǽ���ݻٱ�ը����������ըǽ������,��n��ǽ 100Ϊ��ʼǽ�ı��
void WallExplode(const char* szname,int n)
{
    float x,y;
    if(n=100)//����ģ��ǽ   ??:��==�������Զ���=������
    {
        //��¼ǽ������
        x = dGetSpritePositionX(szname);
        y = dGetSpritePositionY(szname);
        dCloneSprite("wallexplode",dMakeSpriteName("wallexplode",n));

        //���ñ�ըλ��
        dSetSpritePosition(dMakeSpriteName("wallexplode",n),x,y);
        dSetSpriteLifeTime(dMakeSpriteName("wallexplode",n),0.5);//���ʱ��Ϊ0.5��
    }

    else
    {

        x = dGetSpritePositionX(szname);
        y = dGetSpritePositionY(szname);
        dCloneSprite("wallexplode",dMakeSpriteName("wallexplode",n));
    }
}

//����̹�˵���������������ײ���������ӵ������ɼ�
void IEnemyDie(const char* szname)
{
    dSetSpriteVisible(szname,false);
    dSetSpriteCollisionActive(szname,false,false);
    Ienemy.State = 2;//״̬Ϊ����
}

void toolbomp(void)//���ߣ���ը
{
    //��ʼ̹������
    Explode("enemy",2);
    IEnemyDie("enemy");//���Ϊ����
    TotalEnemyNum--;
    Score++;
    dPlaySound("kaipao",0,1);//���ű�ը����

    //�ִ�̹�˵�����
    for(int i=1;i<j;i++)
    {
        for(int k=0;k<EnemyNum;k++)
            {
                dDeleteSprite(enemy[10*i+k].name);
                CopyExplode(enemy[10*i+k].name,10*i+k);//���ű�ը����
                dPlaySound("kaipao",0,1);//���ű�ը����
                Score++;
            }
    }
}

void tooltimer(void)//���ߣ�������Ϸʱ��
{
        GameLeftTime-=10;//������Ϸʱ��
}

void toolstar(void)//���ߣ��������
{
        MyBulletSpeed = 50.f;
}

void toolcreat(int s)//���߲���,�����������ĵ�s�Ź���
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

    while(Map[i=dRandomRange(2,10)][j=dRandomRange(2,12)]==0)//Ϊ0�Ĳ�����ǽ����������Ե��������߳�����ǽ��
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

void differentnum(int a[],int m,int n,int k)//������ͬ�ļ��������,����:�������飬��Χ������n��m������k��
{
    int i,j,t;
    for(i = 0; i < k; i++)
    {
        t = rand()%(n-m+1)+m;
        for(j = 0; j < i; j ++)
            if(a[j] == t)
                break;
            if(j == i)//���ظ�
                a[i] = t;//��¼�������
    }
}
