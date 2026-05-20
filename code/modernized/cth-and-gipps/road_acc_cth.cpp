// 	Pipeline Simulation Program 
// 	Version 2.0 Beta 0.0.1 	Dec. 2000
//
// 	This program is used to simulate the traffic flow in a single pipeline.
// 

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include "vehicle.h"


const short maxVehicleNumber= 200;
const float RoadLength= 200;		// meters
const float simulateTime= 100;  	// seconds
// const float maxVehicleSize = 5;  // meter
// const float initialSpeed= 20 *1.6*1000/(3600);  	// meter/s
// const float inTrafficFlowRate= 1; // veh/s
// const float sampleTime = 0.1;  // s 

vehicle vehicleCalculation(float time, vehicle veh1, vehicle veh2, int followingMode);
vehicle grkt1(float time,float sampletime,vehicle veh1, vehicle veh2);	
void carDynamics(float time, vehicle curruntVehicle, vehicle leadingVehicle, float *det);  
float carFollowing(float time, vehicle veh1, vehicle veh2);
float inFlow(float time);

 // Initialize the platoon
typedef vector<vehicle> Platoon;
Platoon vehiclePlatoon;	

using namespace std; 	//introduces namespace std
int main()
{		

	int i, counter;
	int followingMode=1;
	long numberOfVehicles;
	long firstVehicleID=1;
	long lastVehicleID;
	float time=0.0, timeHeadway, lastTimeHeadway, LeftTime, oldLeftTime;
	float lastEntryTime=0;
	float density[10000], speed[10000], inflow[10000], outflow[10000];
	FILE *fd1, *fd2;

	//vector<vehicle>::iterator IDinPlatoon;
	
	vehicle junkVehicle, junkVehicle1, junkVehicle2;
	
// Open the data file to store simulation results
	fd1=fopen("simudata1.dat","w"); 	
	fd2=fopen("simudata2.dat","w"); 
	
// Calclulate the initial number of vehicles
	timeHeadway= 1 / inFlow(time);
	numberOfVehicles = RoadLength / (initialSpeed * timeHeadway + maxVehicleSize);
	lastVehicleID = numberOfVehicles;
	
// Initialize the postion, speed and acceleration of the platoon 
	for(i=0;i<=numberOfVehicles-1;i++)	
	{	
		junkVehicle.vehicleID= i;
		junkVehicle.vehiclePosition=RoadLength - (initialSpeed * timeHeadway + maxVehicleSize) * i;
		junkVehicle.vehicleSpeed=initialSpeed;
		junkVehicle.vehicleAccel=0.0;
		junkVehicle.desiredAccel=0.0;
		junkVehicle.flowrate=inFlow(time);
		vehiclePlatoon.push_back(junkVehicle);
	}

//
// The main loop: simulate unitil time=simulateTime
//
	counter=0;
	do{	
	
		// (1)
		// The last vehicle's timeHeadway = lastVehicle's postion / initialSpeed
		// If timeHeadway > 1/(inTrafficFlowRate), then GenerateNewVehicle  
		timeHeadway= 1 / inFlow(time);
		junkVehicle=vehiclePlatoon[numberOfVehicles-1]; //.back();
		lastTimeHeadway = junkVehicle.vehiclePosition / initialSpeed;
		//lastTimeHeadway=time-lastEntryTime;
		if(lastTimeHeadway > timeHeadway)
		{
			numberOfVehicles++; 
			lastVehicleID++;
			
			// Parameters of New Vehicle
			junkVehicle.vehicleID=lastVehicleID;
			junkVehicle.vehiclePosition = 0;
			junkVehicle.vehicleSpeed=initialSpeed;
			junkVehicle.vehicleAccel=0;
			junkVehicle.desiredAccel=0;
			junkVehicle.flowrate=inFlow(time);
			// Add new vehicle
			vehiclePlatoon.push_back(junkVehicle);
			inflow[counter]=1/(time-lastEntryTime);
			lastEntryTime=time;
		}	
		else
		{	if(counter==0) inflow[counter]=0;
			else inflow[counter]=inflow[counter-1];
		}
		// (2)
		// If vehiclePosition > RoadLength then delete first vehicle
		junkVehicle=vehiclePlatoon.front();
		if(junkVehicle.vehiclePosition > RoadLength)	
		{	
			
			vehiclePlatoon.erase(vehiclePlatoon.begin());
			numberOfVehicles--;
			firstVehicleID++;
			LeftTime=time;
			outflow[counter]=1/(LeftTime-oldLeftTime);
			oldLeftTime=LeftTime;
		}
		else 
		{
			if(counter==0) outflow[counter]=0;
			else outflow[counter]=outflow[counter-1];
		}
		// The speed of the new first vehicle become constant
		junkVehicle = vehiclePlatoon[0];
		junkVehicle.vehicleAccel=0;
		junkVehicle.desiredAccel=0;
		vehiclePlatoon[0]=junkVehicle;

		// (3)
		// Calculate the new states of each vehicle, not including the first vehicle
		for(i=0;i<=numberOfVehicles-1;i++) //IDinPlatoon; IDinPlatoon!=vehiclePlatoon.begin(); IDinPlatoon--)
		{
			if(i==0)
			{
			// If it's the first vehicle, no acceleration or deceleration
			junkVehicle1 = vehiclePlatoon.at(i); 
			junkVehicle2 = vehiclePlatoon.at(i); 
			}
			else
			{
				junkVehicle1 = vehiclePlatoon.at(i); 	// current vehicle
				junkVehicle2 = vehiclePlatoon.at(i-1); 	// the leading vehicle
			}
			// Calculate the new states of vehicles in time+sampleTime
			junkVehicle1 = vehicleCalculation(time, junkVehicle1, junkVehicle2, followingMode);
			vehiclePlatoon[i]= junkVehicle1;
		}

		//
		// (4)
		// Save the states of each vehicle
		
		for(i=0;i<=numberOfVehicles-1;i++)
		{
			junkVehicle1 = vehiclePlatoon[i];
			fprintf(fd1, "%8.4f\t", time);
			fprintf(fd1, "%d\t", junkVehicle1.vehicleID);
			fprintf(fd1, "%8.4f\t", junkVehicle1.vehiclePosition); 
			fprintf(fd1, "%8.4f\t", junkVehicle1.vehicleSpeed);
			fprintf(fd1, "%8.4f\n", junkVehicle1.vehicleAccel);
		}
		
		// (5)
		// Taffic Parameters Calculation: q, u, k
		density[counter] = numberOfVehicles/RoadLength;
		speed[counter]=0;
		for(i=0;i<=numberOfVehicles-1;i++)
		{
			junkVehicle1 = vehiclePlatoon[i];
			speed[counter]=speed[counter]+1/junkVehicle1.vehicleSpeed;
		}
		speed[counter]=numberOfVehicles/(speed[counter]);
		//inflow[counter] = inFlow(time);
		
		fprintf(fd2, "%8.4f\t", time);
		fprintf(fd2, "%8.4f\t", density[counter]);
		fprintf(fd2, "%8.4f\t", speed[counter]); 
		fprintf(fd2, "%8.4f\t", inflow[counter] );
		fprintf(fd2, "%8.4f\n", outflow[counter] );

		// (6)
		// Update Time
		time=time+sampleTime;
		counter++;

	}while(time<simulateTime);
	

	fclose(fd1);
	fclose(fd2);
	return 0;
}

