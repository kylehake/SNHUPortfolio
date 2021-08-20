//Author Name: Kyle Hake
//Date: 1/23/2021
//Course ID: CS-320
/*Description:  This file is the contact service 
 *it uses contact objects to make an array list of contacts.
 *Can add/remove/modify contacts from this array list.
 */

package contact;

import java.util.ArrayList;
import java.util.List;

public class ContactService {
	
	//List of contacts for add/delete/update methods
	public List<Contact> contacts = new ArrayList<Contact>();
	
	//Constructors
	
	public ContactService() {
		
	}
	
	//Methods
	
	//gets size of contacts list, used for iteration in while loops
	public int getContactCount() {
		return contacts.size();
	}
	
	//adds contact to contact list
	public Contact addContact(String contactID, String firstName, String lastName, String phoneNumber, String address) {
		
		Contact contact = null;
		
		int i = 0;
		
		//checks to see if there is a contact with the same contact ID
		while(i < getContactCount()) {
			
			//if there is a match, throws exception
			if (contactID == contacts.get(i).getContactID()) {
				contact = contacts.get(i);
				throw new IllegalArgumentException("Duplicate contactId not allowed.");
			}
		}
		
		//if not match, adds the contact to the contact list
		if (contact == null) {
			contact = new Contact(contactID, firstName, lastName, phoneNumber, address);
			contacts.add(contact);
		}
		
		
		return contact;
		
	}
	
	//deletes contact based on contactID
	public void deleteContact(String contactID) {
		
		int i = 0;
		
		while (i < getContactCount()) {
			if (contactID == contacts.get(i).getContactID()) {
				contacts.remove(i);
			}
		}
	}
	
	//updates the contact information based on 
	public void updateContact(String contactID, String firstName, String lastName, String phoneNumber, String address) {
		
		int i = 0;
		
		while (i < getContactCount()) {
			if (contactID == contacts.get(i).getContactID()) {
				contacts.get(i).setFirstName(firstName);
				contacts.get(i).setLastName(lastName);
				contacts.get(i).setPhoneNumber(phoneNumber);
				contacts.get(i).setAddress(address);
				break;
			}
		}
		
	}

}