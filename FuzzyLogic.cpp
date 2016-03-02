extern "C"
{
	#include <stdlib.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <string.h>
	#include <ctype.h>

	
}

#include "SonarLib.h"
#include "dcLib.h"
#include <stdio.h>
#include <iostream>
#include <map>
#include <math.h> 

using namespace std;

//const
const int sizeOfArrayMembershipFunctionInput = 3;
const int sizeOfArrayMembershipFunctionMotor = 3;
const int sizeOfArrayMembershipFunctionMotorOA = 4;
const int sizeOfArrayReturnFunctionOutput = 9;
const int sizeOfArrayReturnFunctionOutputOA = 27;
const int NumberOfOutput = 2;

class MembershipFunc{
public:
	int leftConner;
	int leftShoulder;
	int rightShoulder;
	int rightConner;
	int center;
	string word;
public:	MembershipFunc(string _word, int _leftConner, int _leftShoulder, int  _rightShoulder, int _rightConner, int _center = 0)
{
	leftConner = _leftConner;
	leftShoulder = _leftShoulder;
	rightShoulder = _rightShoulder;
	rightConner = _rightConner;
	center = _center;
	word = _word;
}

		pair<string, double> GetValueOfMembershipInPoint(double point)
		{
			pair<string, double> _return = make_pair(word, 0.0);
			if (point >= leftConner && point <= rightConner)
			{
				if (point >= leftShoulder && point <= rightShoulder)
				{
					_return.second = 1.0;
				}
				else if (point < leftShoulder)
				{
					_return.second = (point - leftConner) / (leftShoulder - leftConner);
				}
				else if (point >= rightShoulder)
				{
					_return.second = (rightConner - point) / (rightConner - rightShoulder);
				}
			}
			return _return;
		}
};

class MembershipRezults
{
public:
	double fazzyficationMeaning[sizeOfArrayMembershipFunctionInput];
	string fazzyficationWord[sizeOfArrayMembershipFunctionInput];
	MembershipRezults()
	{
		fazzyficationWord[0] = "Near";
		fazzyficationWord[1] = "Medium";
		fazzyficationWord[2] = "Far";
	}
	void setMembershipValue(pair<string, double> MembershipValue)
	{
		string linguisticInt = MembershipValue.first;
		for (int i = 0; i < sizeOfArrayMembershipFunctionInput; i++)
		{
			if (linguisticInt == fazzyficationWord[i])
			{
				fazzyficationWord[i] = MembershipValue.first;
				fazzyficationMeaning[i] = MembershipValue.second;
				break;
			}
		}

	}
	double getMemberShipResults(string value)
	{
		double _return = 0.0;
		for (int i = 0; i < sizeOfArrayMembershipFunctionInput; i++)
		{
			if (value == fazzyficationWord[i])
			{
				_return = fazzyficationMeaning[i];
				break;
			}
		}
		return _return;
	}
	void ClearResults()
	{
		for (int i = 0; i < sizeOfArrayMembershipFunctionInput; i++)
		{
			fazzyficationMeaning[i] = 0;
		}
	}
};
// global varibles for right edge folowing(can be local, but it is more complicated)
MembershipRezults frontSensorMembershipRezults;
MembershipRezults rightSensorMembershipRezults;
pair<string, double> rightMotorMembershipRezults[sizeOfArrayReturnFunctionOutput];
pair<string, double> leftMotorMembershipRezults[sizeOfArrayReturnFunctionOutput];
//----------------------------------------------------------------------------------
// global value to obstacle avoidance
MembershipRezults frontLeftSensorOAMembershipRezults;
MembershipRezults frontSensorOAMembershipRezults;
MembershipRezults frontRightSensorOAMembershipRezults;
pair<string, double> rightMotorMembershipRezultsOA[sizeOfArrayReturnFunctionOutput];
pair<string, double> leftMotorMembershipRezultsOA[sizeOfArrayReturnFunctionOutput];
//--------------------------------------------------------------------------------