vehicle vehicleCalculation(float time, vehicle veh1, vehicle veh2, int followingMode)
{
	vehicle junkVehicle;
	// Data used in calculation:
	// The former positions and speeds of this vehicle and the leading vehicle
	// Data generated : the present position, speed and acceleration of this vehicle

//	switch(followingMode) 
//	{
//		case 1: // ACC Mode
			
	veh1=grkt1(time,sampleTime,veh1, veh2);	
	// The speed of the new first vehicle become constant
		junkVehicle = vehiclePlatoon[0];
		junkVehicle.vehicleAccel=0;
		junkVehicle.desiredAccel=0;
		vehiclePlatoon[0]=junkVehicle;
						
//		case 2: // Gipps Mode
//			veh1.vehicleSpeed = carFollowing(veh1.vehiclePosition, veh1.vehicleSpeed, veh2.vehiclePosition, veh2.vehicleSpeed, 2);
//			veh1.vehiclePosition = veh1.vehiclePosition +  veh1.vehicleSpeed * sampleTime + 0.5 *  veh1.vehicleAccel * sampleTime* sampleTime;
//	}
	return veh1;
}

void carDynamics(float time, vehicle curruntVehicle, vehicle leadingVehicle, float *det)
{
	float tau=0.1;
	vehicle veh1,veh2;
	veh1=curruntVehicle;
	veh2=leadingVehicle;
//%State equations
//f(1:n,1) = X(n+1 : 2*n,1);
//f(n+1:2*n,1) = X(2*n+1 : 3*n,1);
//f(2*n+1:3*n,1) = (-1/tau) *  X(2*n+1:3*n,1) + (1/tau) *xdes_dot_dot ;
	
	curruntVehicle.desiredAccel=carFollowing(time, veh1, veh2);
	det[0]=curruntVehicle.vehicleSpeed; 
	det[1]=curruntVehicle.vehicleAccel; 
	det[2]=(-1/tau) * curruntVehicle.vehicleAccel + (1/tau) *curruntVehicle.desiredAccel;  //xdes_dot_dot;
	
}

