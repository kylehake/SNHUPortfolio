//Author Name: Kyle Hake
//Date: 2/3/2021
//Course ID: CS-320
/*Description:  This file is the appointment test file.
 *It tests the constructor and setters, as well as input validation
 */

package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import appointment.Appointment;

import java.util.Calendar;

class AppointmentTest {

	//Constructor Testing
	@Test
	void testAppointment() {
		Calendar date = Calendar.getInstance();
		date.add(Calendar.DATE, 15);
		Appointment appointment = new Appointment("12345", date, "Birthday Party");
		assertTrue(appointment.getAppointmentID().equals("12345"));
		assertTrue(appointment.getAppointmentDate().equals(date));
		assertTrue(appointment.getAppointmentDescription().equals("Birthday Party"));
	}
	
	//Setter Testing
	@Test
	void testSetAppointmentDate() {
		Appointment appointment = new Appointment("12345", Calendar.getInstance(), "Birthday Party");
		appointment.setAppointmentDate(Calendar.getInstance());
		assertTrue(appointment.getAppointmentDate().equals(Calendar.getInstance()));
	}
	
	@Test
	void testSetFirstName() {
		Appointment appointment = new Appointment("12345", Calendar.getInstance(), "Birthday Party");
		appointment.setAppointmentDescription("Wedding");
		assertTrue(appointment.getAppointmentDescription().equals("Wedding"));
	}
	
	//Input Validation for Constructors
	@Test
	void testAppointmentIDTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("01234567891", Calendar.getInstance(), "Birthday Party");
		});
	}
	
	@Test
	void testAppointmentIDNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment(null , Calendar.getInstance(), "Birthday Party");
		});
	}
	
	@Test
	void testAppointmentDateNotBefore() {
		Calendar date = Calendar.getInstance();
		date.add(Calendar.DATE, -15);
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("12345", date, "Birthday Party");
		});
	}
	
	@Test
	void testAppointmentDateNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("12345", null, "Birthday Party");
		});
	}
	
	@Test
	void testAppointmentDescriptionTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("12345", Calendar.getInstance(), "Birthday Party Birthday Party Birthday Party Birthday Party");
		});
	}
	
	@Test
	void testAppointmentDescriptionNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Appointment("12345", Calendar.getInstance(), null);
		});
	}
	
	//Input Validation for Setter Testing
	@Test
	void testSetAppointmentDateIsNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Appointment appointment = new Appointment("12345", Calendar.getInstance(), "Birthday Party");
			appointment.setAppointmentDate(null);
		});
	}
	
	@Test
	void testSetAppointmentDateIsNotBefore() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Calendar date = Calendar.getInstance();
			Calendar date1 = Calendar.getInstance();
			date1.add(Calendar.DATE, -15);
			Appointment appointment = new Appointment("12345", date, "Birthday Party");
			appointment.setAppointmentDate(date1);
		});
	}
	
	@Test
	void testSetAppointmentDescriptionIsNotNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Appointment appointment = new Appointment("12345", Calendar.getInstance(), "Birthday Party");
			appointment.setAppointmentDescription(null);
		});
	}
	
	@Test
	void testSetAppointmentDescriptionIsNotLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Appointment appointment = new Appointment("12345", Calendar.getInstance(), "Birthday Party");
			appointment.setAppointmentDescription("Birthday Party Birthday Party Birthday Party Birthday Party");
		});
	}

}
