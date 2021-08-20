//Author Name: Kyle Hake
//Date: 1/23/2021
//Course ID: CS-320
/*Description:  This file is the tasks service 
 *it uses task objects to make an array list of tasks.
 *Can add/remove/modify tasks from this array list.
 */

package task;

import java.util.ArrayList;
import java.util.List;


public class TaskService {
	
	//List of tasks for add/delete/update methods
		public List<Task> tasks = new ArrayList<Task>();
		
		//Constructors
		
		public TaskService() {
			
		}
		
		//Methods
		
		//gets size of tasks list, used for iteration in while loops
		public int getTaskCount() {
			return tasks.size();
		}
		
		//adds task to task list
		public Task addTask(String taskID, String taskName, String taskDescription) {
			
			Task task = null;
			
			int i = 0;
			
			//checks to see if there is a task with the same task ID
			while(i < getTaskCount()) {
				
				//if there is a match, throws exception
				if (taskID == tasks.get(i).getTaskID()) {
					task = tasks.get(i);
					throw new IllegalArgumentException("Duplicate taskId not allowed.");
				}
			}
			
			//if not match, adds the task to the task list
			if (task == null) {
				task = new Task(taskID, taskName, taskDescription);
				tasks.add(task);
			}
			
			
			return task;
		}
		
		//deletes task based on taskID
		public void deleteTask(String taskID) {
			
			int i = 0;
			
			while (i < getTaskCount()) {
				if (taskID == tasks.get(i).getTaskID()) {
					tasks.remove(i);
				}
			}
		}
		
		//updates the task information based on taskID
		public void updateTask(String taskID, String taskName, String taskDescription) {
			
			int i = 0;
			
			while (i < getTaskCount()) {
				if (taskID == tasks.get(i).getTaskID()) {
					tasks.get(i).setTaskName(taskName);
					tasks.get(i).setTaskDescription(taskDescription);
					break;
				}
			}
			
		}
		
		

}
