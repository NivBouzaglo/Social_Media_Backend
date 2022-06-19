package bgu.spl.net.api.actions;

public class ack implements message{
    private final String ack;
    public ack(String ack){
        this.ack = "10 " + ack + ";";
    }
    public message process(){
        return this;
    }
    public String toString(){
        return ack;
    }
    public String string(){return ack;}
}
