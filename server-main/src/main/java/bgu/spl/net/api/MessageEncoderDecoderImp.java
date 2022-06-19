package bgu.spl.net.api;


import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class MessageEncoderDecoderImp implements MessageEncoderDecoder<String> {
    private byte[] message = new byte[1 << 16];
    private byte[] op = new byte[2];
    private String opcode = null;
    private int len = 0;
    private int index = 0;

    /**
     * add the next byte to the decoding process
     *
     * @param nextByte the next byte to consider for the currently decoded
     *                 message
     * @return a message if this byte completes one or null if it doesnt.
     */

    public String decodeNextByte(byte nextByte) {
        System.out.println(nextByte);
        if (nextByte == '\n') {
            return popString();
        } else if (message.length == len)
            message = Arrays.copyOf(message, len * 2);
        else if (opcode != null) {
            decodeAccordingOpcode(nextByte);
        } else if (index < 2) {
            op[index] = nextByte;
            index++;
            if (index == 2) {
                short opcode = bytesToShort(op);
                if (opcode == 1) this.opcode = "REGISTER";
                else if (opcode == 2) this.opcode = "LOGIN";
                else if (opcode == 3) this.opcode = "LOGOUT";
                else if (opcode == 4) this.opcode = "FOLLOW";
                else if (opcode == 5) this.opcode = "POST";
                else if (opcode == 6) this.opcode = "PM";
                else if (opcode == 7) this.opcode = "LOGSTAT";
                else if (opcode == 8) this.opcode = "STAT";
                else if (opcode == 12) this.opcode = "BLOCK";
                index = 3;
            }
        }
        return null;
    }

    public void decodeAccordingOpcode(byte next) {

        if (opcode.equals("LOGIN")) {
            if (next == '\0')
                message[len++] = ' ';
            else if (next == '\1')
                message[len++] = '1';
            else if (next == '\2')
                message[len++] = '0';
            else
                message[len++] = next;
        } else if (opcode.equals("FOLLOW")) {
            if (next == '\0') {
                message[len++] = '0';
                message[len++] = ' ';
            } else if (next == '\1') {
                message[len++] = '1';
                message[len++] = ' ';
            } else
                message[len++] = next;
        } else {
            if (next == '\0')
                message[len++] = ' ';
            else
                message[len++] = next;
        }
    }

    public String popString() {
        String pop;
        if (opcode != null && (opcode.equals("PM") || opcode.equals("PUBLIC")))
            pop = opcode + new String(message, 0, len, StandardCharsets.UTF_8)+";";
        else
           pop = opcode + " " + new String(message, 0, len, StandardCharsets.UTF_8)+";";
        len = 0;
        index = 0;
        message = new byte[1 << 16];
        op = new byte[2];
        opcode = null;
        return pop;
    }

    /**
     * encodes the given message to bytes array
     *
     * @param message the message to encode
     * @return the encoded bytes
     */
    @Override
    public byte[] encode(String message) {
        Queue q = new LinkedBlockingQueue();
        String action = "";
        int i = 0;
        short op;
        while (message.charAt(i) != ' ') {
            action = action + message.charAt(i);
            i++;
        }
        i++;
        if (action.equals("9")) {
            op = 9;
            byte[] no = shortToBytes(op);
            q.add(no[0]);
            q.add(no[1]);
            byte[] temp = message.substring(2).getBytes(StandardCharsets.UTF_8);
            for (int j = 0 ;j < temp.length; j++) {
                q.add(temp[j]);
            }
        } else if (action.equals("10")) {
            op = 10;
            byte[] ac = shortToBytes(op);
            q.add(ac[0]);
            q.add(ac[1]);
            String o = "" + message.charAt(3);
            if (message.charAt(4) != ';' && message.charAt(4) != ' ')
                o = o + message.charAt(4);
            short p = returnOpcode(o);
            byte[] t = shortToBytes(p);
            q.add(t[0]);
            q.add(t[1]);
            if (p == 4) {
                if (message.charAt(5) == '1') {
                    byte[] t1 = shortToBytes((short)1);
                    q.add(t1[0]);
                    q.add(t1[1]);
                }
                else{
                    byte[] t1 = shortToBytes((short)0);
                    q.add(t1[0]);
                    q.add(t1[1]);
                }
                byte[]temp = message.substring(7).getBytes(StandardCharsets.UTF_8);
                for (int j = 0; j < temp.length; j++) {
                    q.add(temp[j]);
                }
            }
            else if (p == 7) {
                int j = 5;
                while ( j<message.length() && message.charAt(j) != ';') {
                    String o1 = "";
                    while (j < message.length() && message.charAt(j) != ' '&& message.charAt(j) != ';') {
                        o1 = o1 + message.charAt(j);
                        j++;
                    }
                    if (j < message.length() && message.charAt(j) == ' ')
                        j++;
                    byte[] b = shortToBytes(Short.parseShort(o1));
                    q.add(b[0]);
                    q.add(b[1]);
                }
            }
            else if (p == 8) {
                int j1 = 5;
                while ( j1<message.length() && message.charAt(j1) != ';') {
                    String o2 = "";
                    while (j1 < message.length() && message.charAt(j1) != ' '&& message.charAt(j1) != ';') {
                        o2 = o2 + message.charAt(j1);
                        j1++;
                    }
                    if (j1 < message.length() && message.charAt(j1) == ' ')
                        j1++;
                    byte[] b = shortToBytes(Short.parseShort(o2));
                    q.add(b[0]);
                    q.add(b[1]);
                }
            }
        } else if (action.equals("11")) {
            op = 11;
            byte[] er = shortToBytes(op);
            q.add(er[0]);
            q.add(er[1]);
            action = "";
            while (message.charAt(i) != ';') {
                action = action + message.charAt(i);
                i++;
            }
            er = shortToBytes(returnOpcode(action));
            q.add(er[0]);
            q.add(er[1]);
        }
        byte[] output = new byte[q.size()];
        int j=0;
        while(!q.isEmpty()) {
            output[j] = (byte) q.poll();
            j++;
        }
        return output;
    }

    /**
     * encodes the given message to bytes array
     *
     * @param num the message to encode
     * @return the encoded bytes
     */
    public byte[] shortToBytes(short num) {
        byte[] bytesArr = new byte[2];
        bytesArr[0] = (byte) ((num >> 8) & 0xFF);
        bytesArr[1] = (byte) (num & 0xFF);
        return bytesArr;
    }

    public short bytesToShort(byte[] byteArr) {
        short result = (short) ((byteArr[0] & 0xff) << 8);
        result += (short) (byteArr[1] & 0xff);
        return result;
    }

    public short returnOpcode(String p) {
        short op = 0;
        if (p.equals("1")) {
            op = 1;
        } else if (p.equals("2"))
            op = 2;
        else if (p.equals("3"))
            op = 3;
        else if (p.equals("4"))
            op = 4;
        else if (p.equals("5"))
            op = 5;
        else if (p.equals("6"))
            op = 6;
        else if (p.equals("7"))
            op = 7;
        else if (p.equals("8"))
            op = 8;
        else if (p.equals("12"))
            op = 12;
        return op;
    }
}
