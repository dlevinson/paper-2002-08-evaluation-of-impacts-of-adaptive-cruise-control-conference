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
#include <cmath>
#include "vehicle.h"


const short maxVehicleNumber= 200;
const float RoadLength= 200;		// meters
const float simulateTime= 100;  	// seconds
// const float maxVehicleSize = 5;  // meter
// const float initialSpeed= 20 *1.6*1000/(3600);  	// meter/s
// const float inTrafficFlowRate= 1; // veh/s
// const float sampleTime = 0.1;  // s 
	
vehicle carFollowing(float time, vehicle veh1, vehicle veh2);
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
		junkVehicle.oldSpeed=initialSpeed;
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
		//junkVehicle=vehiclePlatoon[numberOfVehicles-1]; //.back();
		//lastTimeHeadway = junkVehicle.vehiclePosition / initialSpeed;
		lastTimeHeadway=time-lastEntryTime;
		if(lastTimeHeadway > timeHeadway)
		{
			numberOfVehicles++; 
			lastVehicleID++;
			
			// Parameters of New Vehicle
			junkVehicle.vehicleID=lastVehicleID;
			junkVehicle.vehiclePosition = 0;
			junkVehicle.vehicleSpeed=initialSpeed;
			junkVehicle.oldSpeed=initialSpeed;
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
		//do{

			junkVehicle=vehiclePlatoon[0];
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
		//}while((vehiclePlatoon.front()).vehiclePosition>RoadLength);

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
			junkVehicle1 = carFollowing(time, junkVehicle1, junkVehicle2);
			vehiclePlatoon[i]= junkVehicle1;
		}

		//
		// (4)
		// Save the states of each vehicle
		
		for(i=0;i<=numberOfVehicles-1;i++)
		{
			junkVehicle1 = vehiclePlatoon[i];
			fprintf(fd1, "%d\t", junkVehicle1.vehicleID);
			fprintf(fd1, "%8.4f\t", time);
			fprintf(fd1, "%8.4f\t", junkVehicle1.vehiclePosition); 
			fprintf(fd1, "%8.4f\n", junkVehicle1.vehicleSpeed);
		}
		
		// (5)
		// Taffic Parameters Calculation: q-inflow & outflow, u-speed, k-density
		density[counter] = numberOfVehicles/RoadLength;
		speed[counter]=0;
		for(i=0;i<=numberOfVehicles-1;i++)
		{
			junkVehicle1 = vehiclePlatoon[i];
			if(junkVehicle1.vehicleSpeed>0.1) speed[counter]=speed[counter]+1/junkVehicle1.vehicleSpeed;
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

// Gipps Car-following Model
vehicle carFollowing(float time, vehicle veh1, vehicle veh2)
{

	float	vehPosition1=veh1.vehiclePosition;
	float	vehSpeed1=veh1.vehicleSpeed;
	float	oldSpeed1=veh1.oldSpeed;
	float	vehPosition2=veh2.vehiclePosition;
	float	vehSpeed2=veh2.vehicleSpeed;
	
	float 	va, vb, junk, definitiveSpeed;
	float 	lambda = 0.3;
	float	v_desired = 20;
	float	accl_max=1.7;
	float 	del_max=-2.0*accl_max;
	float	del_max_est;

	//float timeHeadway =  1/ veh1.flowrate;
	//float lamda = 0.4; 	// Control gain for constant time-gap control law
	//float desiredAccel = -(vehSpeed1 - vehSpeed2 + lamda * (vehPosition1 - vehPosition2 + maxVehicleSize + timeHeadway* vehSpeed1))/timeHeadway;

	va=vehSpeed1+2.5*accl_max*sampleTime*(1-vehSpeed1/v_desired)*sqrt(0.025+vehSpeed1/v_desired);
			
	del_max_est=(-3.0 < ((double)(del_max-3.0)/2))? (-3.0):((del_max-3.0)/2);
		junk = 2 * (vehPosition2 - maxVehicleSize - vehPosition1) - vehSpeed1 * sampleTime - (oldSpeed1*oldSpeed1) / del_max_est;
			
	vb=del_max*sampleTime+sqrt(del_max*del_max * sampleTime * sampleTime- del_max * junk);
	definitiveSpeed = (va<vb)? va:vb; 
	
	//if(definitiveSpeed>10) definitiveSpeed=10;
	veh1.oldSpeed=veh1.vehicleSpeed; 
	veh1.vehicleSpeed=definitiveSpeed;
	veh1.vehiclePosition=veh1.vehiclePosition+sampleTime*veh1.vehicleSpeed;

	return veh1;
}

float inFlow(float time)
{
	if(time>=15 && time<30) return 3;
	else return 1;
}
