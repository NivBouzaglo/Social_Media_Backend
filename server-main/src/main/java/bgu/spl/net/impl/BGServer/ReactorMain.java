package bgu.spl.net.impl.BGServer;

import bgu.spl.net.api.BidiMessagingProtocolImp;
import bgu.spl.net.api.MessageEncoderDecoderImp;
import bgu.spl.net.srv.Server;

public class ReactorMain {
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("input:port,server");
            System.exit(1);
        }
                     //number of thread           //port
        Server.reactor(Integer.parseInt(args[0]), Integer.parseInt(args[1]), BidiMessagingProtocolImp::new, MessageEncoderDecoderImp::new).serve();
    }
}
