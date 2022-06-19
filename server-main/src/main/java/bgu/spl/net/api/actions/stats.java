package bgu.spl.net.api.actions;

import java.util.LinkedList;
import java.util.Queue;


public class stats implements message{
    private String message;
    private Queue<String> usernames;

    public stats(String message){
        this.message= message;
        this.usernames=new LinkedList<>();
        usernames();
    }
    public Queue<String> getUsernames(){
        return usernames;
    }

    public void usernames(){
        int i=0;
        String username="";
        while(i< message.length()&&message.charAt(i)!= ';'){
            while (message.charAt(i)!= ';'&& message.charAt(i)!='|'){
                username= username+message.charAt(i);
                i++;
            }
            i++;
            usernames.add(username);
            username="";
        }
    }
    @Override
    public message process() {
        return DataBase.getInstance().stats(usernames.poll());
    }
}
