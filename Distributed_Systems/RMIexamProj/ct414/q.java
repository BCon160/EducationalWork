package ct414;


public class q implements Question{
	private String question;
	private String[] answers;
	private int number;
	

	public q(String q, String[] ans, int num){
		question = q;
		answers = ans;
		number = num;
	}
	// Return the question number
	public int getQuestionNumber(){
		return(number);
		}

	// Return the question text
	public String getQuestionDetail(){
		return(question);
		}
	
	// Return the possible answers to select from
	public String[] getAnswerOptions(){
		return(answers);
	}
}