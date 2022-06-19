package bgu.spl.net.api.actions;

public class pm implements message {
    private String input;
    private String p = "";
    private int id;
    private String name = "";

    public pm(String m, int id) {
        input = m;
        this.id = id;
    }

    @Override
    public message process() {
        int index=0;
        while (input.charAt(index) != ' '){
            name = name + input.charAt(index);
            index++;
        }
        index++;
        while(index<input.length()) {
            String word="";
            while (index<input.length()&&input.charAt(index) != ' '){
                word = word +input.charAt(index);
                index++;
            }
            index++;
            if (DataBase.getInstance().getFilteredWord().containsKey(word))
                p = p + "<filtered>";
            else
                p = p + word;
            if (index != input.length() - 1)
                p = p + " ";
        }
        return DataBase.getInstance().pm(name , this);
    }
    public int getId(){
        return DataBase.getInstance().getId(name);
    }

    public int Sending_msg_Id(){
        return id;
    }

    public String toString() {
        return p;
    }
}
