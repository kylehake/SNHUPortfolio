//Author Name: Kyle Hake
//Date: 1/23/2021
//Course ID: CS-320
/*Description:  This file is the task class object, 
 *it is used by the Task Service script to add/delete/modify tasks to
 *an array list of tasks.
 */

package task;

public class Task {
	
	// private variables
	private String taskID;
	
	//public variables
	public String taskName;
	public String taskDescription;
	
	//constructors
	public Task(String taskID, String taskName, String taskDescription) {
		if(taskID == null || taskID.length() > 10) {
			throw new IllegalArgumentException("Invalid Task ID");
		}
		if(taskName == null || taskName.length() > 20) {
			throw new IllegalArgumentException("Invalid Task Name");
		}
		if(taskDescription == null || taskDescription.length() > 50) {
			throw new IllegalArgumentException("Invalid Task Description");
		}
			
		this.taskID = taskID;
		this.taskName = taskName;
		this.taskDescription = taskDescription;
	}
	
	//getters
	public String getTaskID() {
		return taskID;
	}
	
	public String getTaskName() {
		return taskName;
	}
		
	public String getTaskDescription() {
		return taskDescription;
	}
	
	//setters
	public void setTaskName(String taskName) {
		if(taskName == null || taskName.length() > 20) {
			throw new IllegalArgumentException("Invalid Task Name");
		}
		this.taskName = taskName;
	}
	
	public void setTaskDescription(String taskDescription) {
		if(taskDescription == null || taskDescription.length() > 50) {
			throw new IllegalArgumentException("Invalid Task Description");
		}
		this.taskDescription = taskDescription;
	}
}