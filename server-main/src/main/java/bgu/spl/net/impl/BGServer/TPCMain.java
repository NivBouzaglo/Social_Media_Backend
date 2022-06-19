package bgu.spl.net.impl.BGServer;

import bgu.spl.net.api.BidiMessagingProtocolImp;
import bgu.spl.net.api.MessageEncoderDecoderImp;
import bgu.spl.net.srv.Server;

public class TPCMain {
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("input:port,server");
            System.exit(1);
        }
        Server.threadPerClient(Integer.parseInt(args[0]), BidiMessagingProtocolImp::new, MessageEncoderDecoderImp::new).serve();
    }
}
