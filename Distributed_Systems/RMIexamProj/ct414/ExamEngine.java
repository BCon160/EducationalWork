
package ct414;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;
import java.util.Map;
import java.util.Date;
import java.util.HashMap;

public class ExamEngine implements ExamServer {
	
	private List<String> cCodes;
	
	private String[] mathsQs;
	private String[][] mathsAs;
	private String[] computersQs;
	private String[][] computersAs;
	
	private ArrayList<Integer> users;
	private ArrayList<String> passwords;
	private ArrayList<Integer> tokens;
	private ArrayList<Date> dates;
	
	//private ArrayList<ArrayList<Assessment>> oldAss;
	private Map<Integer, ArrayList<Assessment>> saveans = new HashMap<Integer, ArrayList<Assessment>>();
	
    // Constructor is required
    public ExamEngine() {
//here we have our assessments hard coded in.  
        super();
//the two courses with assessments available are MA101, and CS101.  
        cCodes = new ArrayList<String>(Arrays.asList("ma101", "cs101"));
//These two contain the questions and answers to the maths assessment respectively.  
        mathsQs = new String[] {"1+1=", "5+4=", "9-7=", "13-7="};
        mathsAs = new String[][] {{"1","2","3","4"},{"5","7","9","11"},{"0","2","4","3"},{"7","6","9","5"}};
//And these are the q's and a's for the computer assessment.  
        computersQs = new String[] {"Which of the following is a type of loop?", "Which of the following is a common conditional clause keyword?", "What does [] commonly signify?", "What letter precided by a \\ signifies the end of line in a file?"};
        computersAs = new String[][] {{"when","file","else","while"},{"if","for","open","move"},{"a file","an array","an int","a dictionary"},{"t","s","n","b"}};
//they are contained ass strings so that the positioning of them is easy to access for answering the questions.  		
     
//Here are just some User i.d's and their passwords.  	 
        users = new ArrayList<Integer>(Arrays.asList(123,456,789));
        passwords = new ArrayList<String>(Arrays.asList("pass", "word", "hi"));
//the date is initialised so that the end date for the assessment can be accessed.  
		dates = new ArrayList<Date>(Arrays.asList());
//and this is the hardcoded end date for both assessments.  
		Date tempdate = new Date(2016, 2, 6);
		dates.add(tempdate);
		dates.add(tempdate);
		
		tokens = new ArrayList<Integer>();
//this for loop initialises an assignment to associate them with each student.  	
		int j;
		for(j = 0; j < users.size(); j++){
			saveans.put(users.get(j), new ArrayList<Assessment>(Arrays.asList()));
		}
    }

    // Implement the methods defined in the ExamServer interface...
    // Return an access token that allows access to the server for some time period
    public int login(int studentid, String password) throws 
                UnauthorizedAccess, RemoteException {
				Random rand = new Random();
				long date = new Date().getTime();
				rand.setSeed(date);
				
//these nested if statements confirm that if a users i.d is valid, and also the password is present, then a token is returned.  
				if(users.contains(studentid)){
					if(passwords.contains(password)){
						
						int token = rand.nextInt();
						tokens.add(token);
						return(token);
						}
//else, detailed explanations are given depending on which hurdle they fell at.  
					else{
						throw new UnauthorizedAccess("Password not recognised");
					}
				}
				else{
					throw new UnauthorizedAccess("Student I.D not recognised");
				}
    }

    // Return a summary list of Assessments currently available for this studentid
    public List<String> getAvailableSummary(int token, int studentid) throws
                UnauthorizedAccess, NoMatchingAssessment, RemoteException {
//these two validators simply make sure that the user has signed in correctly.  			
				if(!users.contains(studentid)){
					throw new UnauthorizedAccess("student i.d not recognized");
				}
				if(!tokens.contains(token)){
					throw new UnauthorizedAccess("login failed, please ensure you have logged in.  ");
				}
//otherwise, the course codes are returned to the user.  
				return (cCodes);
    }

    // Return an Assessment object associated with a particular course code
    public Assessment getAssessment(int token, int studentid, String courseCode) throws
                UnauthorizedAccess, NoMatchingAssessment, RemoteException {
//here we are initialising variables.  				
				int i;
				int j;
				List<String> studcodes = new ArrayList<String>();
				ArrayList <Assessment> studrec = saveans.get(studentid);
//this for loop runs through each entry in the 
				for(j = 0; j < studrec.size(); j++){
					studcodes.add(studrec.get(0).getInformation());
				}
//these nested if statements ensure that it is the correct assessment being accessed, in this case it is the maths assessment.  
				if(cCodes.indexOf(courseCode)==0){
					if(studcodes.contains(cCodes.get(0))){
						return(studrec.get(studcodes.indexOf(cCodes.get(0))));
						}
					else{
						List<Question> questions = new ArrayList<Question>(Arrays.asList());
//this for loop incrementally goes through each entry in the string and adds it to questions.   
						for(i = 0; i < mathsQs.length; i++){
							questions.add(new q(mathsQs[i], mathsAs[i], i));
							}
//initialising test object, populating it with the questions we just retrieved, and returning it.  
						Test temp = new Test(cCodes.get(0), dates.get(0), questions, studentid);
						return(temp);
					}
				}
//this does the same as the previous if statement except for the change in position in the string, meaning it is for the cs101 assessment.  
				else if(cCodes.indexOf(courseCode)==1){
					if(studcodes.contains(cCodes.get(1))){
						return(studrec.get(studcodes.indexOf(cCodes.get(1))));
						}
					else{
						List<Question> questions = new ArrayList<Question>(Arrays.asList());
//again, this for loop retrieves the computer questions.  
						for(i = 0; i < computersQs.length; i++){
							questions.add(new q(computersQs[i], computersAs[i], i));
							}
//then creates the test object, populates the assessment and returns it.  
						Test temp = new Test(cCodes.get(1), dates.get(1), questions, studentid);
						return(temp);
					}
				}
				else{
					throw new NoMatchingAssessment("No assessment for course code");
					}
    }

    // Submit a completed assessment
    public void submitAssessment(int token, int studentid, Assessment completed) throws 
                UnauthorizedAccess, NoMatchingAssessment, RemoteException {
				
				if(studentid != completed.getAssociatedID()){
					throw new UnauthorizedAccess("Student i.d does not match Assignment id");
				}
				if(!users.contains(studentid)){
					throw new UnauthorizedAccess("student i.d not recognized");
				}
				if(!tokens.contains(token)){
					throw new UnauthorizedAccess("login failed, please ensure you have logged in.  ");
				}
//the answers are stored in the link map linked to the student i.d
				ArrayList<Assessment> sturec = saveans.get(studentid);
//it is then regarded that the student has completed this assignment
				sturec.add(completed);
				saveans.put(studentid, sturec);
    }

    public static void main(String[] args) {
//this if statements makes sure that is there is no security manager present, then it gets one.  
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        try {
//this initialises the name of the server, so that the client code knows what to work with
            String name = "ExamServer";
            ExamServer engine = new ExamEngine();
//this is what is exported whenever the client code put out a request.  
            ExamServer stub = (ExamServer) UnicastRemoteObject.exportObject(engine, 0);
//this works as the index of the server code, for the client code.  A yellow pages as it were.  
            Registry registry = LocateRegistry.getRegistry();
            registry.rebind(name, stub);
//this confirms to the user that the server and client code are connected.  
            System.out.println("ExamEngine bound");
        } catch (Exception e) {
//in case of a networking error while placing the server on the registry, this specifies when and what has happened.  
            System.err.println("ExamEngine exception:");
            e.printStackTrace();
        }
    }
}
