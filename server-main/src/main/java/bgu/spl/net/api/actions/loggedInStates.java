package bgu.spl.net.api.actions;

import java.util.Queue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;

public class loggedInStates implements message{


    private ConcurrentHashMap<Integer , user> loginUsers;
    private Queue<Integer> k=new LinkedBlockingQueue<>();
    private Integer id ;

    public loggedInStates(){
        loginUsers = DataBase.getInstance().getConnectedUsers();
        for (Integer i : loginUsers.keySet()) {
            k.add(i);
        }
    }
    public message process(){
            id =k.poll();
            user user =loginUsers.get(id);
            return new ack("7"+" "+user.getAge()+" "+ user.numPost()+" "+user.numFollowers()+" "+user.numFollowing());
    }
    public boolean done(){
        return k.size() ==0;
    }
    public int getId(){
        return id;
    }
}
