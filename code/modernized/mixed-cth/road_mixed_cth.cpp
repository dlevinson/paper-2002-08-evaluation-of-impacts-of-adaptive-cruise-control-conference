// 	Pipeline Simulation Program 
// 	Version 2.0 Beta 0.1.6 	Jan. 2001
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
const float RoadLength= 1000;	// meters
const float simulateTime= 500;  	// seconds
const float initialSpeed= 40 *1.6*1000/(3600);  	//meter/s
const float sampleTime = 0.1;  // s 
const float inTrafficFlowRate= 1; // veh/s
const float maxVehicleSize = 6; // meters


vehicle vehicleCalculation(int counter, float time, vehicle veh1, vehicle veh2, long followingMode);
vehicle grkt1(float time,float sampletime,vehicle veh1, vehicle veh2);	
void carDynamics(float time, vehicle curruntVehicle, vehicle leadingVehicle, float *det);  
float constantTime(float time, vehicle veh1, vehicle veh2);
vehicle gipps(int counter, float time, vehicle veh1, vehicle veh2);
float inFlow(float time);
float rand01(float *r);

// Initialize the platoon
typedef vector<vehicle> Platoon;
Platoon vehiclePlatoon;	

using namespace std; 	//introduces namespace std
int main()
{		

	int i, counter;
	long numberOfVehicles;
	long firstVehicleID=1;
	long lastVehicleID;
	float time=0.0, timeHeadway, lastTimeHeadway, LeftTime, oldLeftTime;
	float randNumber, tmp, tmp1, tmp2;
	float lastEntryTime=0, ACC_proportion=0.9;
	float density[10000], speed[10000], inflow[10000], outflow[10000];
	FILE *fd1, *fd2;

	//vector<vehicle>::iterator IDinPlatoon;
	
	vehicle junkVehicle, junkVehicle1, junkVehicle2;
	
// Open the data file to store simulation results
	fd1=fopen("simudata1.dat","w"); 	
	fd2=fopen("simudata2.dat","w"); 
	
// Calclulate the initial number of vehicles
	timeHeadway= 1; 
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
		junkVehicle.flowrate=1; 
		
		tmp=rand01(&randNumber);
		if(tmp>=ACC_proportion) junkVehicle.followingMode=1;		// use Gipps model
			else junkVehicle.followingMode=0;			// use contant Time Headway control
		vehiclePlatoon.push_back(junkVehicle);
	}

//
// The main loop: simulate unitil time=simulateTime
//
	counter=0;
	do{	
	
		// (1)
		// The last vehicle's timeHeadway = lastVehicle's postion / initialSpeed
		// If lastTimeHeadway > timeHeadway, then Generate New Vehicle  
		timeHeadway= 1 / inFlow(time);
		junkVehicle=vehiclePlatoon[numberOfVehicles-1]; //.back();
		//lastTimeHeadway = junkVehicle.vehiclePosition / initialSpeed;
		lastTimeHeadway=time-lastEntryTime;
		if(lastTimeHeadway >= timeHeadway) 
		{
			numberOfVehicles++; 
			lastVehicleID++;
			
			// Parameters of New Vehicle
			junkVehicle1.vehicleID=lastVehicleID;
			junkVehicle1.vehiclePosition = maxVehicleSize;
			junkVehicle1.vehicleSpeed=initialSpeed; //junkVehicle.vehiclePosition/timeHeadway;
			junkVehicle1.oldSpeed=initialSpeed;	//junkVehicle.vehiclePosition /timeHeadway;
			junkVehicle1.vehicleAccel=0;
			junkVehicle1.desiredAccel=0;
			junkVehicle1.flowrate=inFlow(time);
		
			tmp=rand01(&randNumber);
			if(tmp>=ACC_proportion) junkVehicle1.followingMode=1;		// use Gipps model
			else junkVehicle1.followingMode=0;			// use contant Time Headway control
			// Add new vehicle
			vehiclePlatoon.push_back(junkVehicle1);
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
			junkVehicle1 = vehicleCalculation(counter, time, junkVehicle1, junkVehicle2, junkVehicle1.followingMode);
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
			fprintf(fd1, "%8.4f\t", junkVehicle1.vehicleSpeed);
			fprintf(fd1, "%8.4f\n", junkVehicle1.vehicleAccel);
		}
		
		// (5)
		// Taffic Parameters Calculation: q, u, k
		density[counter] = numberOfVehicles;
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
		fprintf(fd2, "%8.4f\t", inflow[counter]);
		fprintf(fd2, "%8.4f\n", outflow[counter]);

		// (6)
		// Update Time
		time=time+sampleTime;
		counter++;

	}while(time<simulateTime);
	
	tmp=0;
	for(i=0;i<=counter-1;i++) tmp=tmp+speed[i];
	tmp=tmp/counter;
	tmp1=0;
	for(i=0;i<=counter-1;i++) tmp1=tmp1+speed[i]*speed[i];
	
	tmp1=tmp1/counter-tmp*tmp;

	//fprintf(fd2, "mean:\t %8.4f\t", tmp);
	//fprintf(fd2, "variance\t %8.4f\t", tmp1);

	fclose(fd1);
	fclose(fd2);
	return 0;
}

