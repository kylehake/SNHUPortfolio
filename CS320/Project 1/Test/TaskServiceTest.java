//Author Name: Kyle Hake
//Date: 1/23/2021
//Course ID: CS-320
/*Description:  This file is the task service test file.
 *It tests the methods of task service, as well as to make
 *sure all tasks have a unique ID.
 */

package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import task.TaskService;

class TaskServiceTest {

	@Test
	void testAddTask() {
		TaskService service = new TaskService();
		service.addTask("12345", "Call", "Calls a contact");
		assertFalse(service.tasks.isEmpty());
		assertTrue(service.tasks.get(0).getTaskID().equals("12345"));
		assertTrue(service.tasks.get(0).getTaskName().equals("Call"));
		assertTrue(service.tasks.get(0).getTaskDescription().equals("Calls a contact"));
		Assertions.assertThrows(IllegalArgumentException.class, () -> {
			service.addTask("12345", "Email", "Opens Email");
		});
		
	}

	@Test
	void testDeleteTask() {
		TaskService service = new TaskService();
		service.addTask("12345", "Call", "Calls a contact");
		service.deleteTask("12345");
		assertTrue(service.tasks.isEmpty());
		
	}

	@Test
	void testUpdateTask() {
		TaskService service = new TaskService();
		service.addTask("12345", "Call", "Calls a contact");
		service.updateTask("12345", "Email", "Opens Email");
		assertTrue(service.tasks.get(0).getTaskID().equals("12345"));
		assertTrue(service.tasks.get(0).getTaskName().equals("Email"));
		assertTrue(service.tasks.get(0).getTaskDescription().equals("Opens Email"));
		
	}

}
