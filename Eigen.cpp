#include <iostream>
#include <string>
#include <Eigen/Dense>
#include "math.h"
#include <stdlib.h>

#define pi 3.141592653589793

using namespace std;
using namespace Eigen;

 int Dimension = 2;

MatrixXd & getCoordinate(MatrixXd &,string & coordinate);//����������ֵ���ַ���ת��Ϊ��ֵ
MatrixXd & RotationMatrix(MatrixXd &,string &);	//�����������ת�Ƕ������ת����
MatrixXd & Move(MatrixXd &,MatrixXd &);	//ƽ��֮��õ�ƽ���������
MatrixXd & Rotate(MatrixXd &,MatrixXd &,string &,string &);	//��ת���
MatrixXd & LineMirror(MatrixXd &, Vector3d &);

int main()
{
  int NumberOfPoints ;
  string GraphicName;
  string Coordinate;

  cout <<"����ͼ�����֣�ͼ�ε������������:" <<endl;
  cin >> GraphicName >> NumberOfPoints >> Coordinate;

  MatrixXd PointMatrix(Dimension,NumberOfPoints);	//�����ľ���
  PointMatrix = getCoordinate(PointMatrix,Coordinate);
  MatrixXd store = PointMatrix;

  cout << "��ʼ����ֵ" << endl << PointMatrix << endl;

  string Command;
  string GraphicNameC;
  string RotPoint;	//��ת����
  string CommandCoordinate;	//��ת������

  cout <<"������ָ���move l1 ��3,6����rotate -30 (0,0):" <<endl;
  cin >> Command >> GraphicNameC >> CommandCoordinate>>RotPoint ;

  if (Command == "move")
  {
	MatrixXd MoveMatrix(Dimension,1);	//�����ƶ�����
	//int Num = 1;
	MoveMatrix = getCoordinate(MoveMatrix,CommandCoordinate);
	PointMatrix = Move(PointMatrix,MoveMatrix);

  }
  if(Command == "rotate")
  {
	MatrixXd Rotation(Dimension,Dimension);	//��ת����
	PointMatrix = Rotate(PointMatrix,Rotation,CommandCoordinate,RotPoint);
  }

  cout << "The transfer result is:" <<endl << PointMatrix << endl;

  double a,b,c;
  cout << "������ֱ�ߵĲ���a,b,c,ax+by+c=0:"<< endl;
  cin >> a >> b >> c;
  Vector3d LCo(a,b,c);
  MatrixXd MirrorResult;
  MirrorResult = LineMirror(store,LCo);
  cout << "����ֱ�߾���Ľ����"<< endl << MirrorResult<< endl;
  system("pause");
}

MatrixXd & getCoordinate(MatrixXd & pointMatrix,string & coordinate)
{
	//��ȡ���������ֵ
  string tempt;		//temp�����洢�ַ��������������
  int count = 0;
//  MatrixXd pointMatrix(Dimen,pointNumber);
  for(int i = 0; i < coordinate.length(); i++)
  {  
	if((coordinate[i] >= '0' && coordinate[i] <= '9')|| 
		coordinate[i] == ','||coordinate[i] == ')'|| coordinate[i] == '.')
	{
		if(coordinate[i] == ',' || coordinate[i] == ')')
		{ 
			count = count + 1;
			int k = count/2;
			int m = count%2 - 1;
		if(m == 0)
			double j = pointMatrix(0,k) = atof(tempt.c_str());	
		if(m == -1)
		{
			double n = 0;
			n = pointMatrix(1,k-1) = atof(tempt.c_str());
			int i = 0;
		}
		tempt.clear();
		continue;
		}
		tempt = tempt + coordinate[i];
	}	
  }
  return pointMatrix;
}

//���������ת���Ƕ����ת������
MatrixXd & RotationMatrix(MatrixXd & Rotation,string & arc)
{
	double angle = atof(arc.c_str());
	
	double s1,c1;
	s1 = sin(angle*pi/180);
	c1 = cos(angle*pi/180);

	if(abs(s1)<0.000000000000001)
		s1 = 0.0;
	if (abs(c1) < 0.000000000000001)
		c1=0;
	
	Rotation(0,0) = c1;
	Rotation(0,1) = s1;
	Rotation(1,0) = -s1;
	Rotation(1,1) = c1;

	return Rotation;
}

//���ƶ���ĵ�����
MatrixXd & Move(MatrixXd & point,MatrixXd & moveMatrix)
{
	int Num = point.cols();

	for(int i = 0; i < Num; i++)
	{
		point(0,i) = point(0,i) + moveMatrix(0,0);
		point(1,i) = point(1,i) + moveMatrix(1,0);
	}

	return point;
}

//����ת��ĵ�����
MatrixXd & Rotate(MatrixXd & Poi,MatrixXd & Rot ,string &command,string &RPoint)
{
	MatrixXd rotPoint;	//�洢������ת����
	MatrixXd temp(Dimension,Dimension);

	rotPoint = getCoordinate(temp,RPoint);	//��ת��������
	rotPoint = (-1)*rotPoint;
	Poi = Move(Poi,rotPoint);	//��ת�����ƶ���ԭ��
	Rot = RotationMatrix(Rot,command);
	Poi = Rot*Poi;		//����ԭ��Գ�
	rotPoint = (-1)*rotPoint;
	Poi = Move(Poi,rotPoint);	//����ת�����ƻ�ԭʼλ��

	return Poi;
}

//����������ֵ
MatrixXd & LineMirror(MatrixXd & point,Vector3d & Co)
{
	int Num = point.cols();
	double a = Co(0);
	if( Co(0) == 0)
	{
		double y=-Co(2)/Co(1);
		for(int i = 0; i < Num; i++)
			point(1,i) = 2*y - point(1,i);
	}
	if(Co(1) == 0)
	{
		double x = -Co(2)/Co(0);
		for(int i = 0; i < Num; i++)
			point(0,i) = 2*x - point(0,i);
	}
	if(Co(0)!=0 && Co(1)!=0)
	{
		
		for(int i = 0; i < Num; i++)
		{
			double L = (Co(0)*point(0,i)+Co(1)*point(1,i)+Co(2));
			double D = (Co(0)*Co(0)+Co(1)*Co(1));
			if(Co(0)*point(0,i)+Co(1)*point(1,i)+Co(2) == 0)
				continue;
			point(0,i) = point(0,i)-2*Co(0)*L/D;
			point(1,i) = point(1,i)-2*Co(1)*L/D;
		}
	}
	return point;
}