// Calculate the state of vehicle according to its control mode
vehicle vehicleCalculation(int counter, float time, vehicle veh1, vehicle veh2, long followingMode)
{
	// Data used in calculation:
	// The former positions and speeds of this vehicle and the leading vehicle
	// Data generated : the present position, speed and acceleration of this vehicle
	vehicle junkVehicle;
	int i=0;
	switch(followingMode) 
	{
		case 0: // ACC Mode
			veh1=grkt1(time, sampleTime, veh1, veh2);
			break;
						
		case 1: // Gipps Mode
			veh1=gipps(counter, time, veh1, veh2);
			break;

		default:
			break;
			// veh1.vehiclePosition = veh1.vehiclePosition +  veh1.vehicleSpeed * sampleTime + 0.5 *  veh1.vehicleAccel * sampleTime* sampleTime;
	}//end of switch

	// The speed of the new first vehicle become constant
	junkVehicle = vehiclePlatoon[0];
	junkVehicle.vehicleAccel=0;
	junkVehicle.desiredAccel=0;
	vehiclePlatoon[0]=junkVehicle;

	return veh1;
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
		if(z[3*k-1]>1.7) z[3*k-1]=1.7;
		veh1.vehicleAccel=z[3*k-1];
	
		return veh1;
}

// State equations of a single vehicle
void carDynamics(float time, vehicle curruntVehicle, vehicle leadingVehicle, float *det)
{
	float tau=0.1;
	vehicle veh1,veh2;
	veh1=curruntVehicle;
	veh2=leadingVehicle;

//%State equations
	
	curruntVehicle.desiredAccel=constantTime(time, veh1, veh2);
	det[0]=curruntVehicle.vehicleSpeed; 
	det[1]=curruntVehicle.vehicleAccel; 
	det[2]=(-1/tau) * curruntVehicle.vehicleAccel + (1/tau) *curruntVehicle.desiredAccel;  //xdes_dot_dot;
	
}

// Constant time headway control algorithm
float constantTime(float time, vehicle veh1, vehicle veh2)
{
	float vehPosition1=veh1.vehiclePosition;
	float vehSpeed1=veh1.vehicleSpeed;
	float vehPosition2=veh2.vehiclePosition;
	float vehSpeed2=veh2.vehicleSpeed;

	float timeHeadway =  1; //  / veh1.flowrate;
	float lamda = 0.4; 	// Control gain for constant time-gap control law
	
	float desiredAccel = -(vehSpeed1 - vehSpeed2 + lamda * (vehPosition1 - vehPosition2 + maxVehicleSize + timeHeadway* vehSpeed1))/timeHeadway;
	//if(desiredAccel<-5) desiredAccel=-5;
	return desiredAccel;
}

// Gipps Car-following Model
vehicle gipps(int counter, float time, vehicle veh1, vehicle veh2)
{
	int		i;
	float	vehPosition1=veh1.vehiclePosition;
	float	vehSpeed1=veh1.vehicleSpeed;
	float	oldSpeed1=veh1.oldSpeed;
	float	vehPosition2=veh2.vehiclePosition;
	float	vehSpeed2=veh2.vehicleSpeed;
	
	float 	va, vb, junk, definitiveSpeed;
	float	v_desired = 50 * 1.6* 1000 /(3600);
	float	accl_max=1.7;
	float 	del_max=-2.0*accl_max;
	float	del_max_est;

	vehicle junkVehicle;

	va=vehSpeed1+2.5*accl_max*sampleTime*15*(1-vehSpeed1/v_desired)*sqrt(0.025+vehSpeed1/v_desired);
			
	del_max_est=(-2.0 < ((double)(del_max-2.0)/2))? (-2.0):((del_max-2.0)/2);
		junk = 2 * (vehPosition2 - maxVehicleSize - vehPosition1) - vehSpeed1 * sampleTime - (oldSpeed1*oldSpeed1) / del_max_est;
			
	vb=del_max*sampleTime*15+sqrt(del_max*del_max * sampleTime * sampleTime*225- del_max * junk);
	definitiveSpeed = (va<vb)? va:vb; 
	
	if(definitiveSpeed<=0) definitiveSpeed=0;
	
	i=fmod(counter,15);
	if(i==0)
	{
		veh1.oldSpeed=veh1.vehicleSpeed; 
		veh1.vehicleSpeed=definitiveSpeed;
	}
	veh1.vehiclePosition=veh1.vehiclePosition+sampleTime*veh1.vehicleSpeed;

	return veh1;
}

// Generation of random number between 0 and 1 with uniform distribution
float rand01(float *r)
{ 
	int m;
    float s,u,v,p;
    s=65536.0; u=2053.0; v=13849.0;
    m=(int)(*r/s); *r=*r-m*s;
    *r=u*(*r)+v; m=(int)(*r/s);
    *r=*r-m*s; p=-*r/s;
    return(p);
}

// The entering flow rate 
float inFlow(float time)
{
	//if(time>=100 && time<200) return time*0.005;
	//else 
	return 0.8;
}
