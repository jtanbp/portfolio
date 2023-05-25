import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;

public class DNSHeader {
    //Saved
    private int id; // 2 byte
    private byte[] flags = new byte[2]; // QR / OPCODE (4) / AA / TC / RD / RA / Z / AD / CD / RCODE (4)
    private int QR = 0;
    private int opcode = 0;
    private int AA = 0;
    private int TC = 0;
    private int RD = 0;
    private int RA = 0;
    private int Z = 0;
    private int AD = 0;
    private int CD = 0;
    private int rcode = 0;
    private short queryCount = 0; // QD Count (2 bytes)
    private short answerCount = 0; // AN Count (2 bytes)
    private short authorityCount = 0; // NS Count (2 bytes)
    private short additionalCount = 0; // AR Count (2 bytes)

    public DNSHeader() {
        QR = 0;
        opcode = 0;
        AA = 0;
        TC = 0;
        RD = 0;
        RA = 0;
        Z = 0;
        AD = 0;
        CD = 0;
        rcode = 0;
        queryCount = 0; // QD Count (2 bytes)
        answerCount = 0; // AN Count (2 bytes)
        authorityCount = 0; // NS Count (2 bytes)
        additionalCount = 0; // AR Count (2 bytes)
    }

    // read the header from an input stream (we'll use a ByteArrayInputStream but we will only use the basic read methods of input stream to read 1 byte, or to fill in a byte array, so we'll be generic).
    public static DNSHeader decodeHeader(ByteArrayInputStream inputStream) {
        DNSHeader header = new DNSHeader();

        header.id = (inputStream.read() << 8 | inputStream.read()); //its not casted to short to avoid negative values
        int flags = inputStream.read() << 8 | inputStream.read();

        header.QR = (flags & 0x8000) >> 15;
        header.opcode = (flags & 0x7800) >> 11;
        header.AA = (flags & 0x0400) >> 10;
        header.TC = (flags & 0x0200) >> 9;
        header.RD = (flags & 0x0100) >> 8;
        header.RA = (flags & 0x0080) >> 7;
        header.Z = (flags & 0x0040) >> 6;
        header.AD = (flags & 0x0020) >> 5;
        header.CD = (flags & 0x0010) >> 4;
        header.rcode = flags & 0x000f;

        header.queryCount = (short) (inputStream.read() << 8 | inputStream.read());
        header.answerCount = (short) (inputStream.read() << 8 | inputStream.read());
        header.authorityCount = (short) (inputStream.read() << 8 | inputStream.read());
        header.additionalCount = (short) (inputStream.read() << 8 | inputStream.read());

        return header;
    }

    // This will create the header for the response. It will copy some fields from the request
    public static DNSHeader buildHeaderForResponse(DNSMessage request, DNSMessage response) {
        DNSHeader header = new DNSHeader();

        // For Response QR is set to 1
        header.id = request.header.id;
        header.QR = 1;
        header.opcode = request.header.opcode;
        header.AA = response.header.AA;
        header.TC = response.header.TC;
        header.RD = response.header.RD;
        header.RA = response.header.RA;
        header.Z = response.header.Z;
        header.AD = response.header.AD;
        header.CD = response.header.CD;
        header.rcode = response.header.rcode; //request.header.rcode; //No Error
        header.queryCount = (short) request.header.getQueryCount();
        header.answerCount = (short) response.answers.length;
        header.authorityCount = (short) request.authorityRecords.length;
        header.additionalCount = (short) request.additionalRecords.length;

        return header;
    }

