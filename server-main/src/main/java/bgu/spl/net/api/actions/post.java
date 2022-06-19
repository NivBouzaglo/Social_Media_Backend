package bgu.spl.net.api.actions;

import java.util.List;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class post implements message {
    private String input;
    private String post = "";
    private int id;
    private Queue<String> hashtag = new LinkedBlockingQueue<>();
    private List<user> follower;
    private int index = 0;
    private DataBase b = DataBase.getInstance();

    public post(String substring, int connectionId) {
        input = substring;
        id = connectionId;
        follower = DataBase.getInstance().getConnectedUsers().get(id).getFollowing();
        send();
    }

    @Override
    public message process() {
        return new notification("PUBLIC", this, DataBase.getInstance().getConnectedUsers().get(id).getName());
    }

    public void send() {
        for (int i = 0; i < input.length(); i++) {
            post = post + input.charAt(i);
            if (input.charAt(i) == '@') {
                String name = "";
                i++;
                while (i < input.length() && input.charAt(i) != ' ') {
                    post = post + input.charAt(i);
                    name = name + input.charAt(i);
                    i++;
                }
                post = post + input.charAt(i);
                if (b.getAllUsers().containsKey(name) && b.getConnectedUsers().get(id).getBlocked().contains(b.getAllUsers().get(name)))
                    hashtag.add(name);
            }
        }
        b.getConnectedUsers().get(id).addPost(this);
    }

    public boolean hasNext() {
        return index >= 0 && (index < follower.size() || !hashtag.isEmpty());
    }

    public int next() {
        int i;
        if (!follower.isEmpty() && index < follower.size() && index >= 0) {
            if (!b.getConnectedUsers().containsKey(follower.get(index).getId())) {
                follower.get(index).addToUnSeen(new notification("PUBLIC", this, DataBase.getInstance().getConnectedUsers().get(id).getName()));
                i = -1;
                index++;
            } else {
                i = follower.get(index).getId();
                index++;
            }
        } else {
            if (!b.getAllUsers().containsKey(hashtag.peek())) {
                i = -1;
                hashtag.poll();
            } else if (b.getConnectedUsers().containsKey(b.getAllUsers().get(hashtag.peek()).getId())) {
                if (!follower.contains(b.getAllUsers().get(hashtag.peek()))) {
                    i = b.getAllUsers().get(hashtag.peek()).getId();
                    hashtag.poll();
                } else {
                    hashtag.poll();
                    i = -1;
                }
            } else {
                b.getAllUsers().get(hashtag.poll()).addToUnSeen(new notification("PUBLIC", this, DataBase.getInstance().getConnectedUsers().get(id).getName()));
                i = -1;
            }
        }
        return i;
    }

    public String toString() {
        return post;
    }

}
