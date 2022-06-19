package bgu.spl.net.srv;

import bgu.spl.net.api.bidi.ConnectionHandler;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void broadcast(T msg);

    void disconnect(int connectionId);
    void add(int key , ConnectionHandler h);
}
