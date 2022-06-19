package bgu.spl.net.api.actions;

import java.util.Queue;

public class login implements message {
    private String message;
    private String username = "";
    private String password = "";
    private int newId;

    public login(String message , int id) {
        this.message = message;
        newId = id;

    }

    public message process() {
        int index = 0;
        while (message.charAt(index) != ' ') {
            username = username + message.charAt(index);
            index++;
        }index++;
        while (message.charAt(index) != ' ') {
            password = password + message.charAt(index);
            index++;
        }
        index++;
        if(message.charAt(index) == '1' ) {
            return DataBase.getInstance().login(username, password , newId);
        }
        else
            return new ERROR(((short) 2));
    }
    public Queue<notification> unSeen(){
        return DataBase.getInstance().getAllUsers().get(username).getUnSeen();
    }
}
