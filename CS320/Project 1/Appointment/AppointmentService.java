//Author Name: Kyle Hake
//Date: 1/23/2021
//Course ID: CS-320
/*Description:  This file is the appointment service 
 *it uses appointment objects to make an array list of appointments.
 *Can add/remove appointments from this array list.
 */


package appointment;

import java.util.ArrayList;
import java.util.List;
import java.util.*;


public class AppointmentService {
	
	//List of appointments for add/delete methods
	public List<Appointment> appointments = new ArrayList<Appointment>();
	
	//Constructors
	public AppointmentService() {
		
	}
	
	//Methods
	
	//gets size of appointments list, used for iteration in while loops
	public int getAppointmentCount() {
		return appointments.size();
	}
	
	
	//adds appointment to appointment list
	public Appointment addAppointment(String appointmentID, Calendar appointmentDate, String appointmentDescription) {
		
		Appointment appointment = null;
		
		int i = 0;
		
		//checks to see if there is a appointment with the same appointment ID
		while(i < getAppointmentCount()) {
			
			//if there is a match, throws exception
			if (appointmentID == appointments.get(i).getAppointmentID()) {
				appointment = appointments.get(i);
				throw new IllegalArgumentException("Duplicate appointmentID not allowed.");
			}
		}
		
		//if not match, adds the appointment to the appointment list
		if (appointment == null) {
			appointment = new Appointment(appointmentID, appointmentDate, appointmentDescription);
			appointments.add(appointment);
		}
		
		
		return appointment;
	}
	
	//deletes appointment based on appointmentID
	public void deleteAppointment(String appointmentID) {
		
		int i = 0;
		
		while (i < getAppointmentCount()) {
			if (appointmentID == appointments.get(i).getAppointmentID()) {
				appointments.remove(i);
			}
		}
	}

}
