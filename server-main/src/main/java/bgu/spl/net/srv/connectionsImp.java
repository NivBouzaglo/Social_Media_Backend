package bgu.spl.net.srv;


import bgu.spl.net.api.actions.message;
import bgu.spl.net.api.bidi.ConnectionHandler;


import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;

public class connectionsImp<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler> connectionHandlerMap = new ConcurrentHashMap<Integer, ConnectionHandler>();

    @Override
    public boolean send(int connectionId, T msg) {
        if (connectionHandlerMap.containsKey(connectionId)) {
            connectionHandlerMap.get(connectionId).send(msg.toString());
            return true;
        }
        return false;
    }
    public void add(int key, ConnectionHandler handler) {
        connectionHandlerMap.putIfAbsent(key, handler);
    }

    @Override
    public void broadcast(T msg) {}

    @Override
    public void disconnect(int connectionId) {
        try {
            connectionHandlerMap.get(connectionId).close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        connectionHandlerMap.remove(connectionId);
    }
}
