//Author Name: Kyle Hake
//Date: 1/23/2021
//Course ID: CS-320
/*Description:  This file is the contact test file.
 *It tests the constructor and setters, as well as input validation
 */


package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import contact.Contact;

class ContactTest {

	//Constructor Testing
	@Test
	void testContact() {
		Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
		assertTrue(contact.getContactID().equals("12345"));
		assertTrue(contact.getFirstName().equals("John"));
		assertTrue(contact.getLastName().equals("Smith"));
		assertTrue(contact.getPhoneNumber().equals("0123456789"));
		assertTrue(contact.getAddress().equals("123 N Main St."));
	}
	
	//Setter Testing
	@Test
	void testSetAddress() {
		Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
		contact.setAddress("456 N Main St.");
		assertTrue(contact.getAddress().equals("456 N Main St."));
	}
	
	@Test
	void testSetFirstName() {
		Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
		contact.setFirstName("James");
		assertTrue(contact.getFirstName().equals("James"));
	}
	
	@Test
	void testSetLastName() {
		Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
		contact.setLastName("Doe");
		assertTrue(contact.getLastName().equals("Doe"));
	}
	
	@Test
	void testSetPhoneNumber() {
		Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
		contact.setPhoneNumber("9876543210");
		assertTrue(contact.getPhoneNumber().equals("9876543210"));
	}
	
	//Input Validation for Construction Testing
	@Test
	void testContactIDTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("01234567891", "John", "Smith", "0123456789", "123 N Main St.");
		});
	}
	
	@Test
	void testContactIDIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact(null, "John", "Smith", "0123456789", "123 N Main St.");
		});
	}

	@Test
	void testFirstNameTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", "Johnjohnjohn", "Smith", "0123456789", "123 N Main St.");
		});
	}
	
	@Test
	void testFirstNameIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", null, "Smith", "0123456789", "123 N Main St.");
		});
	}
	
	@Test
	void testLastNameTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", "John", "SmithSmithSmith", "0123456789", "123 N Main St.");
		});
	}
	
	@Test
	void testLastNameIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", "John", null, "0123456789", "123 N Main St.");
		});
	}
	
	@Test
	void testPhoneNumberTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", "John", "Smith", "01234567890", "123 N Main St.");
		});
	}
	
	@Test
	void testPhoneNumberTooShort() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", "John", "Smith", "012345678", "123 N Main St.");
		});
	}
	
	@Test
	void testPhoneNumberIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", "John", "Smith", null, "123 N Main St.");
		});
	}
	
	@Test
	void testAddressTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", "John", "Smith", "0123456789", "123 N Main St. 123 N Main St. 123 N Main St.");
		});
	}
	
	@Test
	void testAddressIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Contact("12345", "John", "Smith", "0123456789", null);
		});
	}
	
	//Input Validation for Setters Testing
	@Test
	void testSetAddressisNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setAddress(null);
		});
	}
	
	@Test
	void testSetFirstNameisNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setFirstName(null);
		});
	}
	
	@Test
	void testSetLastNameisNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setLastName(null);
		});
	}
	
	@Test
	void testSetPhoneNumberisNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setPhoneNumber(null);
		});
	}
	
	@Test
	void testSetAddressisTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setAddress("123 N Main St. 123 N Main St. 123 N Main St.");
		});
	}
	
	@Test
	void testSetFirstNameisTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setFirstName("Johnjohnjohn");
		});
	}
	
	@Test
	void testSetLastNameisTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setLastName("SmithSmithSmith");
		});
	}
	
	@Test
	void testSetPhoneNumberTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setPhoneNumber("01234567890");
		});
	}
	
	@Test
	void testSetPhoneNumberTooShort() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Contact contact = new Contact("12345", "John", "Smith", "0123456789", "123 N Main St.");
			contact.setPhoneNumber("012345678");
		});
	}


}