void SetFuzzyRule(string frontMeaning, string leftMeaning, string leftMotorResults, string rightMotorResults, int *currentNumber)
{

	double rightMembershipRezults = rightSensorMembershipRezults.getMemberShipResults(leftMeaning);
	double frontMembershipRezults = frontSensorMembershipRezults.getMemberShipResults(frontMeaning);
	if (rightMembershipRezults > 0
		&& frontMembershipRezults > 0)
	{
		double min = rightMembershipRezults;
		if (frontMembershipRezults < min)
		{
			min = frontMembershipRezults;
		}
		rightMotorMembershipRezults[*currentNumber] = make_pair(rightMotorResults, min);
		leftMotorMembershipRezults[*currentNumber] = make_pair(leftMotorResults, min);

		(*currentNumber)++;
	}
}
void SetFuzzyRuleOA(string leftMeaning, string rightMeaning, string leftMotorResults, string rightMotorResults, int *currentNumber)
{
	double leftMembershipRezults = frontLeftSensorOAMembershipRezults.getMemberShipResults(leftMeaning);
	double rightMembershipRezults = frontRightSensorOAMembershipRezults.getMemberShipResults(rightMeaning);

	if (leftMembershipRezults > 0 && rightMembershipRezults > 0)
	{
		double min = leftMembershipRezults;
		if (rightMembershipRezults < min)
		{
			min = rightMembershipRezults;
		}
		rightMotorMembershipRezultsOA[*currentNumber] = make_pair(rightMotorResults, min);
		leftMotorMembershipRezultsOA[*currentNumber] = make_pair(leftMotorResults, min);
		(*currentNumber)++;
	}
}
double getMembershipCenterOfGravity(MembershipFunc(MembershipOutput[sizeOfArrayReturnFunctionOutput]), string nameOfMembership)
{
	for (int i = 0; i < sizeOfArrayMembershipFunctionInput; i++)
	{
		if ((MembershipOutput)[i].word == nameOfMembership)
		{
			return (MembershipOutput)[i].center;
		}
	}
}
double defuzzyfication(pair<string, double>(*MembershipRezult)[sizeOfArrayReturnFunctionOutput], MembershipFunc MembershipOutput[sizeOfArrayMembershipFunctionInput])
{
	double _return = 0.0;
	double summ = 0.0;
	double summ2 = 0.0;
	for (int i = 0; i < sizeOfArrayMembershipFunctionInput; i++)
	{
		if ((*MembershipRezult)[i].first == "")
		{
			continue;
		}
		summ += getMembershipCenterOfGravity(MembershipOutput, (*MembershipRezult)[i].first) * ((*MembershipRezult)[i]).second;
		summ2 += ((*MembershipRezult))[i].second;
	}
	if (summ2 > 0)
	{
		_return = summ / summ2;
	}
	return _return;
}
void clearArray(pair<string, double> ar[sizeOfArrayReturnFunctionOutput])
{
	for (int i = 0; i < sizeOfArrayReturnFunctionOutput; i++)
	{
		ar[i].first = "";
		ar[i].second = 0;
	}
}
double defuzzyficationOA(pair<string, double>(*MembershipRezult)[sizeOfArrayReturnFunctionOutput], MembershipFunc MembershipOutput[sizeOfArrayMembershipFunctionInput])
{
	double _return = 0.0;
	double summ = 0.0;
	double summ2 = 0.0;
	for (int i = 0; i < sizeOfArrayMembershipFunctionInput; i++)
	{
		if ((*MembershipRezult)[i].first == "")
		{
			continue;
		}
		summ += getMembershipCenterOfGravity(MembershipOutput, (*MembershipRezult)[i].first) * ((*MembershipRezult)[i]).second;
		summ2 += ((*MembershipRezult))[i].second;
	}
	if (summ2 > 0)
	{
		_return = summ / summ2;
	}
	return _return;
}
void getFuzzyMembershipRightEdgeFollowing(double FrontSensorValue, double RightSensorValue, double *leftMotorSpeed, double *rightMotorSpeed)
{
	clearArray(rightMotorMembershipRezults);
	clearArray(leftMotorMembershipRezults);
	frontSensorMembershipRezults.ClearResults();
	rightSensorMembershipRezults.ClearResults();
	int plus = -11;
	// Membership Right Edge Following
	MembershipFunc MembershipInput[sizeOfArrayMembershipFunctionInput] {
		MembershipFunc("Near", 0, 0, 30 + plus, 45 + plus),
			MembershipFunc("Medium", 30 + plus, 50 + plus, 40 + plus, 65 + plus),
			MembershipFunc("Far", 40 + plus, 65 + plus, 100, 100)
	};
	//---------------------------------------------------------------

	// Motor Membership function
	MembershipFunc MembershipOutput[sizeOfArrayMembershipFunctionMotor] {
		    MembershipFunc("Slow", 0, 5, 5, 15, 5),
			MembershipFunc("Medium", 10, 20, 20, 25, 20),
			MembershipFunc("Fast", 20, 30, 30, 45, 30)
	};
	// --------------------------------------------------------------
	// calculate membership rezults from input 
	for (int i = 0; i < sizeOfArrayMembershipFunctionInput; i++)
	{
		frontSensorMembershipRezults.setMembershipValue(MembershipInput[i].GetValueOfMembershipInPoint(FrontSensorValue));
		rightSensorMembershipRezults.setMembershipValue(MembershipInput[i].GetValueOfMembershipInPoint(RightSensorValue));
	}
	//---------------------------------------------------------------


	// fuzzy rules
	int z = 0;
	SetFuzzyRule("Near", "Near", "Slow", "Medium", &z);
	SetFuzzyRule("Near", "Medium", "Slow", "Medium", &z);
	SetFuzzyRule("Near", "Far", "Slow", "Medium", &z);

	SetFuzzyRule("Medium", "Near", "Medium", "Slow", &z);
	SetFuzzyRule("Medium", "Medium", "Medium", "Medium", &z);
	SetFuzzyRule("Medium", "Far", "Medium", "Slow", &z);

	SetFuzzyRule("Far", "Near", "Fast", "Medium", &z);
	SetFuzzyRule("Far", "Medium", "Medium", "Slow", &z);
	SetFuzzyRule("Far", "Far", "Medium", "Slow", &z);
	//---------------------------------------------------------------


	// calc deffuzyfication and set motor speed
	*rightMotorSpeed = defuzzyfication(&rightMotorMembershipRezults, MembershipOutput);
	*leftMotorSpeed = defuzzyfication(&leftMotorMembershipRezults, MembershipOutput);
	//--------------------------------------------------------

}
double GetMinimum(double x1, double x2, double x3 = -1)
{
	double _return = x1;
	if (x2 < _return)
	{
		_return = x2;
	}
	if (x3 != -1 && x3 < _return)
	{
		_return = x3;
	}
	return _return;
}

