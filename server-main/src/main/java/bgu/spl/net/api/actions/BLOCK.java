package bgu.spl.net.api.actions;

public class BLOCK implements message{
    private final String message;
    private String usernameToBlock;
    private int userId;

    public BLOCK(String message,int id){
        this.message=message;
        this.usernameToBlock="";
        this.userId=id;
    }
    @Override
    public message process() {
        int i=0;
        while(message.charAt(i)!=';'){
            usernameToBlock=usernameToBlock+message.charAt(i);
            i++;
        }
        return DataBase.getInstance().block(usernameToBlock,userId);
    }
}
