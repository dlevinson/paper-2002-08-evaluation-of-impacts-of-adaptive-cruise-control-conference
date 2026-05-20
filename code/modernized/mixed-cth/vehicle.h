// vehicle.h
// Define the class : vehicle and related functions


// Class of Vehicle
using namespace std; 	//introduces namespace std
class vehicle
{
// Data members...
	private:
		
		
// Member functions...
	public:	
		long vehicleID;
		float vehicleLength;
		float vehiclePosition;
		float vehicleSpeed;
		float oldSpeed;
		float vehicleAccel;
		float desiredAccel;
		float flowrate;
		long nextVehicle;
		long followingMode; 
		

		vehicle();
		vehicle(long id, float vehiclePosition, float vehicleSpeed, float vehicleAccel);
		~vehicle();

		long ID() const;
		float Length() const;
		float Position() const;
		float Speed() const;
		float Accel() const;
		long NextVehicle() const;
		
};

inline long vehicle::ID() const
{
	return vehicleID;
}

inline float vehicle::Length() const
{
	return vehicleLength;
}

inline float vehicle::Position() const
{
	return vehiclePosition;
}

inline float vehicle::Speed() const
{
	return vehicleSpeed;
}

inline float vehicle::Accel() const
{
	return vehicleAccel;
}
    
inline long vehicle::NextVehicle() const
{
	return nextVehicle;
}

vehicle::vehicle()
{
	vehicleID = 0;
	vehiclePosition= 0;
	vehicleSpeed = 0;
	vehicleAccel=0;
}
// Single Vehicle generation: initial position, speed and acceleration 
vehicle::vehicle(long id, float vehPosition, float vehSpeed, float vehAccel)
{
	vehicleID = id;
	vehiclePosition= vehPosition; // initial Position
	vehicleSpeed = vehSpeed;   //initial Speed;
	vehicleAccel=vehAccel;
//	cout << "Creating new vehicle #" << vehicleID << '\n';
}

// Single Vehicle Deletion  
vehicle::~vehicle()
{

}

