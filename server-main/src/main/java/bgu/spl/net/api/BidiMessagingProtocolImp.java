package bgu.spl.net.api;

import bgu.spl.net.api.actions.*;
import bgu.spl.net.api.bidi.BidiMessagingProtocol;
import bgu.spl.net.srv.Connections;

import java.util.List;

import static bgu.spl.net.api.actions.notification.type.PUBLIC;

public class BidiMessagingProtocolImp<T> implements BidiMessagingProtocol<String> {
    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections connections;

    /**
     * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
     *
     * @param connectionId
     * @param connections
     */

    public void start(int connectionId, Connections connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }


    /**
     * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
     * *@param connectionId
     * * @param connections
     */

    public void process(String message) {
        message m;
        message answer;
        int index = 0;
        String opcode = "";
        while (message.charAt(index) != ' ') {
            opcode = opcode + message.charAt(index);
            index++;
        }
        if (opcode.equals("REGISTER")) {
            m = new register(message.substring(index+1));
            answer = m.process();
            connections.send(connectionId, answer);
        } else if (opcode.equals("LOGIN")) {
            m = new login(message.substring(index+1), connectionId);
            answer = m.process();
            connections.send(connectionId, answer);
            if (answer instanceof ack) {
                while (!((login) m).unSeen().isEmpty()) {
                    connections.send(connectionId, ((login) m).unSeen().peek());
                    if (((login) m).unSeen().peek().getType()== PUBLIC)
                        DataBase.getInstance().getConnectedUsers().get(connectionId).addPost(((login) m).unSeen().poll());
                    else
                        DataBase.getInstance().getConnectedUsers().get(connectionId).addPm(((login) m).unSeen().poll());
                }
            }
        } else if (opcode.equals("LOGOUT")) {
            answer = DataBase.getInstance().logout(connectionId);
            connections.send(connectionId, answer);
            if (answer instanceof ack)
                connections.disconnect(connectionId);
        } else if (opcode.equals("FOLLOW")) {
            if (!DataBase.getInstance().getConnectedUsers().containsKey(connectionId))
                connections.send(connectionId, new ERROR((short) 4));
            else {
                follow f = new follow(message.substring(index+1), connectionId);
                answer = f.process();
                connections.send(connectionId, answer);
            }
        } else if (opcode.equals("POST")) {
            if (!DataBase.getInstance().getConnectedUsers().containsKey(connectionId))
                connections.send(connectionId, new ERROR((short) 5));
            else {
                post post = new post(message.substring(index+1), connectionId);
                connections.send(connectionId, new ack("5"));
                while (post.hasNext()) {
                    answer = post.process();
                    int id = post.next();
                    connections.send(id, answer);
                    }
                }
            }
         else if (opcode.equals("PM")) {
            if (!DataBase.getInstance().getConnectedUsers().containsKey(connectionId))
                connections.send(connectionId, new ERROR((short) 6));
            else {
                pm pm = new pm(message.substring(index+1), connectionId);
                answer = pm.process();
                int id = pm.getId();
                if (answer instanceof ERROR)
                    connections.send(connectionId, new ERROR((short) 6));
                else if (id != -1) {
                    if (answer != null)
                        connections.send(id, answer);
                    connections.send(connectionId, new ack("6"));
                } else
                    connections.send(connectionId, new ERROR((short) 6));
            }
        } else if (opcode.equals("LOGSTAT")) {
            loggedInStates l = new loggedInStates();
            if(!DataBase.getInstance().getConnectedUsers().containsKey(connectionId)){
                connections.send(connectionId, new ERROR((short) 7));
            }
            else {
                while (!l.done()) {
                    answer = l.process();
                    connections.send(connectionId, answer);
                }
            }
        } else if (opcode.equals("STAT")) {
           if (!DataBase.getInstance().getConnectedUsers().containsKey(connectionId)) {
                connections.send(connectionId, new ERROR((short) 8));
            }
            else {
                stats s = new stats(message.substring(index+1));
                while (!s.getUsernames().isEmpty()) {
                    answer = s.process();
                    connections.send(connectionId, answer);
                }
            }
        } else if (opcode.equals("BLOCK")) {
            if (!DataBase.getInstance().getConnectedUsers().containsKey(connectionId))
                connections.send(connectionId, new ERROR((short) 12));
            else if (!DataBase.getInstance().getConnectedUsers().containsKey(connectionId))
                connections.send(connectionId, new ERROR((short) 12));
            else {
                BLOCK b = new BLOCK(message.substring(index+1), connectionId);
                answer = b.process();
                connections.send(connectionId, answer);
            }
        }
    }


    /**
     * @return true if the connection should be terminated
     */
    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
