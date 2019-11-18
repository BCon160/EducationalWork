//Test.java

package ct414;

import java.util.ArrayList;
import java.util.List;
import java.util.Date;

public class Test implements Assessment{
	private int student; 
	private Integer lastQuestion;
	private String info;
	private Date closeDate;
	private List<Question> questions;
	private ArrayList<Integer> answers;
	
	public Test(String information, Date closingDate, List<Question> questionsIn, int stuID){
		super();
		student = stuID;
		info = information;
		closeDate = closingDate;
		questions = questionsIn;
		answers = answersInit(questions.size());
	}
	
	private ArrayList<Integer> answersInit(int size){
		int i;
		ArrayList<Integer> answers = new ArrayList<Integer>(size);
		for (i = 0; i < size; i++){
			answers.add(i,-1);
		}
		return(answers);
	}
	
	public String getInformation(){
		return(info);
	}
	
	public Date getClosingDate(){
		return(closeDate);
	}
	
	public List<Question> getQuestions(){
		return(questions);
	}
	
	public Question getQuestion(int questionNumber) throws 
		InvalidQuestionNumber{
		return(questions.get(questionNumber));
	}
	
	public void selectAnswer(int questionNumber, int optionNumber) throws
		InvalidQuestionNumber, InvalidOptionNumber{
		answers.set(questionNumber, optionNumber);
		lastQuestion = questionNumber;
	}
	
	public int getSelectedAnswer(int questionNumber){
		return(answers.get(questionNumber));
	}
	
	public int getAssociatedID(){
		return(student);
	}
}
