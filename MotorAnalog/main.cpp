#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "oldaapi.h" //For Keithley



int NumberOfUSB1s = 0;
unsigned char ModuleAddress;
bool blnResult;
DBL volt = 2.5, gain, min, max, gain1, min1, max1;
HDEV hDev = NULL;
HDASS hDA = NULL, hDA1 = NULL;
LNG iValue;

char lpszDriverName1[100], lpszDriverName2[100];
UINT resolution, encoding, resolution1, encoding1, channel[2] = { 0,1 };
HDEV hDevice[2];
char Bname[2][100];
using namespace std;

void EnumBrdProc(LPSTR lpszBrdName, LPSTR lpszDriverName, LPARAM lParam)
{
	// Make sure we can Init Board
	//MessageBox::Show("Hollo");
	(olDaInitialize((PTSTR)lpszBrdName, (LPHDEV)lParam));
	
	// Make sure Board has an A/D Subsystem 
	UINT uiCap = 0;
	olDaGetDevCaps(*((LPHDEV)lParam), OLDC_DAELEMENTS, &uiCap);
	
	//printf("Called%d\n",cnt);
	
}
/*Intitialisation of Keithley board*/
void init(void) {
	olDaEnumBoards((DABRDPROC)EnumBrdProc, (LPARAM)&hDevice[0]);
	(olDaGetDeviceName(hDevice[0], (PTSTR)lpszDriverName1, 100));
	olDaGetDASS(hDevice[0], OLSS_DA, 0, &hDA);
	olDaSetDataFlow(hDA, OL_DF_SINGLEVALUE);
	olDaSetChannelListEntry(hDA, 0, channel[0]);
	olDaSetChannelListEntry(hDA, 0, channel[1]);
	olDaConfig(hDA);
	olDaGetResolution(hDA, &resolution);
	olDaGetEncoding(hDA, &encoding);
	olDaGetGainListEntry(hDA, 0, &gain);
	olDaGetRange(hDA, &max, &min);
	olDaVoltsToCode(min, max, gain, resolution, encoding, volt, &iValue);
	olDaPutSingleValue(hDA, iValue, channel[0], gain);
	olDaPutSingleValue(hDA, iValue, channel[1], gain);

}

/* Function to stop motor*/
void stopmotor()
{
	(olDaVoltsToCode(min, max, gain, resolution, encoding, 2.5, &iValue));  //To Stop Motor
	(olDaPutSingleValue(hDA, iValue, channel[0], gain));
	(olDaPutSingleValue(hDA, iValue, channel[1], gain));
}

/*Function to start motor*/
void startmotor(DBL v)
{
	(olDaVoltsToCode(min, max, gain, resolution, encoding, v, &iValue));  //To Stop Motor
	(olDaPutSingleValue(hDA, iValue, channel[0], gain));
	(olDaPutSingleValue(hDA, iValue, channel[1], gain));
}

void startmotor_R(DBL v)
{
	(olDaVoltsToCode(min, max, gain, resolution, encoding, v, &iValue));  //To Stop Motor
	(olDaPutSingleValue(hDA, iValue, channel[0], gain));
	//(olDaPutSingleValue(hDA, iValue, channel[1], gain));
}

void startmotor_L(DBL v)
{
	(olDaVoltsToCode(min, max, gain, resolution, encoding, v, &iValue));  //To Stop Motor
	//(olDaPutSingleValue(hDA, iValue, channel[0], gain));
	(olDaPutSingleValue(hDA, iValue, channel[1], gain));
}


int main() {

	int b;
	init();
	  
	stopmotor();
	while (1) {
		printf("********************\n");
		printf("MOTOR CONTROL\n");
		printf("*********************\n");
		printf("\n \n");
		printf("Select the option:\n\n");
		printf("1.Colckwise\n");
		printf("2.Counter Clockwise\n");
		printf("3.Stop\n");
		printf("4. Quit\n");
		scanf_s("%d", &b);

		if (b == 2) {
			printf("****Running Counter Clockwise****\n");
			startmotor(2.5);
			startmotor(0);

		}
		else if (b == 1) {
			printf("****Running  Clockwise****\n");
			startmotor(2.5);
			startmotor(5);

		}

		else if (b == 4) {
			startmotor_L(0);
			startmotor_R(5);
			
		}
		else  {
			printf("****Motor Stopped****\n");
			startmotor(2.5);
			break;
		}

		getchar();
	}

	return 0;
}


