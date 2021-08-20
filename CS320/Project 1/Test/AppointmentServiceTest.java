//Author Name: Kyle Hake
//Date: 2/3/2021
//Course ID: CS-320
/*Description:  This file is the appointment service test file.
 *It tests the methods of appointment service, as well as to make
 *sure all appointments have a unique ID.
 */
package test;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Calendar;


import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import appointment.AppointmentService;

class AppointmentServiceTest {

	@Test
	void testAddAppointment() {
		AppointmentService service = new AppointmentService();
		Calendar date = Calendar.getInstance();
		date.add(Calendar.DATE, 1);
		service.addAppointment("12345", date , "Birthday Party");
		assertFalse(service.appointments.isEmpty());
		assertTrue(service.appointments.get(0).getAppointmentID().equals("12345"));
		assertTrue(service.appointments.get(0).getAppointmentDate().equals(date));
		assertTrue(service.appointments.get(0).getAppointmentDescription().equals("Birthday Party"));
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			service.addAppointment("12345", Calendar.getInstance(), "Wedding");
		});
	}
	
	@Test
	void testDeleteAppointment() {
		AppointmentService service = new AppointmentService();
		Calendar date = Calendar.getInstance();
		date.add(Calendar.DATE, 1);
		service.addAppointment("12345", date , "Birthday Party");
		service.deleteAppointment("12345");
		assertTrue(service.appointments.isEmpty());
		
	}

}
