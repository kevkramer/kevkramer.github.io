import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class Day03App {
        public static void main(String[] args) throws IOException {

        BufferedReader inputStream = null;

        try {
            inputStream = new BufferedReader(new FileReader("Day03.txt"));

            String line;

	    int sum = 0;

	    String totline = " ";
	    
            while ((line = inputStream.readLine()) != null) {
		if (line.trim().isEmpty()) {
		    continue;
		}
		totline = totline + line;
	    }

	    StringBuilder result = new StringBuilder();
	    int currentPos = 0;

	    while (true) {
		int startPos = totline.indexOf("don't()", currentPos);
		if (startPos == -1) {
                    // No more "don't" found, append remaining text
                    result.append(totline.substring(currentPos));
                    break;
                }
		result.append(totline.substring(currentPos, startPos));
		int endPos = totline.indexOf("do()", startPos);
                if (endPos == -1) {
                    // No matching "end" found, append rest of text
                    //result.append(totline.substring(startPos));
                    break;
		}
		currentPos = endPos + "do()".length();
            }


	    String lineRegex = result.toString().replaceAll("don\\'t\\(\\).*?do\\(\\)","removed");

	    Pattern pattern = Pattern.compile("mul\\((\\d+)\\,(\\d+)\\)");

	    Matcher matcher = pattern.matcher(lineRegex);

	    while (matcher.find()) {

  		   String match = matcher.group();
		   System.out.println(match);

		   int firstNumber = Integer.parseInt(matcher.group(1));
		   int secondNumber = Integer.parseInt(matcher.group(2));
		   System.out.println(firstNumber + " " + secondNumber);

		   sum += firstNumber*secondNumber;

	    }

	    System.out.println(sum);

        } finally {
            if (inputStream != null) {
                inputStream.close();
            }
        }
    }
}

