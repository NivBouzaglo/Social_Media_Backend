package bgu.spl.net.api.actions;

public class ERROR implements message{
    private short error;
    public ERROR(short error){
        this.error = error;
    }
    public String toString(){
        short opcode = 11;
        return opcode + " " + error + ";";
    }

    @Override
    public message process() {
        return this;
    }
}