void getFuzzyLogicObsticalAvoidnes(double FrontLeftSensorValue, double FrontRightSensorValue, double *leftMotorSpeed, double *rightMotorSpeed)
{
	frontLeftSensorOAMembershipRezults.ClearResults();
	frontRightSensorOAMembershipRezults.ClearResults();
	clearArray(rightMotorMembershipRezultsOA);
	clearArray(leftMotorMembershipRezultsOA);
	// Membership Function Obstacle Avoidance

	MembershipFunc MembershipInput[sizeOfArrayMembershipFunctionInput] {
		MembershipFunc("Near", 0, 0, 30, 50),
			MembershipFunc("Medium", 50, 70, 70, 95),
			MembershipFunc("Far", 75, 90, 100, 100)
	};
	//--------------------------------------------------------------
	// Motor Membership function
	MembershipFunc MembershipOutput[sizeOfArrayMembershipFunctionMotorOA] {
		    MembershipFunc("Back", -20, -10, -10, 10, -10),
			MembershipFunc("Slow", 0, 10, 10, 20, 10),
			MembershipFunc("Medium", 20, 25, 25, 30, 20),
			MembershipFunc("Fast", 30, 35, 35, 40, 35)
	};
	// --------------------------------------------------------------
	// calculate membership rezults from input 
	for (int i = 0; i < sizeOfArrayMembershipFunctionInput; i++)
	{
		frontLeftSensorOAMembershipRezults.setMembershipValue(MembershipInput[i].GetValueOfMembershipInPoint(FrontLeftSensorValue));
		frontRightSensorOAMembershipRezults.setMembershipValue(MembershipInput[i].GetValueOfMembershipInPoint(FrontRightSensorValue));
	}
	//---------------------------------------------------------------

	// fuzzy rules 
	int z = 0;
	SetFuzzyRuleOA("Near", "Near", "Back", "Slow", &z);
	SetFuzzyRuleOA("Medium", "Medium", "Back", "Slow", &z);
	SetFuzzyRuleOA("Far", "Far", "Medium", "Medium", &z);


	SetFuzzyRuleOA("Near", "Medium", "Slow", "Back", &z);
	SetFuzzyRuleOA("Near", "Far", "Medium", "Back", &z);
	SetFuzzyRuleOA("Medium", "Far", "Medium", "Back", &z);

	SetFuzzyRuleOA("Medium", "Near", "Back", "Slow", &z);
	SetFuzzyRuleOA("Far", "Near", "Back", "Slow", &z);
	SetFuzzyRuleOA("Far", "Medium", "Slow", "Medium", &z);


	//---------------------------------------------------------------


	// calc deffuzyfication and set motor speed
	*rightMotorSpeed = defuzzyficationOA(&rightMotorMembershipRezultsOA, MembershipOutput);
	*leftMotorSpeed = defuzzyficationOA(&leftMotorMembershipRezultsOA, MembershipOutput);
	//--------------------------------------------------------

}

