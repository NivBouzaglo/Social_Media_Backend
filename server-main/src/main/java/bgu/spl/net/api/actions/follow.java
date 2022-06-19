package bgu.spl.net.api.actions;

public class follow implements message{
    private String message;
    private String userName="";
    private boolean follow_unfollow;
    private int userId;

    public follow(String message , int userId){
        this.userId =userId;
        this.message = message;
    }
    public message process() {
        if (message.charAt(0) == '1')
            follow_unfollow = false;
        else
            follow_unfollow = true;
        int i = 2;
        while (message.charAt(i) != ';') {
            userName = userName + message.charAt(i);
            i++;
        }
        return DataBase.getInstance().follow(userName , follow_unfollow , userId);
    }
}