    // encode the header to bytes to be sent back to the client. The OutputStream interface has methods to write a single byte or an array of bytes.
    public void writeBytes(ByteArrayOutputStream outputStream) {
        outputStream.write((id >> 8) & 0xFF);
        outputStream.write(id & 0xFF);

        flags[0] = (byte) (((QR & 0x1) << 7) | ((opcode & 0xF) << 3) | ((AA & 0x1) << 2) | ((TC & 0x1) << 1) | (RD & 0x1));
        flags[1] = (byte) (((RA & 0x1) << 7) | ((Z & 0x7) << 4) | ((AD & 0x1) << 5) | ((CD & 0x1) << 4) | (rcode & 0xF));
        outputStream.write(flags[0]);
        outputStream.write(flags[1]);

        outputStream.write((queryCount >> 8) & 0xFF);
        outputStream.write(queryCount & 0xFF);
        outputStream.write((answerCount >> 8) & 0xFF);
        outputStream.write(answerCount & 0xFF);
        outputStream.write((authorityCount >> 8) & 0xFF);
        outputStream.write(authorityCount & 0xFF);
        outputStream.write((additionalCount >> 8) & 0xFF);
        outputStream.write(additionalCount & 0xFF);
    }

    // Return a human readable string version of a header object. A reasonable implementation can be autogenerated by your IDE.
    public String toString() {
        // return a string representation of the DNSHeader object
        StringBuilder sb = new StringBuilder();

        sb.append("Header:\n");
        sb.append("opcode: ");
        switch(opcode) {
            case 0:
                sb.append("QUERY");
                break;
            case 1:
                sb.append("IQUERY");
                break;
            case 2:
                sb.append("STATUS");
                break;
            case 4:
                sb.append("NOTIFY");
                break;
            case 5:
                sb.append("UPDATE");
                break;
            default:
                sb.append("available for assignment");// for cases 3 and 6 through 15
        }

        sb.append(", status: ");
        switch(rcode) {
            case 0:
                sb.append("NOERROR");
                break;
            case 1:
                sb.append("FORMERR");
                break;
            case 2:
                sb.append("SERVFAIL");
                break;
            case 3:
                sb.append("NXDOMAIN");
                break;
            case 4:
                sb.append("NOTIMP");
                break;
            case 5:
                sb.append("REFUSED");
                break;
            case 6:
                sb.append("YXDOMAIN");
                break;
            case 7:
                sb.append("YXRRSET");
                break;
            case 8:
                sb.append("NXRRSET");
                break;
            case 9:
                sb.append("NOTAUTH");
                break;
            case 10:
                sb.append("NOTZONE");
                break;
            case 16:
                sb.append("BADVERS"); // 16 has another case "BADSIG"
                break;
            case 17:
                sb.append("BADKEY");
                break;
            case 18:
                sb.append("BADTIME");
                break;
            case 19:
                sb.append("BADMODE");
                break;
            case 20:
                sb.append("BADNAME");
                break;
            case 21:
                sb.append("BADALG");
                break;
            case 22:
                sb.append("BADTRUC");
                break;
            default:
                sb.append("available for assignment");// for cases 11-15 assumption also made that anything after 23 as well
        }
        sb.append(", id: " + id + "\n");

        sb.append("flags: ");
        String QR_Result = (QR == 1) ? "qr " : "";
        String AA_Result = (AA == 1) ? "aa " : "";
        String TC_Result = (TC == 1) ? "tc " : "";
        String RD_Result = (RD == 1) ? "rd " : "";
        String RA_Result = (RA == 1) ? "ra " : "";
        String Z_Result = (Z == 1) ? "z " : "";
        String AD_Result = (AD == 1) ? "ad " : "";
        String CD_Result = (CD == 1) ? "cd " : "";
        sb.append(QR_Result + AA_Result + TC_Result + RD_Result + RA_Result + Z_Result + AD_Result + CD_Result + ";");
        sb.append(" QUERY: " + queryCount);
        sb.append(", ANSWER: " + answerCount);
        sb.append(", AUTHORITY: " + authorityCount);
        sb.append(", ADDITIONAL: " + additionalCount);
        sb.append("\n");

        return sb.toString();
    }

    public short getQueryCount() {
        return queryCount;
    }

    public short getAnswerCount() {
        return answerCount;
    }

    public short getAuthorityCount() {
        return authorityCount;
    }

    public short getAdditionalCount() {
        return additionalCount;
    }
}