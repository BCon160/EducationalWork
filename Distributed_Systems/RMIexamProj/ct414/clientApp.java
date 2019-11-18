//clientApp.java

package ct414;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;

//ClientApp makes a connection to an exam server and allows a studen to take any tests avaiable to them
//Input: 	args[0] = The address of the registry where the exam server is held
//			args[1] = The id of a student
//			args[2] = The password of above student
public class clientApp{
	public static void main(String args[]){
		//Get securiity manager if non is already set
		if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        try {
			//Server name that the client app will lookup
            String name = "ExamServer";
			//Find the registry where the ExamServer can be looked up
            Registry registry = LocateRegistry.getRegistry(args[0]);
			//Look up the exam server on the name server and get a stub of the exam server class
            ExamServer examServ = (ExamServer) registry.lookup(name);
			//scanner will read inputs off the command line
            Scanner scanner = new Scanner(System.in);
			//Read in the student id and password from the command line arguments
			int stuID = Integer.parseInt(args[1]);
            String password = args[2], choice = "";
			//Log the student into the server
            int token = examServ.login(stuID, password);
            System.out.print("Welcome student ");
            System.out.print(stuID);
            System.out.print("\n");
            System.out.println("Note: At any stage (before or after you take a test) type the word 'exit' and press the return key to log off");
			//Keep asking the user which assessment they want to attempt until they type 'exit'
            while (true){
				//Give the student a list of all tests available to them
            	System.out.println("Open assessments:");
				List<String> summary = examServ.getAvailableSummary(token, stuID);
				for (int i = 0; i < summary.size(); i++){
					System.out.print(summary.get(i)+" ");
				}
            	System.out.print("\n");
            	System.out.println("Which assessment would you like to complete?(give course code)");
				choice = scanner.next();
				//Make sure that the student gives a valid input
            	while ((choice.equals("") || !summary.contains(choice)) && !choice.equals("exit")){
					System.out.format("%s is not a valid input. Please type assessment course code or type 'exit' and hit return.\n", choice);
            		choice = scanner.next();
            	}
            	if (choice.equals("exit")){
            		break;
            	}
				//Get an assessment off the exam server
            	Assessment test = examServ.getAssessment(token, stuID, choice);
				//Let the student take the test
				test = runTest(test);
				//Submit the Assessment back to the exam server
            	examServ.submitAssessment(token, stuID, test);
            }
        }
        catch (Exception e) {
            System.err.println("clientApp exception:");
            e.printStackTrace();
        }
    }
    
	//runTest takes an assessment object and allows the user to answer the questions in that Assessment
	//Input: exam = The assessment that the user has specified that they want to do
    private static Assessment runTest(Assessment exam){
		//Hold all of the questions in exam
    	List<Question> Qs = exam.getQuestions();
		//Hold all of the question numbers that have not been answered yet
		List<Integer> openQs = new ArrayList<Integer>(Qs.size());
		//scanner will read in inputs from the command line
    	Scanner scanner = new Scanner(System.in);
    	int choice = -2, ans, ind;
		//Give the student some info on this assessment
    	System.out.println("Module code for this Assessment:");
    	System.out.println(exam.getInformation());
		//Populate the openQs list
    	for (int i = 1; i <= Qs.size(); i++){
    		openQs.add(i-1,i);
    	}
    	while (true){
			//Hold the questions and answers for display to the student
			Question tempQ;
			String[] tempQans;
			//Display all Questions an answers in a nice format
    		for (int i = 0; i < openQs.size(); i++){
				System.out.format("Question %d: ", openQs.get(i));
				tempQ = Qs.get(openQs.get(i)-1);
				System.out.println(tempQ.getQuestionDetail());
				System.out.print("\n");
				tempQans = tempQ.getAnswerOptions();
				for (int j = 0; j < tempQans.length; j++){
					System.out.format("Option %d: ", j+1);
					System.out.println(tempQans[j]+" ");
				}
				System.out.print("\n");
    		}
			//Get the question number that the student wishes to attempt
    		System.out.println("Specify Question number that you want to answer or 0 to exit or -1 to see your previous answers:");
    		choice = Integer.parseInt(scanner.next());
			//Make sure the choice is valid
    		while (choice < -1 || choice > Qs.size()){
					System.out.format("%d is not a valid input. Please give question number or 0 to exit:\n", choice);
            		choice = Integer.parseInt(scanner.next());
            }
			System.out.print("\n");
            if (choice == 0){
            	break;
            }
			//Print the questions and answers that the user has already done
			else if (choice == -1){
				int ansNum;
				System.out.println("Questions and Answers so far");
				for(int k = 0; k < Qs.size(); k++){
					if (!openQs.contains(k+1)){
						System.out.println(Qs.get(k).getQuestionDetail());
						ansNum = exam.getSelectedAnswer(k);
						System.out.format("You chose option: %d which was ", ansNum + 1);
						System.out.println(Qs.get(k).getAnswerOptions()[ansNum]);
					}
				}
				System.out.print("\n");
			}
			else{
			//This block is where the student answers their chosen question
				try{
					//Get and show the questions and answers
					Question curQuestion = exam.getQuestion(choice-1);
					System.out.println(curQuestion.getQuestionDetail());
					String[] posAnswers = curQuestion.getAnswerOptions();
					System.out.print("\n");
					for (int j = 1; j <= posAnswers.length; j++){
						System.out.format("Option %d: %s \n", j, posAnswers[j-1]);
					}
					System.out.print("\n");
					//Ask the student for their answer
					System.out.println("Please give option number or 0 to leave blank:");
					ans = Integer.parseInt(scanner.next());
					//Make sure the choice is valid
					while (ans < 0 || ans > posAnswers.length){
						System.out.println("Please give a valid answer number or 0 to leave blank:");
						ans = Integer.parseInt(scanner.next());
					}
					System.out.print("\n");
					//Let the user leave the answer blank should they wish
					if (ans == 0){
						continue;
					}
					//Otherwise, save the answer they give
					else{
						exam.selectAnswer(choice-1, ans-1);
						//Update the list of hitherto unanswered questions
						ind = openQs.indexOf(choice);
						if (ind != -1){
							openQs.remove(ind);
						}
						
					}
				}
				catch (Exception e) {
				System.err.println("ClientApp exception:");
				e.printStackTrace();
				}
			}
        }
        return(exam);
    }
}
            	
