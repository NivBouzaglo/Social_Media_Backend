package bgu.spl.net.api.actions;

import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class user {
    private int Userid;
    private final String name;
    private final String password;
    private final String birthday;
    private List<user> followers= new LinkedList<>();
    private List<user> following = new LinkedList<>();
    private Queue<message> post=new LinkedBlockingQueue<>();
    private Queue<message> pm= new LinkedBlockingQueue<>();
    private  List<user> blocked;
    private Queue<notification> unSeen;

    public user(register r) {
        name = r.getUsername();
        password = r.getPassword();
        birthday = r.getBirth();
        blocked = new LinkedList<>();
        unSeen = new LinkedBlockingQueue<>();
    }

    public List<user> getBlocked() {
        return blocked;
    }

    public String getName() {
        return name;
    }


    public List<user> getFollowers() {
        return followers;
    }

    public String getPassword() {
        return password;
    }


    public List<user> getFollowing() {
        return following;
    }

    public Queue<message> getPost() {
        return post;
    }

    public Queue<message> getPm() {
        return pm;
    }

    public boolean startFollowers(user user) {
        if (!user.blocked.contains(this)) {
            this.followers.add(user);
            return true;
        }
        return false;
    }

    public boolean setFollowing(user user) {
        if (!blocked.contains(user)) {
            this.following.add(user);
            return true;
        }
        return false;
    }

    public boolean unFollower(user user) {
        if (followers.contains(user)) {
            this.followers.remove(user);
            return true;
        }
        return false;
    }

    public boolean unFollowing(user user) {
        return this.following.remove(user);
    }

    public void Logout() {
        DataBase.getInstance();
    }

    public void addPost(message m) {
        post.add(m);
    }

    public void addPm(message m) {
        pm.add(m);
    }

    public message logout() {
        return DataBase.getInstance().logout(Userid);
    }

    public int getId() {
        return this.Userid;
    }

    public short getAge() {
        return (short) (2021 - Integer.parseInt(birthday.substring(6,10)));
    }

    public short numPost() {
        if (!post.isEmpty())
          return (short) post.size();
        else
            return (short)0;
    }

    public short numFollowing() {
        return (short) following.size();
    }

    public short numFollowers() {
        return (short) followers.size();
    }

    public Queue<notification> getUnSeen() {
        return unSeen;
    }

    public void addToUnSeen(notification p) {
        unSeen.add(p);
    }

    public void setUserid(int id) {
        this.Userid = id;
    }
}