int main(int argc, char *argv[])
{
	initialiseSonars();
	InitialiseDC();
	double leftMotorValue = 0.0, rigthMotorValue = 0.0;
	double rightFrontSensor = 0.0, rightWallSensor = 0.0;
	double FrontLeftSensorValue = 0.0, FrontRightSensorValue = 0.0;
	try{
	while (true){

		rightFrontSensor = getCM(FRS);
		rightWallSensor = getCM(BRS);

		FrontLeftSensorValue = getCM(LFS);
		FrontRightSensorValue = getCM(RFS);

		//cout << "left " << FrontLeftSensorValue;
		//cout << " Right " << FrontRightSensorValue << endl;

		double arrayLeftMotor[NumberOfOutput];
		double arrayRightMotor[NumberOfOutput];

		int dObsticalAvoidnes = GetMinimum(FrontLeftSensorValue, FrontRightSensorValue);
		int dRightEdgeFollowing = GetMinimum(rightFrontSensor, rightWallSensor);

		MembershipFunc dObsticalAvoidnesMembershipValue("Near", 0, 0, 80, 100);
		MembershipFunc dRightEdgeFollowingMembershipValue("Near", 0, 0, 90, 100);

		pair<string, double> membershipValuedObsticalAvoidnes = dObsticalAvoidnesMembershipValue.GetValueOfMembershipInPoint(dObsticalAvoidnes);
		pair<string, double> membershipValuedRightEdgeFollowing = dRightEdgeFollowingMembershipValue.GetValueOfMembershipInPoint(dRightEdgeFollowing);

		double arrayF[NumberOfOutput];
		arrayF[0] = 0;
		if (membershipValuedObsticalAvoidnes.second > 0)
		{
			arrayF[0] = membershipValuedObsticalAvoidnes.second;
			getFuzzyLogicObsticalAvoidnes(FrontLeftSensorValue, FrontRightSensorValue, &leftMotorValue, &rigthMotorValue);
			arrayLeftMotor[0] = leftMotorValue;
			arrayRightMotor[0] = rigthMotorValue;
			//cout << "left " << leftMotorValue;
			//cout << " Right " << rigthMotorValue << endl;
		}
		arrayF[1] = 0;
		if (membershipValuedRightEdgeFollowing.second > 0)
		{
			arrayF[1] = membershipValuedRightEdgeFollowing.second;
			getFuzzyMembershipRightEdgeFollowing(rightFrontSensor, rightWallSensor, &leftMotorValue, &rigthMotorValue);
			arrayLeftMotor[1] = leftMotorValue;
			arrayRightMotor[1] = rigthMotorValue;
		}

		double summ1 = 0.0, summ2 = 0.0;
		double LeftMotorSpeed = 0.0, RightMotorSpeed = 0.0;
		for (int i = 0; i < NumberOfOutput; i++)
		{
			summ1 += arrayF[i] * arrayLeftMotor[i];
			summ2 += arrayF[i];
		}
		LeftMotorSpeed = summ1 / summ2;
		summ1 = 0.0;
		summ2 = 0.0;
		for (int i = 0; i < NumberOfOutput; i++)
		{
			summ1 += arrayF[i] * arrayRightMotor[i];
			summ2 += arrayF[i];
		}
		RightMotorSpeed = summ1 / summ2;

		getFuzzyMembershipRightEdgeFollowing(rightFrontSensor, rightWallSensor, &leftMotorValue, &rigthMotorValue);
		cout << "LeftMotorSpeed = " << LeftMotorSpeed << endl;
		cout << "RightMotorSpeed = " << RightMotorSpeed << endl;
		//MotorMotion(LeftMotorSpeed, RightMotorSpeed);
		 time_sleep(0.3);
	}
	}
	catch()
	{cout << "we have problem" << endl;}
	MotorMotion(0,0);
}