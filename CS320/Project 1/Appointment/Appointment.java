//Author Name: Kyle Hake
//Date: 1/31/2021
//Course ID: CS-320
/*Description:  This file is the appointment class object, 
 *it is used by the Appointment Service script to add/delete/modify contacts to
 *an Arraylist of appointment.
 */

package appointment;

import java.util.*;
import java.util.Date;

public class Appointment {
	
	//Private Variables
	private String appointmentID;
	
	//Public Variables
	public Calendar appointmentDate = Calendar.getInstance();
	public Date date = new Date();
	public String appointmentDescription;
	
	//constructors
	public Appointment(String appointmentID, Calendar appointmentDate, String appointmentDescription) {
		if(appointmentID == null || appointmentID.length() > 10) {
			throw new IllegalArgumentException("Invalid Appointment ID");
		}
		if(appointmentDate == null || appointmentDate.before(Calendar.getInstance())) {
			throw new IllegalArgumentException("Invalid Appointment Date");
		}
		if(appointmentDescription == null || appointmentDescription.length() > 50) {
			throw new IllegalArgumentException("Invalid Appointment Description");
		}
			
		this.appointmentID = appointmentID;
		this.appointmentDate = appointmentDate;
		this.appointmentDescription = appointmentDescription;
	}
	
	//getters
	public String getAppointmentID() {
		return appointmentID;
	}
		
	public Calendar getAppointmentDate() {
		return appointmentDate;
	}
			
	public String getAppointmentDescription() {
		return appointmentDescription;
	}
	
	//setters
	public void setAppointmentDate(Calendar appointmentDate) {
		if(appointmentDate == null || appointmentDate.before(Calendar.getInstance())) {
			throw new IllegalArgumentException("Invalid Appointment Date");
			}
		this.appointmentDate = appointmentDate;
	}
			
	public void setAppointmentDescription(String appointmentDescription) {
		if(appointmentDescription == null || appointmentDescription.length() > 50) {
			throw new IllegalArgumentException("Invalid Appointment Description");
		}
		this.appointmentDescription = appointmentDescription;
	}

	
	
}
