import java.io.*;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.*;
import java.security.cert.Certificate;
import java.security.spec.PKCS8EncodedKeySpec;
import javax.crypto.Mac;

import static java.lang.System.exit;

public class Server {

    final static int _Port = 8080;
    final static String _CASignedCert = "CASignedServerCertificate.pem";
    final static String _PrivateKey = "serverPrivateKey.der";
    final static String _SignType = "SHA256withRSA";
    final static String _MacType = "HmacSHA256";
    public static void main(String[] args) throws Exception {
        Handshake.getCertificate(_CASignedCert);

        // Enables Server
        ServerSocket ss = new ServerSocket(_Port);
        Socket socketToClient = ss.accept();
        System.out.println("Server Connected");
        ObjectInputStream in = new ObjectInputStream(socketToClient.getInputStream());
        ObjectOutputStream out = new ObjectOutputStream( socketToClient.getOutputStream() );

        // Certificate
        Certificate certificate = Handshake.getCertificate(_CASignedCert);

        // Nonce for DH Keys
        byte[] Nonce = (byte[]) in.readObject();

        byte[] DHPrivateKeyArr = SecureRandom.getSeed(32);

        // Using DH method to compute BigInteger
        BigInteger DHPrivateKey = new BigInteger(DHPrivateKeyArr);
        BigInteger DHPublicKey = Handshake.g.modPow(DHPrivateKey, Handshake.n);

        KeyFactory keyFactory = KeyFactory.getInstance("RSA");

        File PrivateKeyFile = new File (_PrivateKey);
        FileInputStream PrivateKeyFileStream = new FileInputStream(PrivateKeyFile);
        byte PrivateKeyString[] = PrivateKeyFileStream.readAllBytes();

        PKCS8EncodedKeySpec KeySpec = new PKCS8EncodedKeySpec(PrivateKeyString);
        PrivateKey PrivateKey = keyFactory.generatePrivate(KeySpec);

        // Signing Key
        Signature PrivateSignature = Signature.getInstance(_SignType);
        PrivateSignature.initSign(PrivateKey);
        PrivateSignature.update(DHPublicKey.toByteArray());
        byte[] DHSignedPublicKey = PrivateSignature.sign();

        // Output Keys to Client
        out.writeObject(certificate);
        out.writeObject(DHPublicKey);
        out.writeObject(DHSignedPublicKey);
        out.flush();

        // Information received from Client
        Certificate clientCert = (Certificate) in.readObject();
        BigInteger clientDHPublicKey = (BigInteger) in.readObject();
        byte[] clientDHSignedPublicKey = (byte[]) in.readObject();

        // Using DH method to compute Key
        BigInteger SharedKey = clientDHPublicKey.modPow((BigInteger) DHPrivateKey, Handshake.n);

        // Write in Secret Keys
        Handshake.makeSecretKeys(Nonce, SharedKey.toByteArray());

        // Mac encryption
        Mac mac = Mac.getInstance(_MacType);
        mac.init(Handshake._serverMAC);

        // Mac Encryption to Bytes
        byte[] NonceMac = mac.doFinal(Nonce);

        byte[] serverCertMac = mac.doFinal(certificate.getEncoded());
        byte[] serverDHPublicKeyMac = mac.doFinal(DHPublicKey.toByteArray());
        byte[] serverDHSignedPublicKeyMac = mac.doFinal(DHSignedPublicKey);

        byte[] clientCertMac = mac.doFinal(clientCert.getEncoded());
        byte[] clientDHPublicKeyMac = mac.doFinal(clientDHPublicKey.toByteArray());
        byte[] clientDHSignedPublicKeyMac = mac.doFinal(clientDHSignedPublicKey);

        // Write to Byte Stream for Output
        ByteArrayOutputStream serverHandshake = new ByteArrayOutputStream();
        serverHandshake.writeBytes(NonceMac);
        serverHandshake.writeBytes(serverCertMac);
        serverHandshake.writeBytes(serverDHPublicKeyMac);
        serverHandshake.writeBytes(serverDHSignedPublicKeyMac);
        serverHandshake.writeBytes(clientCertMac);
        serverHandshake.writeBytes(clientDHPublicKeyMac);
        serverHandshake.writeBytes(clientDHSignedPublicKeyMac);

        // Write byte stream to output
        out.writeObject(serverHandshake.toByteArray());

        // Using MAC to encrypt message
        mac = Mac.getInstance(_MacType);
        mac.init(Handshake._clientMAC);

        // Mac encrypt all previous messages with clientMac for comparison with client records
        NonceMac = mac.doFinal(Nonce);

        serverCertMac = mac.doFinal(certificate.getEncoded());
        serverDHPublicKeyMac = mac.doFinal(DHPublicKey.toByteArray());
        serverDHSignedPublicKeyMac = mac.doFinal(DHSignedPublicKey);


        clientCertMac = mac.doFinal(clientCert.getEncoded());
        clientDHPublicKeyMac = mac.doFinal(clientDHPublicKey.toByteArray());
        clientDHSignedPublicKeyMac = mac.doFinal(clientDHSignedPublicKey);

        // put into byte stream
        serverHandshake = new ByteArrayOutputStream();
        serverHandshake.writeBytes(NonceMac);
        serverHandshake.writeBytes(serverCertMac);
        serverHandshake.writeBytes(serverDHPublicKeyMac);
        serverHandshake.writeBytes(serverDHSignedPublicKeyMac);
        serverHandshake.writeBytes(clientCertMac);
        serverHandshake.writeBytes(clientDHPublicKeyMac);
        serverHandshake.writeBytes(clientDHSignedPublicKeyMac);


        byte[] clientHandshakeByteArr = (byte[]) in.readObject();
        byte[] serverHandshakeByteArr = serverHandshake.toByteArray();

        for(int i = 0; i < clientHandshakeByteArr.length; i++){
            if(serverHandshakeByteArr[i]!=clientHandshakeByteArr[i]){
                System.out.println("Error: Handshake Records do not match!");
                exit(1);
            }
        }
        System.out.println("Handshake Success!");


        // send 2 messages to the client
        String message = "1st Message to Client";
        String message2 = "2nd Message to Client";
        byte[] messageToSend = Message.encrypt(Handshake._serverMAC.getEncoded(), message.getBytes(), Handshake._serverIV.getIV(), Handshake._serverEncrypt.getEncoded());
        byte[] messageToSend2 = Message.encrypt(Handshake._serverMAC.getEncoded(), message2.getBytes(), Handshake._serverIV.getIV(), Handshake._serverEncrypt.getEncoded());

        out.writeObject(messageToSend);
        out.writeObject(messageToSend2);

        // get message from client
        byte[] messageFromClient = (byte[]) in.readObject();
        byte[] stringFromClient = Message.decrypt(messageFromClient, Handshake._clientIV.getIV(), Handshake._clientEncrypt.getEncoded());

        for(int i = 0; i < stringFromClient.length; i++){
            System.out.print((char) stringFromClient[i]);
        }
        System.out.println();
    }
}