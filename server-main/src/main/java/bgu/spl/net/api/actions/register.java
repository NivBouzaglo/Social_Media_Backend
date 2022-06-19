package bgu.spl.net.api.actions;

public class register implements message {

    private String message;
    private String username="";
    private String password="";
    private String birth="";

    public register(String message) {
        this.message = message;
    }

    public message process() {
        int index = 0;
        while (message.charAt(index) != ' ') {
            username = username + message.charAt(index);
            index++;
        }
        index++;
        while (message.charAt(index) != ' ') {
            password = password + message.charAt(index);
            index++;
        }
        index++;
        while (message.charAt(index) != ';') {
            birth = birth + message.charAt(index);
            index++;
        }
        user user = new user(this);
        return DataBase.getInstance().register(user);
    }


    public String getBirth() {
        return birth;
    }

    public String getPassword() {
        return password;
    }

    public String getUsername() {
        return username;
    }
}
