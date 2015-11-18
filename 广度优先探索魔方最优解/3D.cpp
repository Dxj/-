// 3D.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <stdio.h>
#include <queue>
using  namespace std;
//��ʱ��ת��6������(δͨ�����ɼ��ٴ洢)
bool bSurface[6]={false,true,true,false,false,true};//�������ʱ��ת��,��ΪTrue(�������±���ǰTrue��False)
int iSide[6][4][3][2]={//4������ת����һ������
					   {{2,5,2,6,2,7},//��{ȫΪǰ2����2��,����ֻ��¼ÿ�е�һ��}
					    {3,5,3,6,3,7},//��
					    {5,3,5,2,5,1},//��
					    {4,3,4,2,4,1}//��
					   },//0
					   {{1,1,1,2,1,3},
					    {4,3,4,4,4,5},
						{6,7,6,6,6,5},
					    {3,1,3,8,3,7}					    
					   },//1
					   {{1,7,1,8,1,1},
					    {2,5,2,4,2,3},
					    {6,5,6,4,6,3},
					    {5,7,5,8,5,1}
					   },//2
					   {{1,3,1,4,1,5},
					    {5,3,5,4,5,5},
						{6,1,6,8,6,7},
					    {2,1,2,8,2,7}					    
					   },//3
					   {{1,5,1,6,1,7},
					    {3,5,3,4,3,3},
					    {6,3,6,2,6,1},
					    {4,7,4,8,4,1}					    
					   },//4
					   {{2,1,2,2,2,3},
					    {4,7,4,6,4,5},
					    {5,7,5,6,5,5},
					    {3,1,3,2,3,3}					    
					   }//5
	};
typedef struct _CubeWithDir{
	int cube[6][9];
	int iSurfaceDir;
	int iSurfaceDirNo;
	int iStepNo;
}CubeWithDir;
//��ʱ��ת��cube��iSurface��
void rotate(int cube[][9],int iSurface)
{
	//ת����,8Grid
	int iCubeSurfaceGrid_tmp[8],iGrid;//�����м�8
	for(iGrid=0;iGrid<8;iGrid++)
	{
		if(bSurface[iSurface]) iCubeSurfaceGrid_tmp[(iGrid+2)%8]=cube[iSurface][iGrid];
		else iCubeSurfaceGrid_tmp[iGrid]=cube[iSurface][(iGrid+2)%8];
	}
	for(iGrid=0;iGrid<8;iGrid++)
	{
		cube[iSurface][iGrid]=iCubeSurfaceGrid_tmp[iGrid];
	}

	//ת����,3*4Grid
	int iCubeSideGrid_tmp[12];
	for(iGrid=0;iGrid<12;iGrid++)
	{
		iCubeSideGrid_tmp[(iGrid+3)%12]=cube[iSide[iSurface][iGrid/3][iGrid%3][0]-1][iSide[iSurface][iGrid/3][iGrid%3][1]-1];
	}
	for(iGrid=0;iGrid<12;iGrid++)
	{
		cube[iSide[iSurface][iGrid/3][iGrid%3][0]-1][iSide[iSurface][iGrid/3][iGrid%3][1]-1]=iCubeSideGrid_tmp[iGrid];
	}
}

bool success(int cube[][9])
{
	for(int iSurface=0;iSurface<6;iSurface++)
	{
		for(int iGrid=0;iGrid<8;iGrid++)
		{
			if(cube[iSurface][iGrid]!=cube[iSurface][8])return false;
		}
	}
	return true;
}
//�������̽�����Ž�
int complete(int cube[][9])
{
	//��,��,������
	int iSurface=0,iGrid=0,iSurfaceDir_tmp=0;
	queue<CubeWithDir> cubeOfDirs;
	//��¼��ת���״̬
	CubeWithDir cubeAfterRotate;
	//��ǰ״̬
	CubeWithDir cubeCurrent;
	//��ʼ��
	for(iSurface=0;iSurface<6;iSurface++)
	{
		for(iGrid=0;iGrid<9;iGrid++)
		{
			cubeCurrent.cube[iSurface][iGrid]=cube[iSurface][iGrid];
		}
	}
	cubeCurrent.iSurfaceDir=0;
	cubeCurrent.iSurfaceDirNo=0;
	cubeCurrent.iStepNo=0;

	cubeOfDirs.push(cubeCurrent);
	while(!cubeOfDirs.empty())
	{
		memcpy(&cubeCurrent,&cubeOfDirs.front(),sizeof(cubeCurrent));
		cubeOfDirs.pop();
		for(iSurfaceDir_tmp=0;iSurfaceDir_tmp<6;iSurfaceDir_tmp++)
		{
			if(iSurfaceDir_tmp==cubeCurrent.iSurfaceDir&&cubeCurrent.iSurfaceDirNo==3)continue;//������ת
			for(iSurface=0;iSurface<6;iSurface++)
			{
				for(iGrid=0;iGrid<9;iGrid++)
				{
					cubeAfterRotate.cube[iSurface][iGrid]=cubeCurrent.cube[iSurface][iGrid];
				}
			}
//			printf("%d����ת",iSurfaceDir_tmp);
			rotate(cubeAfterRotate.cube,iSurfaceDir_tmp);
			if(cubeAfterRotate.iSurfaceDir==iSurfaceDir_tmp)cubeAfterRotate.iSurfaceDirNo++;
			else cubeAfterRotate.iSurfaceDirNo=1;
			cubeAfterRotate.iSurfaceDir=iSurfaceDir_tmp;
			cubeAfterRotate.iStepNo=cubeCurrent.iStepNo+1;
			if(success(cubeAfterRotate.cube)) return cubeAfterRotate.iStepNo; 
			else cubeOfDirs.push(cubeAfterRotate);
		}
//		printf("\n");
	}
	return 0;
}
int main(int argc, char* argv[])
{
	int cube[6][9]={
		0,0,0,0,0,0,0,0,0,
		1,1,1,1,3,3,3,1,1,
		2,2,2,2,1,1,1,2,2,
		4,4,4,3,3,3,3,3,3,
		2,2,2,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5
	};
	int stepNo=complete(cube);
//	rotate(cube,0);
//	rotate(cube,1);

	printf("ʹ����%d��",stepNo);
	return 0;
}