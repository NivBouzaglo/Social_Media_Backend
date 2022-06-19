package bgu.spl.net.api.actions;

import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;


public class DataBase {

    private final ConcurrentHashMap<String, user> allUsers;
    private final ConcurrentHashMap<Integer, user> connectedUsers;
    private HashMap<String, String> filteredWord = new HashMap<>();

    public DataBase() {
        allUsers = new ConcurrentHashMap<>();
        connectedUsers = new ConcurrentHashMap<>();
    }

    private static class SingletonHolder {
        private static final DataBase instance = new DataBase();
    }

    public static DataBase getInstance() {
        return SingletonHolder.instance;
    }

    public message register(user user) {
        if (allUsers.containsKey(user.getName()))
            return new ERROR((short) 1);
        else {
            allUsers.put(user.getName(), user);
            return new ack("1");
        }
    }

    public message login(String username, String password, int id) {
        if (!allUsers.containsKey(username)) {
            return new ERROR((short) 2);
        }
        user user = allUsers.get(username);
        if (user.getPassword().compareTo(password) != 0)
            return new ERROR((short) 2);
        else {
            user.setUserid(id);
            connectedUsers.put(id, user);
            return new ack("2");
        }
    }

    public message logout(int id) {
        if (!connectedUsers.containsKey(id))
            return new ERROR((short) 3);
        else {
            connectedUsers.remove(id);
            return new ack("3");
        }
    }

    public message follow(String userName, boolean follow_unfollow, int userId) {
        if (!allUsers.containsKey(userName)) {
            return new ERROR((short) 4);
        }
        if (!connectedUsers.containsKey(userId)) {
            return new ERROR((short) 4);
        }
        if (follow_unfollow) {
            if (!connectedUsers.get(userId).startFollowers(allUsers.get(userName)))
                return new ERROR((short) 4);
            if (!allUsers.get(userName).setFollowing(connectedUsers.get(userId)))
                return new ERROR((short) 4);
            return new ack("4 0 " + userName);
        } else {
            if (connectedUsers.get(userId).unFollower(allUsers.get(userName))) {
                allUsers.get(userName).unFollowing(connectedUsers.get(userId));
                return new ack("4 1 " + userName);
            } else
                return new ERROR((short) 4);
        }
    }

    public message pm(String userName, pm p) {
        if (!allUsers.containsKey(userName)) {
            return new ERROR((short) 6);
        }
        if (!connectedUsers.containsKey(p.Sending_msg_Id())) {
            return new ERROR((short) 6);
        }
        if (connectedUsers.get(p.Sending_msg_Id()).getBlocked().contains(allUsers.get(userName))) {
            return new ERROR((short) 6);
        }
        if (allUsers.get(userName).getBlocked().contains(connectedUsers.get(p.Sending_msg_Id()))) {
            return new ERROR((short) 6);
        }
        if (connectedUsers.containsKey(allUsers.get(userName).getId())) {
            allUsers.get(userName).addPm(p);
            return new notification("PM " ,p, connectedUsers.get(p.Sending_msg_Id()).getName());
        } else {
            allUsers.get(userName).addToUnSeen(new notification("PM",p, connectedUsers.get(p.Sending_msg_Id()).getName()));
            return null;
        }
    }

    public message stats(String username) {
        if (!allUsers.containsKey(username))
            return new ERROR((short) 8);
        user user = allUsers.get(username);
        if (!logged(user))
            return new ERROR((short) 8);
        else
            return new ack("8" + " " + user.getAge() + " " + user.numPost() + " " + user.numFollowers() + " " + user.numFollowing());
    }

    public message block(String userToBlock, int userId) {
        user userR = connectedUsers.get(userId);
        user userB = allUsers.get(userToBlock);
        if (userB != null) {
            userR.getFollowing().remove(userB);
            userB.getFollowing().remove(userR);
            userR.getFollowers().remove(userB);
            userB.getFollowers().remove(userR);
            userR.getBlocked().add(userB);
            return new ack("12");

        }
        return new ERROR((short) 12);
    }

    public ConcurrentHashMap<Integer, user> getConnectedUsers() {
        return connectedUsers;
    }

    public ConcurrentHashMap<String, user> getAllUsers() {
        return allUsers;
    }

    public int getId(String name) {
        if (allUsers.containsKey(name))
            return allUsers.get(name).getId();
        return -1;
    }

    public boolean logged(user user) {
        return connectedUsers.containsKey(user.getId());
    }

    public void addFilter(String word) {
        filteredWord.put(word, word);
    }

    public HashMap<String, String> getFilteredWord() {
        return filteredWord;
    }
}
