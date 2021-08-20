//Author Name: Kyle Hake
//Date: 1/23/2021
//Course ID: CS-320
/*Description:  This file is the task test file.
 *It tests the constructor and setters, as well as input validation
 */

package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import task.Task;


class TaskTest {
	
	//Constructor Tests
	@Test
	void testTask() {
		Task task = new Task("12345", "Call", "Calls a contact");
		assertTrue(task.getTaskID().equals("12345"));
		assertTrue(task.getTaskName().equals("Call"));
		assertTrue(task.getTaskDescription().equals("Calls a contact"));
	}
	
	//Setter Tests
	@Test
	void testSetTaskName() {
		Task task = new Task("12345", "Call", "Calls a contact");
		task.setTaskName("Email");
		assertTrue(task.getTaskName().equals("Email"));
	}
	
	@Test
	void testSetTaskDescription() {
		Task task = new Task("12345", "Call", "Calls a contact");
		task.setTaskDescription("Opens Email");
		assertTrue(task.getTaskDescription().equals("Opens Email"));
	}
	
	//Input Validation Tests
	@Test
	void testTaskIDTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("12345123451", "Call", "Calls a contact");
		});
	}
	
	@Test
	void testTaskIDIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task(null, "Call", "Calls a contact");
		});
	}
	
	@Test
	void testTaskNameTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("12345123451", "This name is too long", "Calls a contact");
		});
	}
	
	@Test
	void testTaskNameIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("12345", null, "Calls a contact");
		});
	}
	
	@Test
	void testTaskDescriptionTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("12345123451", "This name is too long", "This description is too long This description is too long");
		});
	}
	
	@Test
	void testTaskDescriptionIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			new Task("12345", "Call", null);
		});
	}
	
	//Set Input Validation Tests
	@Test
	void testSetTaskNameTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Task task = new Task("12345", "Call", "Calls a contact");
			task.setTaskName("This name is too long");
		});
	}
	
	@Test
	void testSetTaskNameIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Task task = new Task("12345", "Call", "Calls a contact");
			task.setTaskName(null);
		});
	}
	
	@Test
	void testSetTaskDescriptionTooLong() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Task task = new Task("12345", "Call", "Calls a contact");
			task.setTaskDescription("This description is too long This description is too long");
		});
	}
	
	@Test
	void testSetTaskDescriptionIsNull() {
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			Task task = new Task("12345", "Call", "Calls a contact");
			task.setTaskDescription(null);
		});
	}

}