vehicle grkt1(float time,float sampletime,vehicle veh1, vehicle veh2)		
{ 	//extern void carDynamics();
    	int i,j,l;
    	float a[4],tt,*b,*d;
    	int n=3;  		// number of variables
		b=(float *) malloc(n*sizeof(double));
    	d=(float *) malloc(n*sizeof(double));
    	float h=0.02;  	// length of integral step
		a[0]=h/2.0; a[1]=a[0];
    	a[2]=h; a[3]=h;
    	
    	int k= sampleTime/h + 1;   // number of integral step 	
    	float y[3];
    	float z[3*10];
    	
    	y[0]=veh1.vehiclePosition;
    	y[1]=veh1.vehicleSpeed;
    	y[2]=veh1.vehicleAccel;
    	for (i=0; i<=n-1; i++) z[i*k]=y[i];
    	for (l=1; l<=k-1; l++)
      	{ 
      		carDynamics(time, veh1, veh2, d);
        	for (i=0; i<=n-1; i++) b[i]=y[i];
        	for (j=0; j<=2; j++)
          	{ 	
          		for (i=0; i<=n-1; i++)
              	{ 
              		y[i]=z[i*k+l-1]+a[j]*d[i];
                	b[i]=b[i]+a[j+1]*d[i]/3.0;
              	}
            	tt=time+a[j];
            	carDynamics(tt, veh1, veh2, d);
          	}
        	for (i=0; i<=n-1; i++)   y[i]=b[i]+h*d[i]/6.0;
        	for (i=0; i<=n-1; i++)   z[i*k+l]=y[i];
        	time=time+h;
      	}
    	free(b); free(d);
    	veh1.vehiclePosition=z[k-1];
    	veh1.vehicleSpeed=z[2*k-1];
    	if(abs(z[3*k-1])<-3) z[3*k-1]=-3;
		veh1.vehicleAccel=z[3*k-1];
	
		return veh1;
}

float carFollowing(float time, vehicle veh1, vehicle veh2)
{
	float vehPosition1=veh1.vehiclePosition;
	float vehSpeed1=veh1.vehicleSpeed;
	float vehPosition2=veh2.vehiclePosition;
	float vehSpeed2=veh2.vehicleSpeed;

	float timeHeadway =  1/ veh1.flowrate;
	float lamda = 0.4; 	// Control gain for constant time-gap control law
	
	float desiredAccel = -(vehSpeed1 - vehSpeed2 + lamda * (vehPosition1 - vehPosition2 + maxVehicleSize + timeHeadway* vehSpeed1))/timeHeadway;
	//if(desiredAccel<-5) desiredAccel=-5;
	return desiredAccel;
}

float inFlow(float time)
{
	if(time>=5 && time<20) return 3;
	else return 1;
}
