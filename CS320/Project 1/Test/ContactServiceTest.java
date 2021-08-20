//Author Name: Kyle Hake
//Date: 1/23/2021
//Course ID: CS-320
/*Description:  This file is the contact service test file.
 *It tests the methods of contact service, as well as to make
 *sure all contacts have a unique ID.
 */

package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import contact.ContactService;

class ContactServiceTest {
 
	
	@Test
	void testAddContact() {
		ContactService service = new ContactService();
		service.addContact("12345", "John", "Smith", "0123456789", "123 N Main St.");
		assertFalse(service.contacts.isEmpty());
		assertTrue(service.contacts.get(0).getContactID().equals("12345"));
		assertTrue(service.contacts.get(0).getFirstName().equals("John"));
		assertTrue(service.contacts.get(0).getLastName().equals("Smith"));
		assertTrue(service.contacts.get(0).getPhoneNumber().equals("0123456789"));
		assertTrue(service.contacts.get(0).getAddress().equals("123 N Main St."));
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			service.addContact("12345", "Jim", "Snow", "1928394056", "789 W Main St.");
		});
		
	}

	@Test
	void testDeleteContact() {
		ContactService service = new ContactService();
		service.addContact("12345", "John", "Smith", "0123456789", "123 N Main St.");
		service.deleteContact("12345");
		assertTrue(service.contacts.isEmpty());
		
	}

	@Test
	void testUpdateContact() {
		ContactService service = new ContactService();
		service.addContact("12345", "John", "Smith", "0123456789", "123 N Main St.");
		service.updateContact("12345", "Jane", "Doe", "9876543210", "456 S Main St.");
		assertTrue(service.contacts.get(0).getContactID().equals("12345"));
		assertTrue(service.contacts.get(0).getFirstName().equals("Jane"));
		assertTrue(service.contacts.get(0).getLastName().equals("Doe"));
		assertTrue(service.contacts.get(0).getPhoneNumber().equals("9876543210"));
		assertTrue(service.contacts.get(0).getAddress().equals("456 S Main St."));
		
	}

}

