package bgu.spl.net.api.actions;

import static bgu.spl.net.api.actions.notification.type.PM;
import static bgu.spl.net.api.actions.notification.type.PUBLIC;

public class notification implements message {
    public enum type{PM , PUBLIC};
    private message p;
    private type type;

    private String username;

    public notification(String type , message p, String username) {
        this.username = username;
        this.p = p;
        setType(type);
    }

    private void setType(String type) {
        if (type.equals("PM "))
            this.type = PM;
        else this.type =PUBLIC;
    }

    public notification.type getType() {
        return type;
    }

    public message process() {
        return this;
    }

    public String toString() {
        if (type == PUBLIC)
            return "9 0 " + username + p.toString();
        else return "9 1 " +username +" "+ p.toString();
    }
}
