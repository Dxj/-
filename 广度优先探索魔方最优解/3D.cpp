// 3D.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <stdio.h>
#include <queue>
using  namespace std;
//逆时针转动6个表面(未通过规律减少存储)
bool bSurface[6]={false,true,true,false,false,true};//表面的逆时针转动,增为True(可以重新编序前True后False)
int iSide[6][4][3][2]={//4个侧面转到另一个侧面
					   {{2,5,2,6,2,7},//增{全为前2增后2减,可以只记录每行第一个}
					    {3,5,3,6,3,7},//增
					    {5,3,5,2,5,1},//减
					    {4,3,4,2,4,1}//减
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
//逆时针转动cube的iSurface面
void rotate(int cube[][9],int iSurface)
{
	//转表面,8Grid
	int iCubeSurfaceGrid_tmp[8],iGrid;//不含中间8
	for(iGrid=0;iGrid<8;iGrid++)
	{
		if(bSurface[iSurface]) iCubeSurfaceGrid_tmp[(iGrid+2)%8]=cube[iSurface][iGrid];
		else iCubeSurfaceGrid_tmp[iGrid]=cube[iSurface][(iGrid+2)%8];
	}
	for(iGrid=0;iGrid<8;iGrid++)
	{
		cube[iSurface][iGrid]=iCubeSurfaceGrid_tmp[iGrid];
	}

	//转侧面,3*4Grid
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
//广度优先探索最优解
int complete(int cube[][9])
{
	//面,格,遍历面
	int iSurface=0,iGrid=0,iSurfaceDir_tmp=0;
	queue<CubeWithDir> cubeOfDirs;
	//记录旋转后的状态
	CubeWithDir cubeAfterRotate;
	//当前状态
	CubeWithDir cubeCurrent;
	//初始化
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
			if(iSurfaceDir_tmp==cubeCurrent.iSurfaceDir&&cubeCurrent.iSurfaceDirNo==3)continue;//避免自转
			for(iSurface=0;iSurface<6;iSurface++)
			{
				for(iGrid=0;iGrid<9;iGrid++)
				{
					cubeAfterRotate.cube[iSurface][iGrid]=cubeCurrent.cube[iSurface][iGrid];
				}
			}
//			printf("%d面旋转",iSurfaceDir_tmp);
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

	printf("使用了%d步",stepNo);
	return 0;
}